#pragma once
#include "ray/raylib.h"





// Core type
typedef struct Core {
    char name[128];
    int fighterCount;
    int* costumeCount;
    char** fighterNames;
    Image*** fighterIcons;
    Image*** fighterRenders;
} Core;

// Template type
typedef struct Template {
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
    // TODO: implement
    //return 0;
}
Core LoadCoreFromFolder(char* path) {
    // TODO: implement
    //return 0;
}
char* GetCoreName(Core* c) { return (char*)(c); }
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
    // TODO: implement
    //return 0;
}
Template LoadTemplateFromFolder(char* path) {
    // TODO: implement
    //return 0;
}
char* GetTemplateName(Template* t) { return (char*)(t); }
int GetLayerCount(Template* t) { return t->layerCount; }
Image* GetLayerRender(Template* t, int layerIdx) { return (t->layerRenders)[layerIdx]; }
void ExportTemplateToBlob(Template* t) {
    // TODO: implement
}
void FreeTemplate(Template* t) {
    // TODO: implement
}