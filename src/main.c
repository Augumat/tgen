// includes
#include "ray/raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_CUSTOM_ICONS
#include "icons.h"
#include "ray/raygui.h"
#include "cyber.h"
#include "windows/resource.h"
#include "windows/winwrapper.h"
//#include "augui.h"
#include "generator.h"





// GUI static definitions
//--------------------------------------------------------------------------------------
// different views of the editor
#define VIEW_EDITOR 0
#define VIEW_PREVIEW 1
#define VIEW_BATCH 2
// the program window
static const Rectangle windowDims = (Rectangle){ 0, 0, 636, 399 };
static const Rectangle windowTitleRect = (Rectangle){ 3, 0, 512, 24 };
// editor view
// - topbar
static const Rectangle githubRect = (Rectangle){ 555, 3, 18, 18 };
static const Rectangle helpRect = (Rectangle){ 575, 3, 18, 18 };
static const Rectangle bugsRect = (Rectangle){ 595, 3, 18, 18 };
// - format settings zone
static const Rectangle formatZoneRect = (Rectangle){ 5, 36, 336, 40 };
static const Rectangle coreAssetsRect = (Rectangle){ 13, 44, 144, 24 };
static const Rectangle templateAssetsRect = (Rectangle){ 161, 44, 144, 24 };
static const Rectangle reloadAssetsRect = (Rectangle){ 309, 44, 24, 24 };
// - program settings zone
static const Rectangle settingsZoneRect = (Rectangle){ 349, 36, 282, 40 };
// - match info zone
static const Rectangle matchZoneRect = (Rectangle){ 5, 88, 626, 74 };
static const Rectangle matchZoneVsRect = (Rectangle){ 204, 112, 16, 24 };
static const Rectangle matchZoneInRect = (Rectangle){ 417, 112, 16, 24 };
// - - player 1
static const Rectangle p1nameRect = (Rectangle){ 13, 96, 186, 32 };
static const Rectangle p1fighterRect = (Rectangle){ 13, 130, 160, 24 };
static const Rectangle p1costumeRect = (Rectangle){ 175, 130, 24, 24 };
// - - player 2
static const Rectangle p2nameRect = (Rectangle){ 226, 96, 186, 32 };
static const Rectangle p2fighterRect = (Rectangle){ 226, 130, 160, 24 };
static const Rectangle p2costumeRect = (Rectangle){ 388, 130, 24, 24 };
// - - tournament and match designation
static const Rectangle tournamentNameRect = (Rectangle){ 437, 96, 160, 24 };
static const Rectangle tournamentNumberRect = (Rectangle){ 599, 96, 24, 24 };
static const Rectangle matchDesignationRect = (Rectangle){ 437, 122, 186, 32 };
// - actions zone
static const Rectangle actionZoneRect = (Rectangle){ 391, 174, 240, 64 };
static const Rectangle resetRect = (Rectangle){ 399, 182, 110, 48 };
static const Rectangle generateRect = (Rectangle){ 513, 182, 110, 48 };
// - preview zone
static const Rectangle previewZoneRect = (Rectangle){ 5, 174, 378, 220 };
static const Rectangle previewPanelRect = (Rectangle){ 13, 182, 362, 204 };
static const Rectangle previewImageRect = (Rectangle){ 14, 183, 360, 202 };
// - batch zone
static const Rectangle batchZoneRect = (Rectangle){ 391, 250, 240, 144 };
static const Rectangle batchPanelRect = (Rectangle){ 399, 258, 224, 128 };
//--------------------------------------------------------------------------------------

// GUI state vars
//--------------------------------------------------------------------------------------
// editor view
// - topbar
static bool windowIsOpen = true;
static char* windowTitleText = "Aug's Thumbnail Generator";
// - - topbar buttons
static bool githubPressed = false;
static bool helpPressed = false;
static bool bugsPressed = false;
// - format settings zone
// - - core asset selector
static bool coreAssetsEditMode = false;
static int coreAssetsActive = 0;
static char* coreAssetsList = " core1; core2";//TODO: dynamic
// - - template asset selector
static bool templateAssetsEditMode = false;
static int templateAssetsActive = 0;
static char* templateAssetsList = " template1; template2; template3";//TODO: dynamic
// - - reload formats button
static bool reloadAssetsPressed = false;
// - player zone
// - - player 1 name input
static bool p1nameEditMode = false;
static char p1name[128] = "Player 1";
// - - player 1 fighter selector
static bool p1fighterEditMode = false;
static int p1fighterActive = 0;
static char* p1fighterDisplayList = "; fighter1; fighter2; fighter3";//TODO: dynamic
// - - player 1 costume selector
static bool p1costumeEditMode = false;
static int p1costumeActive = 0;
static char* p1costumeDisplayList = "";
// - - player 2 name input
static bool p2nameEditMode = false;
static char p2name[128] = "Player 2";
// - - player 2 fighter selector
static bool p2fighterEditMode = false;
static int p2fighterActive = 0;
static char* p2fighterDisplayList = "; fighter1; fighter2; fighter3";//TODO: dynamic
// - - player 2 costume selector
static bool p2costumeEditMode = false;
static int p2costumeActive = 0;
static char* p2costumeDisplayList = "";
// - - tournament name box
static bool tournamentNameEditMode = false;
static char tournamentNameText[128] = "Tournament Number";
// - - tournament number input
static bool tournamentNumberEditMode = false;
static int tournamentNumberValue = 0;
// - - match designation input
static bool matchDesignationEditMode = false;
static char matchDesignationText[128] = "Match Title";
// - actions zone
static bool resetPressed = false;
static bool generatePressed = false;
// - preview zone
//static Image generatedThumbnail = { 0 };
static RenderTexture2D previewRenderTexture = { 0 };
// - batch zone
static char* batchPanelText = "Drag batch files here";
//--------------------------------------------------------------------------------------

// format state vars
//--------------------------------------------------------------------------------------

//--------------------------------------------------------------------------------------





// load an embedded PNG resource (windows only)
Image LoadImageFromResource(unsigned long resourceId)
{
    unsigned long tempSize = 0;
    void* tempDecoded = LoadPackagedResource(PNG_RESOURCE, resourceId, &tempSize);
    return LoadImageFromMemory(".png", (unsigned char*)(tempDecoded), (int)(tempSize));
}

// resets the input fields 
void ResetInputFields()
{
    // TODO: implement
    return;
}

void DrawEditorView()
{
    // lock the GUI if any boxes are being edited
    if (coreAssetsEditMode || templateAssetsEditMode
     || p1fighterEditMode || p2fighterEditMode
     || p1costumeEditMode || p2costumeEditMode
    ) GuiLock();
    
    // draw the window itself
    if (windowIsOpen) windowIsOpen = !GuiWindowBox(windowDims, NULL);
    if (GuiButton(githubRect, GuiIconText(RAYGUI_ICON_GITHUB_SMALL, NULL))) githubPressed = !githubPressed;
    if (GuiButton(helpRect, GuiIconText(RAYGUI_ICON_HELP_SMALL, NULL))) helpPressed = !helpPressed;
    if (GuiButton(bugsRect, GuiIconText(RAYGUI_ICON_BUGS, NULL))) bugsPressed = !bugsPressed;
    
    // draw static elements
    GuiGroupBox(formatZoneRect, "Format Selection");
    GuiGroupBox(settingsZoneRect, "Settings");
    GuiGroupBox(matchZoneRect, "Match Information");
    GuiGroupBox(previewZoneRect, "Live Preview");
    GuiGroupBox(actionZoneRect, "Actions");
    GuiGroupBox(batchZoneRect, "Batch Generation");
    GuiPanel(previewPanelRect);
    GuiPanel(batchPanelRect);
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    GuiLabel(matchZoneVsRect, "VS");
    GuiLabel(matchZoneInRect, "IN");
    GuiLabel(batchPanelRect, GuiIconText(RAYGUI_ICON_FILE_OPEN, batchPanelText));
    GuiSetStyle(LABEL, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    GuiLabel(windowTitleRect, GuiIconText(RAYGUI_ICON_TGEN, windowTitleText));
    
    // draw thumbnail preview
    DrawTexture(previewRenderTexture.texture, previewImageRect.x, previewImageRect.y, WHITE);
    
    // draw action buttons
    if (GuiButton(resetRect, GuiIconText(RAYGUI_ICON_CLEAR, "Clear Fields"))) resetPressed = !resetPressed;
    if (GuiButton(generateRect, GuiIconText(RAYGUI_ICON_PLAYER_PLAY, "Generate"))) generatePressed = !generatePressed;
    
    // draw match information boxes
    if (GuiTextBox(p1nameRect, p1name, 128, p1nameEditMode)) p1nameEditMode = !p1nameEditMode;
    if (GuiTextBox(p2nameRect, p2name, 128, p2nameEditMode)) p2nameEditMode = !p2nameEditMode;
    if (GuiTextBox(tournamentNameRect, tournamentNameText, 128, tournamentNameEditMode)) tournamentNameEditMode = !tournamentNameEditMode;
    if (GuiValueBox(tournamentNumberRect, "", &tournamentNumberValue, 0, 999, tournamentNumberEditMode)) tournamentNumberEditMode = !tournamentNumberEditMode;
    if (GuiTextBox(matchDesignationRect, matchDesignationText, 128, matchDesignationEditMode)) matchDesignationEditMode = !matchDesignationEditMode;
    // draw fighter select boxes
    if (GuiDropdownBox(p1fighterRect, p1fighterDisplayList, &p1fighterActive, p1fighterEditMode)) p1fighterEditMode = !p1fighterEditMode;
    if (GuiDropdownBox(p2fighterRect, p2fighterDisplayList, &p2fighterActive, p2fighterEditMode)) p2fighterEditMode = !p2fighterEditMode;
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    if (GuiDropdownBox(p1costumeRect, p1costumeDisplayList, &p1costumeActive, p1costumeEditMode)) p1costumeEditMode = !p1costumeEditMode;
    if (GuiDropdownBox(p2costumeRect, p2costumeDisplayList, &p2costumeActive, p2costumeEditMode)) p2costumeEditMode = !p2costumeEditMode;
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    
    // draw format selection boxes
    if (GuiButton(reloadAssetsRect, GuiIconText(RAYGUI_ICON_RELOAD, NULL))) reloadAssetsPressed = !reloadAssetsPressed;
    if (GuiDropdownBox(coreAssetsRect, coreAssetsList, &coreAssetsActive, coreAssetsEditMode)) coreAssetsEditMode = !coreAssetsEditMode;
    if (GuiDropdownBox(templateAssetsRect, templateAssetsList, &templateAssetsActive, templateAssetsEditMode)) templateAssetsEditMode = !templateAssetsEditMode;
    
    // unlock the GUI if it was locked for input
    GuiUnlock();
}


/*
void SetupFolderEnvironment()
{
    // create necessary directories
#if defined(_WIN32)
    _mkdir("./cores");
    _mkdir("./templates");
    _mkdir("./output");
#else
    mkdir("./cores", 0700);
    mkdir("./templates", 0700);
    mkdir("./output", 0700);
#endif
    // 
}
*/




int main()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    
    // window data
    int currentMonitor = 0;
    Vector2 windowPos = (Vector2){ 0, 0 };
    Vector2 windowDragOffset = (Vector2){ 0, 0 };
    Vector2 newWindowPos = (Vector2){ 0, 0 };
    bool beingDragged = false;
    
    // window setup
    SetConfigFlags(FLAG_WINDOW_UNDECORATED);
    InitWindow(windowDims.width, windowDims.height, windowTitleText);
    SetTargetFPS(60);
    SetWindowSize(windowDims.width, windowDims.height);
    Image windowIcon = LoadImageFromResource(WICO_TG2);
    SetWindowIcon(windowIcon);
    // style setup
    GuiLoadStyleCyber();
    GuiSetStyle(TEXTBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    
    // position setup
    windowPos.x = (GetMonitorWidth(currentMonitor) - windowDims.width) / 2.0;
    windowPos.y = (GetMonitorHeight(currentMonitor) - windowDims.height) / 2.0;
    SetWindowPosition(windowPos.x, windowPos.y);
    
    // gui setup
    // - preview zone
    int currentView = VIEW_EDITOR;
    //generatedThumbnail = GenImageColor(1280, 720, WHITE);
    previewRenderTexture = LoadRenderTexture(360, 202);
    
    // Load resource cores
    //--------------------------------------------------------------------------------------
    // TODO: implement
    //--------------------------------------------------------------------------------------
    
    // Load templates
    //--------------------------------------------------------------------------------------
    // TODO: implement
    //--------------------------------------------------------------------------------------
    
    // Load format selections and persistant settings
    //--------------------------------------------------------------------------------------
    // TODO: implement
    //--------------------------------------------------------------------------------------
    
    //--------------------------------------------------------------------------------------

    // Main game loop (close on window close button or ESC key)
    while ((!WindowShouldClose()) && windowIsOpen)
    {
        // Update
        //----------------------------------------------------------------------------------
        // handle any dragging done since the previous frame
        if (beingDragged) {
            // set the new window position
            newWindowPos = GetMousePosition();
            windowPos.x = windowPos.x + (newWindowPos.x - windowDragOffset.x);
            windowPos.y = windowPos.y + (newWindowPos.y - windowDragOffset.y);
            // check if released
            if (!IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
                // respect window boundaries
                // TODO: fix this for multi-monitor setups
                if (windowPos.x < 0) windowPos.x = 0;
                if (windowPos.x > GetMonitorWidth(currentMonitor) - windowDims.width) windowPos.x = GetMonitorWidth(currentMonitor) - windowDims.width;
                if (windowPos.y < 0) windowPos.y = 0;
                if (windowPos.y > GetMonitorHeight(currentMonitor) - windowDims.height) windowPos.y = GetMonitorHeight(currentMonitor) - windowDims.height;
                beingDragged = false;
            }
            SetWindowPosition(windowPos.x, windowPos.y);
        // check if starting to drag
        } else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)
                && GetMouseX() >= 0 && GetMouseX() < windowDims.width
                && GetMouseY() >= 0 && GetMouseY() < 24
        ) {
            windowDragOffset = GetMousePosition();
            beingDragged = true;
        }
        // handle link buttons
        if (githubPressed) {
            OpenURLHidden("https://github.com/Augumat/thumbnail-generator-v2");
            githubPressed = !githubPressed;
        }
        if (helpPressed) {
            //OpenURLHidden("https://github.com/Augumat/thumbnail-generator-v2/wiki");
            // TODO: implement when going public
            helpPressed = !helpPressed;
        }
        if (bugsPressed) {
            OpenURLHidden("https://github.com/Augumat/thumbnail-generator-v2/issues");
            bugsPressed = !bugsPressed;
        }
        // handle switching format info
        if (!coreAssetsEditMode) ResetInputFields();
        if (!templateAssetsEditMode) ResetInputFields();
        // handle player info input
        //stub
        // handle action buttons
        if (resetPressed) {
            //stub
            resetPressed = !resetPressed;
        }
        if (generatePressed) {
            CompositeThumbnail(&previewRenderTexture);
            generatePressed = !generatePressed;
        }
        // handle batch generation
        if (IsFileDropped()
         && GetMouseX() >= batchPanelRect.x && GetMouseX() < batchPanelRect.x + batchPanelRect.width
         && GetMouseY() >= batchPanelRect.y && GetMouseY() < batchPanelRect.y + batchPanelRect.height
        ) {
            // grab the dropped files
            int fileCount = 0;
            char** files = GetDroppedFiles(&fileCount);
            CompositeBatch(files, fileCount);
            ClearDroppedFiles();
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        switch (currentView) {
            case VIEW_EDITOR:
                DrawEditorView();
                break;
            case VIEW_PREVIEW:
                //stub
                break;
            case VIEW_BATCH:
                //stub
                break;
        default:break;}
        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    
    // Save format selections and settings
    //--------------------------------------------------------------------------------------
    // TODO: implement
    //--------------------------------------------------------------------------------------
    
    // Cleanup
    //--------------------------------------------------------------------------------------
    CloseWindow();
    UnloadRenderTexture(previewRenderTexture);
    //--------------------------------------------------------------------------------------

    return 0;
}
