#ifndef BASEFONT_H
#define BASEFONT_H

typedef struct bitmapinfo {
    uint32_t width;
    uint32_t *pdata;
} bitmapinfo_t;

class BaseFont {
    public:
        bitmapinfo_t *character_map;

        char start_char;
        char end_char;
        int height;

        BaseFont(bitmapinfo_t *character_map_, char start_char_, char end_char_, int height_):
            character_map(character_map_),
            start_char(start_char_),
            end_char(end_char_),
            height(height_) {}


    public:
        bool isSupported(char c) const {
            return (start_char <= c) && (c <= end_char);
        }

        int getWidth(char c) const {
            return character_map[c - start_char].width;
        }

        int getHeight(char c) const {
            return height;
        }

        bool getPixel(char c, int x, int y) {
            return (character_map[c - start_char].pdata[x + y * getWidth(c)] == 0xFFFFFFFF);
        }
};

#endif