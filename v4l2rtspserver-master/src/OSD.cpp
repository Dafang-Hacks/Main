#include "../inc/OSD.hpp"

#include <stdexcept>
#include <tuple>

#include <ctime>

#define LOGURU_WITH_STREAMS 1
#include <loguru.hpp>

#include "sharedmem.h"
#include "../../v4l2rtspserver-tools/sharedmem.h"


#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_MODULE_H
#include FT_DRIVER_H

OSD::OSD(int x, int y, int width, int height, int layer): _x(x), _y(y), _width(width), _height(height) {
    LOG_S(INFO) <<  "[OSD]: Created OSD(" << x << ", " << y << ", " << width << ", " << height << ", " << layer << ")";

    region = IMP_OSD_CreateRgn(NULL);

    if (region == INVHANDLE) {
        throw std::runtime_error("Could not create region");
    }

    if (IMP_OSD_RegisterRgn(region, 0, NULL) != 0) {
        throw std::runtime_error("Could not register region");
    }

    image = nullptr;
    setBounds(x, y, width, height);

    IMPOSDGrpRgnAttr group_attributes;

    // XXX: unnecessary since we don't read them?
    if (IMP_OSD_GetGrpRgnAttr(region, 0, &group_attributes) != 0) {
        throw std::runtime_error("Could not get group region attributes");
    }

    _layer = layer;

    memset(&group_attributes, 0, sizeof(IMPOSDGrpRgnAttr));
    group_attributes.show = 0;
    group_attributes.gAlphaEn = 0;
    group_attributes.fgAlhpa = 0;
    group_attributes.bgAlhpa = 0;
    group_attributes.layer = _layer + 1;

    if (IMP_OSD_SetGrpRgnAttr(region, 0, &group_attributes) != 0) {
        throw std::runtime_error("Could not set group region attributes");
    }

    return;
}

OSD::~OSD() {
    free(image);
}

int OSD::getX() {
    return _x;
}

int OSD::getY() {
    return _y;
}

int OSD::getWidth() {
    return _width;
}

int OSD::getHeight() {
    return _height;
}

void OSD::setBounds(int x, int y, int width, int height) {
    _x = x;
    _y = y;
    _width = width;
    _height = height;

    IMPOSDRgnAttr attributes;

    memset(&attributes, 0, sizeof(IMPOSDRgnAttr));
    attributes.type = OSD_REG_PIC;
    attributes.rect.p0.x = _x;
    attributes.rect.p0.y = _y;
    attributes.rect.p1.x = _x + _width - 1;
    attributes.rect.p1.y = _y + _height - 1;
    attributes.fmt = PIX_FMT_ABGR;  // Actually RGBA?

    if (IMP_OSD_SetRgnAttr(region, &attributes) != 0) {
        throw std::runtime_error("Could not set boundary attributes");
    }

    clear();
}

void OSD::clear() {
    if (image != nullptr) {
        image = (uint32_t*)realloc(image, sizeof(uint32_t) * _width * _height);
    } else {
        image = (uint32_t*)malloc(sizeof(uint32_t) * _width * _height);
    }

    memset(image, RGBAColor::TRANSPARENT, sizeof(uint32_t) * _width * _height);
}

void OSD::update() {
    IMPOSDRgnAttrData attributes_data;
    memset(&attributes_data, 0, sizeof(attributes_data));
    attributes_data.picData.pData = (void*)image;

    if (IMP_OSD_UpdateRgnAttrData(region, &attributes_data) != 0) {
        throw std::runtime_error("Could not update region attributes");
    }
}

void OSD::show(bool flag) {
    if (IMP_OSD_ShowRgn(region, 0, flag) != 0) {
        throw std::runtime_error("Could not show region");
    }
}

void OSD::drawBitmap(int x, int y, int width, int height, uint32_t *pixels) {
    for (int b_y = 0; b_y < height; b_y++) {
        for (int b_x = 0; b_x < width; b_x++) {
            int i_x = x + b_x;
            int i_y = y + b_y;

            if (i_x < 0 || i_y < 0 || i_x > _width || i_y > _height) {
                continue;
            }

            setPixel(i_x, i_y, pixels[b_y * width + b_x]);
        }
    }
}

void OSD::setPixel(int x, int y, uint32_t value) {
    if ((x < 0) || (y < 0) || (x > _width) || (y > _height)) {
        throw std::invalid_argument("Invalid target coordinates");
    }

    image[y * _width + x] = value;
}

uint32_t OSD::getPixel(int x, int y) {
    if ((x < 0) || (y < 0) || (x > _width) || (y > _height)) {
        throw std::invalid_argument("Invalid target coordinates");
    }

    return image[y * _width + x];
}


std::pair<int, int> get_vertical_font_dimensions(FT_Face &face) {
    int min_below = INT_MAX;
    int max_above = INT_MIN;

    FT_GlyphSlot slot = face->glyph;
    int last_glyph_index = 0;

    // XXX: surely there's a better way to do this
    for (char c = ' '; c < '~'; c++) {
        int glyph_index = FT_Get_Char_Index(face, c);

        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) != 0) {
            throw std::runtime_error("Could not load glyph for character");
        }

        if (FT_Render_Glyph(slot, FT_RENDER_MODE_NORMAL) != 0) {
            throw std::runtime_error("Could not render glyph for character");
        }

        FT_Vector kerning_delta = {0, 0};

        if (last_glyph_index && glyph_index) {
            FT_Get_Kerning(face, last_glyph_index, glyph_index, FT_KERNING_DEFAULT, &kerning_delta);
        }

        last_glyph_index = glyph_index;

        if (min_below > (int)kerning_delta.y + (int)slot->bitmap_top - (int)slot->bitmap.rows) {
            min_below = (int)kerning_delta.y + (int)slot->bitmap_top - (int)slot->bitmap.rows;
        }

        if (max_above < (int)kerning_delta.y + (int)slot->bitmap.rows) {
            max_above = (int)kerning_delta.y + (int)slot->bitmap.rows;
        }

    }

    return std::make_pair(max_above - min_below, min_below);
}



uint32_t mix_rgba_with_grayscale(uint32_t rgba_color, uint8_t value) {
    // This fades the color out to black as value decreases, giving the text a slightly darker border

    int r, g, b, a;
    std::tie(r, g, b, a) = split_rgba(rgba_color);

    return make_rgba((r * value) / 255, (g * value) / 255, (b * value) / 255, (a * value) / 255);
}

void osd_draw_timestamp(OSD &timestamp_osd, FT_Face &face, int baseline_offset, shared_conf &currentConfig) {
    char text[STRING_MAX_SIZE];
    time_t current_time = time(nullptr);
    strftime(text, STRING_MAX_SIZE, currentConfig.osdTimeDisplay, localtime(&current_time));

    FT_Vector pen;
    pen.x = 0;
    pen.y = timestamp_osd.getHeight() + baseline_offset;

    timestamp_osd.clear();

    FT_GlyphSlot glyph = face->glyph;
    int last_glyph_index = 0;

    for (int i = 0; text[i] != '\x00'; i++) {
        char c = text[i];

        int glyph_index = FT_Get_Char_Index(face, c);

        if (FT_Load_Glyph(face, glyph_index, FT_LOAD_DEFAULT) != 0) {
            LOG_S(INFO) << "Could not load glyph for character: " << c;
            break;
        }

        if (FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL) != 0) {
            LOG_S(INFO) << "Could not render glyph for character: " << c;
            break;
        }

        FT_Vector kerning_delta;
        kerning_delta.x = 0;
        kerning_delta.y = 0;

        if (last_glyph_index && glyph_index) {
            FT_Get_Kerning(face, last_glyph_index, glyph_index, FT_KERNING_DEFAULT, &kerning_delta);
        }

        last_glyph_index = glyph_index;

        // Draw the bitmap
        FT_Bitmap &bitmap = glyph->bitmap;

        int start_x = pen.x + glyph->bitmap_left + kerning_delta.x;
        int start_y = pen.y - glyph->bitmap_top + kerning_delta.y;

        for (int x = 0; x < (int)bitmap.width; x++) {
            for (int y = 0; y < (int)bitmap.rows; y++) {
                int shifted_x = start_x + x;
                int shifted_y = start_y + y;

                // Don't draw out of bounds
                if (shifted_x < 0 || shifted_y < 0 || shifted_x >= timestamp_osd.getWidth() || shifted_y >= timestamp_osd.getHeight()) {
                    continue;
                }

                int value = bitmap.buffer[y * bitmap.width + x];


                if (value != 0) {
                    timestamp_osd.setPixel(shifted_x, shifted_y, mix_rgba_with_grayscale(currentConfig.osdColor, value));
                } else {
                    timestamp_osd.setPixel(shifted_x, shifted_y, RGBAColor::TRANSPARENT);
                }
            }
        }

        // Move the pen
        pen.x += (glyph->advance.x / 64) + currentConfig.osdSpace;
        pen.y -= glyph->advance.y / 64;
    }

    timestamp_osd.update();
}

void osd_draw_detection_circle(OSD &motion_osd, bool isDetecting, shared_conf &currentConfig) {
    motion_osd.clear();

    if (isDetecting) {
        for (int x = 0; x < (int)DETECTION_CIRCLE_SIZE; x++) {
            for (int y = 0; y < (int)DETECTION_CIRCLE_SIZE; y++) {
                motion_osd.setPixel(x, y, mix_rgba_with_grayscale(currentConfig.motionOSD, DETECTION_CIRCLE_ALPHA[x + y * DETECTION_CIRCLE_SIZE]));
            }
        }
    }

    motion_osd.update();
}

