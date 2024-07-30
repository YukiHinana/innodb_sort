#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define PAGE_SIZE 16384

int main(int argc, char **argv) {
    printf("hello\n");
    int dryrun = 0;
    char *filename = NULL;
    if (argc > 1) {
        for (int i = 1; i < argc; i++) {
            if (strcmp(argv[i], "-n") == 0) {
                dryrun = 1;
            } else {
                filename = argv[i];
            }
        }
    }
    if (!filename) {
        printf("Usage: %s [-n] <FILENAME>", argv[0]);
        exit(1);
    }
    unsigned char *buffer = (unsigned char*)malloc(PAGE_SIZE); // 16k page
    FILE *f = fopen(filename, "r");
    FILE *output = NULL;
    unsigned char *zero_page = NULL;
    size_t cur_file_size = 0;
    if (!dryrun) {
        char *filename_sorted = malloc(strlen(filename) + 7 + 1);
        strcpy(filename_sorted, filename);
        strcat(filename_sorted, ".sorted");
        output = fopen(filename_sorted, "w");
        zero_page = (unsigned char*)calloc(1, PAGE_SIZE);
    }
    if (f) {
        while (fread(buffer, PAGE_SIZE, 1, f) == 1) {
            uint32_t page_number = (uint32_t)buffer[4] << 24 | (uint32_t)buffer[5] << 16 | (uint32_t)buffer[6] << 8 | buffer[7];
            uint32_t space_id = (uint32_t)buffer[34] << 24 | (uint32_t)buffer[35] << 16 | (uint32_t)buffer[36] << 8 | buffer[37];

            printf("offset: %u\n", page_number);

            if (!dryrun) {
                // sort by page number
                if (output) {
                    // write pagenumber - filesize zero pages
                    if (page_number > cur_file_size) {
                        if (fseek(output, 0, SEEK_END) == 0) {
                            while (page_number > cur_file_size) {
                                if (fwrite(zero_page, PAGE_SIZE, 1, output) != 1) {
                                    exit(1);
                                }
                                cur_file_size++;
                            }
                        } else {
                            printf("failed to seek\n");
                            exit(1);
                        }
                    }
                    if (fseek(output, page_number * PAGE_SIZE, SEEK_SET) == 0) {
                        if (fwrite(buffer, PAGE_SIZE, 1, output) != 1) {
                            exit(1);
                        }
                        if (page_number == cur_file_size) {
                            cur_file_size++;
                        }
                    } else {
                        printf("failed to seek\n");
                        exit(1);
                    }
                }
            }
            
        }
    } else {
        printf("can't open the file.\n");
        return 1;
    }
    if (output)
        fclose(output);
    return 0;
}