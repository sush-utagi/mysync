#include "mysync.h"

int main(int argc, char *argv[])
{
    if (argc <= 2)
        usage();

    // Initialize to all zeros (all flags off)
    CONFIG config = {0};
    PATTERNS patterns = {NULL, 0, NULL, 0};

    // Parse options and initialize directories
    int dir_index = parse_options(argc, argv, &config, &patterns);
    int num_directories = argc - dir_index;

    // Allocate enough memory to structures using the number of directories given.
    DirectoryInfo_t **directories = malloc(sizeof(DirectoryInfo_t *) * (num_directories));
    if (directories == NULL) {
        printf("Error allocating memory for directories\n");
        exit(EXIT_FAILURE);
    }

    // iterate over the directories, extract file info and store away in structs
    for (int i = dir_index; i < argc; i++)
        directories[i - dir_index] = file_status_info(argv[i], argv[i], &config, &patterns);

    // Print Directory tree and options if verbose output enabled
    if (config.verbose) {
        display_options(&config);

        for (int i = 0; i < num_directories; i++) {
            printf("Directory Structure %d:\n", i + 1);
            print_directory_info(directories[i], 0);
        }
    }

    // Compare and eventually sync directories provided in the command line.
    for (int i = 0; i < num_directories; i++) {
        for (int j = i + 1; j < num_directories; j++) {  
            synchronize_entries(directories[i], directories[j], &config, directories[j]->dir_name);
        }
    }

    sync_permissions = false; // set permissions to false to avoid syncing permissions twice
    for (int i = 0; i < num_directories; i++) {
        for (int j = i + 1; j < num_directories; j++) {
            synchronize_entries(directories[j], directories[i], &config, directories[i]->dir_name);
        }
    }
    
    return EXIT_SUCCESS;
}
