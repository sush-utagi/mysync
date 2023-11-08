/*
  CITS2002 Project 2 2023
  Student1:   22502356   Susheel Utagi
*/


#define _DEFAULT_SOURCE                      // Allow DT_DIR and DT_REG to be defined
#define COMMAND_LINE_OPTIONS    "ai:o:nprv"  // options for getopt()

#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <stdbool.h>
#include <unistd.h> 
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <limits.h>
#include <sys/types.h>
#include <regex.h>
#include <utime.h>

#define _POSIX_C_SOURCE         200809L      // for strdup()
#include <string.h>


// GLOBAL VARIABLES ---------------------------------------------------------------------------------------------------------------------------
extern bool sync_permissions;   // global variable to determine whether to sync permissions or not
                                // defined to prevent syncing permisssions over an updated file

// GLOBAL STRUCTURES --------------------------------------------------------------------------------------------------------------------------

struct FileInfoNode;            // forward declarations to allow for recursive structures
struct DirInfoNode;

typedef struct {
    char    *filename;          // name of the file
    char    *file_path;         // path to the file
    char    *rel_path;          // relative path to the file
    time_t  last_modified;      // last modified time in UNIX time  
    mode_t  permissions;
} FileInfo_t;

typedef struct FileInfoNode {
    FileInfo_t data;
    struct FileInfoNode *next;  // Now you can use this
} FileInfoNode_t;

typedef struct {
    char                *dir_name;
    FileInfoNode_t      *files; // linked list of files in the directory
    struct DirInfoNode  *subdir;
} DirectoryInfo_t;

typedef struct DirInfoNode {
    DirectoryInfo_t dir_data;
    struct DirInfoNode *next;
} DirInfoNode_t;

typedef struct {
    bool    allFiles;        // -a 
    bool    ignore;          // -i
    bool    only;            // -o
    bool    noSync;          // -n
    bool    preserve;        // -p
    bool    recursive;       // -r
    bool    verbose;         // -v
} CONFIG;

typedef struct {
    char    **ipatterns;
    int     num_ipatterns;

    char    **opatterns;
    int     num_opatterns;
} PATTERNS;

// FUNCTION PROTOTYPES ----------------------------------------------------------------------------------------------------------------------
extern void usage(void);
extern int parse_options(int argc, char *argv[], CONFIG *config, PATTERNS *patterns);
extern DirectoryInfo_t *file_status_info(const char *directory_path, char *original_path, CONFIG *config, PATTERNS *patterns);
extern void display_options(CONFIG *config);
extern bool does_directory_exist(const char *path);
extern bool copy_file(const char *source_path, const char *dest_path);
extern void print_directory_info(DirectoryInfo_t *directory, int depth);
extern char *glob2regex(char *glob);
extern void sync_file(FileInfo_t *file_to_sync, const char *destination_directory, CONFIG *config);
extern void synchronize_entries(DirectoryInfo_t *src_dir, DirectoryInfo_t *dst_dir, CONFIG *config, char *path_so_far);
