#pragma once
#include <unistd.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include "ray/raylib.h"
#include "windows/winwrapper.h"





// Core type
typedef struct Core {
    int valid;
    char id[128];
    char name[128];
    int fighterCount;
    int* costumeCount;
    char*** fighterNames;
    Image*** fighterIcons;
    Image*** fighterRenders;
} Core;

// Template type
typedef struct Template {
    int valid;
    char id[128];
    char name[128];
    int layerCount;
    Image** layerRenders;
    Font* layerFonts;
} Template;





// Core methods
Core LoadCoreFromBlob(char* path);
Core LoadCoreFromFolder(char* path);
char* GetCoreName(Core* c);
int GetFighterCount(Core* c);
int GetCostumeCount(Core* c, int fighterIdx);
char* GetFighterName(Core* c, int fighterIdx, int costumeIdx);
Image* GetFighterIcon(Core* c, int fighterIdx, int costumeIdx);
Image* GetFighterRender(Core* c, int fighterIdx, int costumeIdx);
void ExportCoreToBlob(Core* c);
void FreeCore(Core* c);

// Template methods
Template LoadTemplateFromBlob(char* path);
Template LoadTemplateFromFolder(char* path);
char* GetTemplateName(Template* t);
int GetLayerCount(Template* t);
Image* GetLayerRender(Template* t, int layerIdx);
void ExportTemplateToBlob(Template* t);
void FreeTemplate(Template* t);





// Core methods
Core LoadCoreFromBlob(char* path) {
    printf("[CORE] Loading Blob %s\n", path);
    
    // TODO: implement
    FILE* infile = fopen(path, "r");
    
    Core out;
    out.valid = 0;
    
    fclose(infile);
    return out;
}
Core LoadCoreFromFolder(char* path) {
    // TODO: implement
    printf("[CORE] Loading Folder %s\n", path);
    // create and save blob
    // load from blob
    //return 0;
}
char* GetCoreName(Core* c) { return c->name; }
int GetFighterCount(Core* c) { return c->fighterCount; }
int GetCostumeCount(Core* c, int fighterIdx) { return (c->costumeCount)[fighterIdx]; }
char* GetFighterName(Core* c, int fighterIdx, int costumeIdx) { return (c->fighterNames)[fighterIdx][costumeIdx]; }
Image* GetFighterIcon(Core* c, int fighterIdx, int costumeIdx) { return (c->fighterIcons)[fighterIdx][costumeIdx]; }
Image* GetFighterRender(Core* c, int fighterIdx, int costumeIdx) { return (c->fighterRenders)[fighterIdx][costumeIdx]; }
void ExportCoreToBlob(Core* c) {
    // TODO: implement
}
void FreeCore(Core* c) {
    // TODO: implement
}



// Template methods
Template LoadTemplateFromBlob(char* path) {
    printf("[TPLT] Loading Blob %s\n", path);
    
    
    FILE* infile = fopen(path, "r");
    
    Template out;
    out.valid = 0;
    // TODO: implement
    
    // clean up and return the Template
    fclose(infile);
    return out;
}
Template LoadTemplateFromFolder(char* path) {
    struct stat statbuf;
    struct dirent* dp;
    printf("[TPLT] Loading Folder %s\n", path);
    
    // create the new Template to return
    Template out;
    
    // open the template directory
    DIR* mainDir = opendir(path);
    if (mainDir == NULL) {
        fprintf(stderr, "cannot open \"%s\" Template folder.\n", path);
        return out;
    }
    
    // load data
    chdir(path);
    while ((dp = readdir(mainDir)) != NULL) {
        // grab stat about the current element
        stat(dp->d_name, &statbuf);
        
        // only handle files
        if ((statbuf.st_mode & S_IFMT) == S_IFREG) {
            // load a layer image
            //TODO
            
            // load a font
            //TODO
            
            // read manifest
            if (strcmp("manifest.txt", dp->d_name) == 0) {
                printf("[TPLT][F] manifest file\n");
                //TODO
            }
            
            // read ordering
            if (strcmp("ordering.txt", dp->d_name) == 0) {
                printf("[TPLT][F] ordering file\n");
                //TODO
            }
            
            //printf("[CORE][B] %s\n", dp->d_name);
            //LoadCoreFromBlob(dp->d_name);
        }
    }
    chdir("..");
    
    // clean up and return the Template
    closedir(mainDir);
    return out;
}
char* GetTemplateName(Template* t) { return t->name; }
int GetLayerCount(Template* t) { return t->layerCount; }
Image* GetLayerRender(Template* t, int layerIdx) { return (t->layerRenders)[layerIdx]; }
void ExportTemplateToBlob(Template* t) {
    // TODO: implement
}
void FreeTemplate(Template* t) {
    // TODO: implement
}



// Resource Control
int TGLoadResources(Core* coreList, int* numCores, Template* templateList, int* numTemplates) {
    struct stat statbuf;
    struct dirent* dp;
    
    // check if there are any issues with opening the root directory
    DIR* rootDir = opendir(".");
    if (rootDir == NULL) {
        fprintf(stderr, "cannot open root directory.\n");
        return 1;
    }
    closedir(rootDir);
    
    // create and/or open core directory
    DIR* coreDir = opendir("cores");
    if (coreDir == NULL) {
        if (errno == ENOENT) {
            mkdir("cores");
            coreDir = opendir("cores");
            if (coreDir == NULL) {
                fprintf(stderr, "created but cannot open cores directory.\n");
                return 3;
            }
            printf("created \"cores\" folder.\n");
        } else {
            fprintf(stderr, "cannot open cores directory.\n");
            return 4;
        }
    }
    
    // construct list of cores to load
    chdir("cores");
    
    int* loadType = (int*)calloc(512, sizeof(int));
    char** bpaths = (char**)calloc(512, sizeof(char*));
    while ((dp = readdir(coreDir)) != NULL) {
        // grab stat about the current element
        stat(dp->d_name, &statbuf);
        
        // store the core temporarily
        Core temp;
        
        // if regular file, attempt to read it as a blob file
        if ((statbuf.st_mode & S_IFMT) == S_IFREG) {
            printf("[CORE][B] %s\n", dp->d_name);
            temp = LoadCoreFromBlob(dp->d_name);
        }
        
        // if folder, attempt to read as a folder
        else if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
            if (strcmp(".", dp->d_name) == 0) { continue; }
            if (strcmp("..", dp->d_name) == 0) { continue; }
            printf("[CORE][F] %s\n", dp->d_name);
            temp = LoadCoreFromFolder(dp->d_name);
        }
        
        // check for validity
        if (temp.valid > 0) {
            //TODO: implement
        } else {
            fprintf(stderr, "(Invalid) %s.\n", dp->d_name);
            return 5;
        }
    }
    closedir(coreDir);
    
    // load cores
    //for (int i = 0; i < )
    // TODO: finish implementing
    free(loadType);
    
    chdir("..");
    
    // create and/or open template directory
    DIR* templateDir = opendir("templates");
    if (templateDir == NULL) {
        if (errno == ENOENT) {
            mkdir("templates");
            templateDir = opendir("templates");
            if (templateDir == NULL) {
                fprintf(stderr, "created but cannot open templates directory.\n");
                return 6;
            }
            printf("created \"templates\" folder.\n");
        } else {
            fprintf(stderr, "cannot open templates directory.\n");
            return 7;
        }
    }
    
    // construct list of templates to load
    chdir("templates");
    while ((dp = readdir(templateDir)) != NULL) {
        // grab stat about the current element
        stat(dp->d_name, &statbuf);
        
        // if regular file, attempt to read it as a blob file
        if ((statbuf.st_mode & S_IFMT) == S_IFREG) {
            printf("[TPLT][B] %s\n", dp->d_name);
            LoadTemplateFromBlob(dp->d_name);
        }
        
        // if folder, attempt to read as a folder
        else if ((statbuf.st_mode & S_IFMT) == S_IFDIR) {
            if (strcmp(".", dp->d_name) == 0) { continue; }
            if (strcmp("..", dp->d_name) == 0) { continue; }
            printf("[TPLT][F] %s\n", dp->d_name);
            LoadTemplateFromFolder(dp->d_name);
        }
    }
    closedir(templateDir);
    
    // load templates
    
    chdir("..");
    
    // construct dropdown box display strings
    
    return 0;
}
void TGUnloadResources() {
    // TODO: implement
}
