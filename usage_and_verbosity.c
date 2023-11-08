#include "mysync.h"

// Print options that have been set
void display_options (CONFIG *config)
{
    printf("------OPTIONS------\n"
            "[-a] allFiles: %d\n"
            "[-i] ignorePattern: %d\n"
            "[-n] noSync: %d\n"
            "[-o] onlyPattern: %d\n"
            "[-p] preserve: %d\n"
            "[-r] recursive: %d\n"
            "[-v] verbose: %d\n"
            "-------------------\n",\
            config->allFiles, config->ignore, config->noSync,
            config->only, config->preserve, config->recursive, config->verbose);
}

// Print usage information and exit
void usage (void)
{
    printf("./mysync  [options]  directory1  directory2  [directory3  ... ]\n");
    exit(EXIT_FAILURE);
}

// Print the directory tree thats been built
void print_directory_info(DirectoryInfo_t *directory, int depth) 
{
    if (directory == NULL) {
        return;
    }

    // Print the current directory's information
    printf("%*sDirectory: %s\n", depth * 4, "", directory->dir_name);

    // Print information about files in the current directory
    FileInfoNode_t *file = directory->files;
    while (file != NULL) {
        printf("%*sFile: %s\n", (depth + 1) * 4, "", file->data.filename);
        printf("%*sFile path: %s\n", (depth + 2) * 4, "", file->data.file_path);
        printf("%*sLast modified: %ld\n", (depth + 2) * 4, "", file->data.last_modified);
        printf("%*sPermissions: %o\n", (depth + 2) * 4, "", file->data.permissions);
        
        file = file->next;
    }

    // Recursively print information about subdirectories
    DirInfoNode_t *subdirNode = directory->subdir;
    while (subdirNode != NULL) {
        print_directory_info(&subdirNode->dir_data, depth + 1);
        subdirNode = subdirNode->next;
    }
}

