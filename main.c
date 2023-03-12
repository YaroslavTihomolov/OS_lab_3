#include <dirent.h>
#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <sys/stat.h>

void swap(char *a, char *b) {
    char c = *a;
    *a = *b;
    *b = c;
}

char *revName(char *name) {
    unsigned int len = strlen(name);
    char *revName = malloc(sizeof(name));
    strcpy(revName, name);
    for (int i = 0; i < len / 2; i++) {
        swap(&revName[i], &revName[len - i - 1]);
        //printf("%c %c\n", revName[i], revName[len - i]);
    }
    return revName;
}

int getSeparatorPos(const char *array, char separator) {
    int index = 0;
    int pos = 0;
    while (array[index] != 0) {
        if (array[index] == separator) {
            pos = index;
        }
        index++;
    }
    return pos;
}

DIR *upInDirectory(const char *path, char *catalogName, char *pathToCatalog) {
    int pos = getSeparatorPos(path, '/');
    char *dest = malloc(sizeof(path));
    memcpy(dest, path, pos);
    if (memcpy(catalogName, path + pos + 1, strlen(path) - pos - 1) == 0) {
        return NULL;
    }

    if (memcpy(pathToCatalog, path, pos) == 0) {
        return NULL;
    }

    DIR *dir = opendir(dest);
    free(dest);
    return dir;
}

void createReverseCatalog(char *path) {
    mkdir(path, 0777);
}

int copyFiles(DIR *dir, char *path) {
    struct dirent *entry;
    char *tmpPath = malloc(sizeof(path));
    if (memcpy(tmpPath, path, strlen(path)) == 0) {
        return -1;
    }
    strcat(tmpPath, "/");
    int pointPos;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_type != 8) {
            continue;
        }
        char *fileCurName = malloc(sizeof(path));
        if (memcpy(fileCurName, tmpPath, strlen(tmpPath)) == 0) {
            return -1;
        }

        pointPos = getSeparatorPos(entry->d_name, '.');
        char *fileNameWithoutFormat = malloc(pointPos);
        char *fileFormat = malloc(sizeof(fileCurName));
        if (memcpy(fileNameWithoutFormat, entry->d_name, pointPos) == 0) {
            return -1;
        }
        if (memcpy(fileFormat, entry->d_name + pointPos, strlen(entry->d_name) - pointPos) == 0) {
            return -1;
        }

        strcat(fileCurName, revName(fileNameWithoutFormat));
        strcat(fileCurName, fileFormat);
        FILE *f = fopen(fileCurName, "w");
    }
    free(tmpPath);
    return 0;
}

int main(__attribute__((unused)) int argc, char **argv) {
    DIR *dir;

    dir = opendir(argv[1]);
    if (dir == NULL) {
        perror("opendir");
        return 1;
    }

    char *catalogName = malloc(sizeof(argv[1]));
    char *path = malloc(sizeof(argv[1]));

    DIR *a = upInDirectory(argv[1], catalogName, path);

    char *reverseName;
    reverseName = revName(catalogName);
    char *pathToNewFile = malloc(sizeof(path));
    if (memcpy(pathToNewFile, path, strlen(path)) == 0) {
        return -1;
    }
    strcat(pathToNewFile, "/");
    strcat(pathToNewFile, reverseName);

    createReverseCatalog(pathToNewFile);

    copyFiles(dir, pathToNewFile);

    closedir(dir);

    free(catalogName);
    free(path);
    free(pathToNewFile);
    return 0;
}
