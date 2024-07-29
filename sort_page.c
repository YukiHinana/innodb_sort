#include "stdio.h"
#include "stdlib.h"
#include "string.h"

#define PAGE_SIZE 16384

int main(int argc, char **argv) {
    printf("hello\n");
    unsigned char *buffer = (unsigned char*)malloc(PAGE_SIZE); // 16k page
    FILE *f = fopen("00000594.pages", "r");
    if (f) {
        while (fread(buffer, PAGE_SIZE, 1, f) == 1) {
            // char* buffer2 = (char*)malloc(4);
            // buffer2[0] = buffer[4];
            // memcpy(buffer2, &buffer[4], 4);
            uint32_t offset = (uint32_t)buffer[4] << 24 | (uint32_t)buffer[5] << 16 | (uint32_t)buffer[6] << 8 | buffer[7];
            uint32_t space_id = (uint32_t)buffer[34] << 24 | (uint32_t)buffer[35] << 16 | (uint32_t)buffer[36] << 8 | buffer[37];
            printf("offset: %u\n", offset);
            printf("space_id: %u\n", space_id);
            // sort by page number
        }
    } else {
        printf("can't open the file.\n");
        return 1;
    }
    return 0;
}