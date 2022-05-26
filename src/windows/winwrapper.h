// created to isolate windows api calls from raylib due to naming conflicts

// function to load packaged resources
void* LoadPackagedResource(unsigned long typeId, unsigned long resourceId, unsigned long* size);

// function to open a URL in the user's preferred browser
void OpenURLHidden(char* url);
