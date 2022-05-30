#include "ray/raylib.h"
#include "loader.h"




// composite together a thumbnail using the provided struct
void CompositeThumbnail(/* ThumbnailSpec* spec */);

// run through 1 or more batch files dropped into the window and generate
// thumbnails for each one
void CompositeBatch(char** files, int fileCount);





/*
// full description of an individual thumbnail
typedef struct ThumbnailSpec {
    // format info
    int coreId;
    int templateId;
    // match info
    char[128] tournamentTitle;
    int tournamentNumber;
    char[128] matchTitle;
    bool isDoubles;
    // p1 info
    char[128] p1name;
    int p1fighter;
    int p1costume;
    // p2 info
    char[128] p2name;
    int p2fighter;
    int p2costume;
} ThumbnailSpec;
*/





// composite together a thumbnail using the provided struct
void CompositeThumbnail(RenderTexture2D* preview/* , ThumbnailSpec* spec */)
{
    // use a render texture to composite together the thumbnail
    RenderTexture buf = LoadRenderTexture(1280, 720);
    
    // compositing
    BeginTextureMode(buf);
        // TODO: take method argument struct and display components here
        DrawCircle(320, 320, 100.0, RED);
    EndTextureMode();
    
    // write the composite to an Image
    Image generatedThumbnail = LoadImageFromTexture(buf.texture);
    // save a scaled-down version of the Image to be used in the preview section
    Image temp1 = ImageCopy(generatedThumbnail);
    ImageResize(&temp1, 360, 202);
    Texture2D temp2 = LoadTextureFromImage(temp1);
    BeginTextureMode(*preview);
        DrawTexture(temp2, 0, 0, WHITE);
    EndTextureMode();
    
    // unload the buffer
    UnloadRenderTexture(buf);
    
    // export the thumbnail
    _mkdir("./output");
    //char path[512] = { 0 };
    //strcpy(path, TextFormat("%s/%s", CORE.Storage.basePath, fileName));
    ExportImage(generatedThumbnail, "./output/test.png");
}

// run through 1 or more batch files dropped into the window and generate
// thumbnails for each one
void CompositeBatch(char** files, int fileCount)
{
    // loop through each 
    for (int i = 0; i < fileCount; ++i) {
        // TODO: implement
        // parse batch file in rows
        //while () {
            //fill out a ThumbnailSpec struct for this line
            //stub
            // call CompositeThumbnail to generate and export a thumbnail with
            // the provided ThumbnailSpec object
            //stub
            // export the thumbnail with a name determined by the batch file
            //char path[512] = { 0 };
            //strcpy(path, TextFormat("%s/%s", CORE.Storage.basePath, fileName));
            //stub
        //}
        files[i];
    }
}