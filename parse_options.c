#include "mysync.h"

// get rid of the directory assigning and just do the options in this function.
int parse_options(int argc, char *argv[], CONFIG *config, PATTERNS *patterns)
{
    int opt;
    while ((opt = getopt(argc, argv, COMMAND_LINE_OPTIONS)) != -1) {
        switch (opt) {
            case 'a':
                config->allFiles = true;
                break;

            case 'i':
                // if the -i pattern starts with a single quote, then it is a valid pattern
                if (optarg) {
                    config->ignore = true;
                    patterns->ipatterns = (char **) realloc(patterns->ipatterns, sizeof(char *) * (patterns->num_ipatterns + 1));
                    if (patterns->ipatterns == NULL) {
                        printf("Error allocating memory for ipatterns\n");
                        exit(EXIT_FAILURE);
                    }
                    patterns->ipatterns[patterns->num_ipatterns] = strdup(optarg);
                    patterns->num_ipatterns++;

                    // remember to free the memiry
                }
                else {
                    printf("Expected pattern after -i\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'n':
                config->noSync = true;
                config->verbose = true;
                break;

            case 'o':
                // if the -o pattern starts with a single quote, then it is a valid pattern
                if (optarg) {
                    config->only = true;
                    patterns->opatterns = (char **) realloc(patterns->opatterns, sizeof(char *) * (patterns->num_opatterns + 1));
                    if (patterns->opatterns == NULL) {
                        printf("Error allocating memory for opatterns\n");
                        exit(EXIT_FAILURE);
                    }
                    patterns->opatterns[patterns->num_opatterns] = strdup(optarg);
                    patterns->num_opatterns++;
                }
                else {
                    printf("Expected pattern after -o\n");
                    exit(EXIT_FAILURE);
                }
                break;

            case 'p':
                config->preserve = true;
                break;

            case 'r':
                config->recursive = true;
                break;

            case 'v':
                config->verbose = true;
                break;

            case '?':
                if (optopt == 'i' || optopt == 'o') {
                    fprintf(stderr, "Option -%c requires an pattern.\n", optopt);
                } else if (isprint(optopt)) {
                    fprintf(stderr, "Unknown option `-%c'.\n", optopt);
                } else {
                    fprintf(stderr, "Unknown option character `\\x%x'.\n", optopt);
                }
                exit(EXIT_FAILURE);
        }
    }
    return optind;
}