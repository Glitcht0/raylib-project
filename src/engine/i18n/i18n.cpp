#include "raylib.h"
#include <string.h>
#include <stdlib.h>
#include <cstdio>


#define MAX_TEXTS 128

typedef struct {
    char key[64];
    char value[256];
} Entry;

static Entry entries[MAX_TEXTS];
static int entryCount = 0;



void I18N_Load(const char* filename) {
    char* file = LoadFileText(filename);
    entryCount = 0;

    char* line = strtok(file, "\n");
    while (line && entryCount < MAX_TEXTS) {
        char key[64], value[256];
        if (sscanf(line, " \"%63[^\"]\" : \"%255[^\"]\"", key, value) == 2) {
            strcpy(entries[entryCount].key, key);
            strcpy(entries[entryCount].value, value);
            entryCount++;
        }
        line = strtok(NULL, "\n");
    }

    UnloadFileText(file);
}



const char* T(const char* key) {
    for (int i = 0; i < entryCount; i++) {
        if (strcmp(entries[i].key, key) == 0)
            return entries[i].value;
    }
    return key;
}


void I18N_Unload(void) {
    entryCount = 0;
}
