#include "mysync.h"

// Takes a path to a directory and returns pointer to populated DirectoryInfo_t struct
// Recursively calls itself to populate the subdirectory list if -r flag is set
DirectoryInfo_t *file_status_info(const char *directory_path, char *original_path, CONFIG *config, PATTERNS *patterns) 
{
    DIR *dir = opendir(directory_path);
    if (dir == NULL) {
        perror("Error opening directory\n");
        exit(EXIT_FAILURE);
    }

    DirectoryInfo_t *directory = malloc(sizeof(DirectoryInfo_t));
    if (!directory) {
        perror("Error allocating memory for directory");
        closedir(dir);
        exit(EXIT_FAILURE);
    }

    directory->dir_name = strdup(directory_path);
    directory->files = NULL;
    directory->subdir = NULL;

    struct dirent *entry;

    // Parse entries and populate the struct
    while ((entry = readdir(dir)) != NULL) {
        char path[PATH_MAX];
        snprintf(path, sizeof(path), "%s/%s", directory_path, entry->d_name);
        struct stat file_stat;

        if (stat(path, &file_stat) == -1) {
            perror("Failed to get file status information");
            exit(EXIT_FAILURE);
        }

        // Skip over '.' and '..' directories
        if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0) {
            continue;
        }

        // Skip over files that don't match the only pattern.
        if (config->only) {
            bool valid_file = false;
            for (int i = 0; i < patterns->num_opatterns; i++) {
                regex_t compiled_regex;
                char *regex_pattern = glob2regex(patterns->opatterns[i]);

                if (!regex_pattern) {
                    perror("glob2regex failed");
                    exit(EXIT_FAILURE);
                }

                if (regcomp(&compiled_regex, regex_pattern, 0) != 0) {
                    free(regex_pattern);
                    perror("regex compilation");
                    exit(EXIT_FAILURE);
                }

                int match = regexec(&compiled_regex, entry->d_name, 0, NULL, 0);
                regfree(&compiled_regex);
                free(regex_pattern);  

                if (!match)
                    valid_file = true;
            }
            if (!valid_file)
                continue;
        }
        
        // Skip over files that match the ignore pattern.
        if (config->ignore) {
            bool ignore_file = false;
            for(int i = 0; i < patterns->num_ipatterns; i++) {
                regex_t compiled_regex;
                char *regex_pattern = glob2regex(patterns->ipatterns[i]);

                if (!regex_pattern) {
                    perror("glob2regex failed");
                    exit(EXIT_FAILURE);
                }

                // Compile the regex
                if (regcomp(&compiled_regex, regex_pattern, 0) != 0) {
                    free(regex_pattern);  // Free memory allocated by glob2regex
                    perror("regex compilation");
                    exit(EXIT_FAILURE);
                }

                // Execute the regex to see if it matches the file name
                int match = regexec(&compiled_regex, entry->d_name, 0, NULL, 0);
                regfree(&compiled_regex);
                free(regex_pattern);  // Free memory allocated by glob2regex

                if (!match)
                    ignore_file = true;
            }
            if (ignore_file)
                continue;
        }

        // Skip hidden files if -a flag not set
        if (!(config->allFiles) && entry->d_name[0] == '.')
            continue;

        // If the entry is a directory, recursively call file_status_info if -r flag is set
        if (entry->d_type == DT_DIR && config->recursive) {
            DirectoryInfo_t *subdirectory = file_status_info(path, original_path, config, patterns);
            if (subdirectory) {
                DirInfoNode_t *newSubdirNode = malloc(sizeof(DirInfoNode_t));
                if (!newSubdirNode) {
                    perror("Error allocating memory for subdir node");
                    exit(EXIT_FAILURE);
                }
                newSubdirNode->dir_data = *subdirectory;
                newSubdirNode->next = directory->subdir;
                directory->subdir = newSubdirNode;
                directory->subdir->dir_data.dir_name = strdup(entry->d_name);

                free(subdirectory); // can free since data has been copied into list
            }
        } 
        
        // If the entry is a regular file, add it to the list of files in the directory
        else if (entry->d_type == DT_REG) {
            FileInfo_t *file = malloc(sizeof(FileInfo_t));
            if (!file) {
                perror("Error allocating memory for file");
                exit(EXIT_FAILURE);
            }

            file->filename = strdup(entry->d_name);
            file->file_path = strdup(path);
            file->rel_path = strdup(path + strlen(original_path));
            file->last_modified = file_stat.st_mtime;
            file->permissions = file_stat.st_mode & 0777;

            FileInfoNode_t *newFileNode = malloc(sizeof(FileInfoNode_t));
            if (!newFileNode) {
                perror("Error allocating memory for file node");
                exit(EXIT_FAILURE);
            }
            newFileNode->data = *file;
            newFileNode->next = directory->files;
            directory->files = newFileNode;
            
            if (config->verbose) {
                printf("------FILE INFO------\n");
                printf("File: %s\n", file->filename);
                printf("Last modified: %ld\n", file->last_modified);
                printf("Permissions: %o\n", file->permissions);
                printf("----------------------\n\n");
            }

            free(file); // can free since data has been copied into list
        }
    }

    closedir(dir);
    return directory;
}