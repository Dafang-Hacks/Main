#include<stdio.h>
#include <cstdlib>


#include "sharedmem.h"

int main(int argc, char *argv[]) {

    SharedMem& mem = SharedMem::instance();
    void* memory = mem.getImage();
    int memlen = mem.getImageSize();

    fwrite(memory, memlen, 1, stdout);
    free(memory);

    return 0;
}
