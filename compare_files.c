#include "mysync.h"

// Check if a directory exists in a given path.
bool does_directory_exist(const char *path) 
{
    struct stat st;
    if (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) {
        return true;
    }
    return false;
}

// Performs the sync of a file when supplied with the file to sync, the destination path.
// also provide options to know whether to sync permissions or not.
void sync_file(FileInfo_t *file_to_sync, const char *dest_path, CONFIG *config)
{

    if (!(config->noSync)) {
        copy_file(file_to_sync->file_path, dest_path);

        if (config->preserve) {
            if (sync_permissions) {
                if (chmod(dest_path, file_to_sync->permissions) == -1) {
                    perror("chmod");
                    exit(EXIT_FAILURE);
                }
            }

            struct utimbuf new_times; 
            new_times.actime = file_to_sync->last_modified;  // Access time
            new_times.modtime = file_to_sync->last_modified; // Modification time

            if (utime(dest_path, &new_times) == -1) {
                perror("utime");
                exit(EXIT_FAILURE);
            }
        }
    }

    if (config->verbose && sync_permissions) {
        printf("Need to sync %-10s ---------> %s\n", file_to_sync->filename, dest_path);
    }
}

// Synchronizes the entries in the source directory to the destination directory.
// Recursively calls itself if -r flag is set.
void synchronize_entries(DirectoryInfo_t *source_dir, DirectoryInfo_t *dest_dir, CONFIG *config, char *path_so_far) 
{
    FileInfoNode_t *source_file_node = source_dir->files;

    // Iterate through the source directory and initialise pointers to the files.
    while (source_file_node != NULL) {
        bool found = false;
        FileInfo_t *source_file = &source_file_node->data;
        FileInfoNode_t *dest_file_node = dest_dir->files;

        // Iterate through the destination directory and check if relative paths match.
        while (dest_file_node != NULL) {
            FileInfo_t *dst_file = &dest_file_node->data;

            if (strcmp(source_file->rel_path, dst_file->rel_path) == 0) {
                found = true;
                break;
            }

            dest_file_node = dest_file_node->next;
        }

        // File has not been found so we need to copy it over.
        if (!found) {
            char dest_path[PATH_MAX];
            snprintf(dest_path, PATH_MAX, "%s/%s", path_so_far, source_file->filename);
            sync_file(source_file, dest_path, config);
        }

        // File has been found so we need to check the latest modification time.
        else { 
            if (source_file->last_modified > dest_file_node->data.last_modified) {
                sync_file(source_file, dest_file_node->data.file_path, config);
            }
            else {
                sync_file(&dest_file_node->data, source_file->file_path, config);
            }
        }

        // Move on to the next file in the source directory.
        source_file_node = source_file_node->next;
    }

    if (config->recursive) {
        DirInfoNode_t *source_file_node = source_dir->subdir;

        // Iterate through the source directory and initialise pointers to the subdirectories.
        while (source_file_node != NULL) {
            bool subdir_found = false;
            DirectoryInfo_t *source_subdir = &source_file_node->dir_data;
            DirInfoNode_t *dest_subdir_node = dest_dir->subdir;

            // Iterate through the destination directory and check if relative paths match.
            while (dest_subdir_node != NULL) {
                DirectoryInfo_t *dst_subdir = &dest_subdir_node->dir_data;

                if (strcmp(source_subdir->dir_name, dst_subdir->dir_name) == 0) {

                    // Construct updated path for recursive calls.
                    char new_path_so_far[PATH_MAX];
                    snprintf(new_path_so_far, PATH_MAX, "%s/%s", path_so_far, source_subdir->dir_name);
                    
                    // Recursively call synchronize_entries on the subdirectories.
                    synchronize_entries(source_subdir, dst_subdir, config, new_path_so_far);
                    subdir_found = true;
                    break;
                }

                dest_subdir_node = dest_subdir_node->next;
            }

            // Subdirectory has not been found so we need to create it.
            if (!subdir_found) {
                char dest_path_dir[PATH_MAX];
                snprintf(dest_path_dir, PATH_MAX, "%s/%s", path_so_far, source_subdir->dir_name);

                if (!does_directory_exist(dest_path_dir)) {
                    if (mkdir(dest_path_dir, 0777) == -1) {
                        perror("mkdir");
                        exit(EXIT_FAILURE);
                    }
                }
                DirectoryInfo_t new_dest_subdir;
                new_dest_subdir.dir_name = dest_path_dir;
                new_dest_subdir.files = NULL;
                new_dest_subdir.subdir = NULL;

                // Using the constructed directory path for the next recursive call.
                synchronize_entries(source_subdir, &new_dest_subdir, config, dest_path_dir);
            }

            // Advance to the next subdirectory in the source directory.
            source_file_node = source_file_node->next;
        }
    }
}

