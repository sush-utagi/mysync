#include "mysync.h"

// Takes and source and destination path and copies the file from source to destination.
bool copy_file(const char *source_path, const char *dest_path) 
{
    FILE *source, *dest;
    int character; // Use int for EOF detection

    source = fopen(source_path, "rb");
    if (source == NULL) {
        perror("Error opening source file");
        return false;
    }

    dest = fopen(dest_path, "wb");
    if (dest == NULL) {
        printf("Error opening destination file: %s\n", dest_path);
        fclose(source); // Close the source file on failure
        return false;
    }

    while ((character = fgetc(source)) != EOF) {
        if (fputc(character, dest) == EOF) {
            perror("Error writing to destination file");
            fclose(source);
            fclose(dest);
            return false;
        }
    }

    fclose(source);
    fclose(dest);

    return true;
}
