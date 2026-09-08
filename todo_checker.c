#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>


bool validate_args(int argc) {
    if (argc!=2) {
        printf("Invalid argument size");
        return false;
    }
    return true;
}



bool validate_cfile(const char *fname, size_t flength) {
     if (flength >= 2 && fname[flength-2] == '.' && fname[flength-1] == 'c') {
        printf("\n%s\n", fname);
        return true;
     }
     return false;
}

int main(int argc, char *argv[]) {
    if(!validate_args(argc)) return EXIT_FAILURE;

    char *folder_name = argv[1];
    DIR *folder = opendir(argv[1]);

    if (folder == NULL) {
        fprintf(stderr,"Unable to read directory %s\n", folder_name);
        return EXIT_FAILURE;
    }

    struct dirent *entry;
    while((entry = readdir(folder)) != NULL) {
        char *file_name = entry->d_name;

        if (validate_cfile(file_name, strlen(file_name))) {
            FILE* fp;
            char* line = NULL;
            size_t len = 0;
            ssize_t read;

            char path[1024];
            snprintf(path, sizeof(path), "%s/%s", folder_name, file_name);

            fp = fopen(path, "r");

            if (fp == NULL) {
                printf("file not found. %s\n",path);
                return false;
            }

            int line_number = 1;
            while ( (read = getline(&line, &len, fp)) != -1) {
                char * todo_comment = strstr(line,"//TODO");
                if(todo_comment != NULL) {
                    printf("\t%d: %s\n", line_number,todo_comment + strlen("//TODO"));
                }
                line_number++;
            }

            fclose(fp);
            free(line);
       }
    }
    closedir(folder);

    return EXIT_SUCCESS;
}
