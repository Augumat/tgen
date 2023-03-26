// includes
#include "ray/raylib.h"
#define RAYGUI_IMPLEMENTATION
#define RAYGUI_CUSTOM_ICONS
#include "icons.h"
#include "ray/raygui.h"
#include "cyber.h"
#include "windows/resource.h"
#include "windows/winwrapper.h"
#include "augui.h"
#include "generator.h"
#include "loader.h"
// testing
//#include <stdio.h>





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
static const Rectangle helpRect = (Rectangle){ 595, 3, 18, 18 };
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
static const Rectangle p1NameRect = (Rectangle){ 13, 96, 186, 32 };
static const Rectangle p1FighterRect = (Rectangle){ 13, 130, 160, 24 };
static const Rectangle p1CostumeRect = (Rectangle){ 175, 130, 24, 24 };
// - - player 2
static const Rectangle p2NameRect = (Rectangle){ 226, 96, 186, 32 };
static const Rectangle p2FighterRect = (Rectangle){ 226, 130, 160, 24 };
static const Rectangle p2CostumeRect = (Rectangle){ 388, 130, 24, 24 };
// - - tournament and match designation
static const Rectangle tournamentNameRect = (Rectangle){ 437, 96, 160, 24 };
static const Rectangle tournamentNumberRect = (Rectangle){ 599, 96, 24, 24 };
static const Rectangle matchDesignationRect = (Rectangle){ 437, 122, 186, 32 };
// - actions zone
static const Rectangle actionZoneRect = (Rectangle){ 391, 174, 240, 64 };
static const Rectangle clearRect = (Rectangle){ 399, 182, 110, 48 };
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
static bool helpPressed = false;
// - format settings zone
// - - core asset selector
static bool coreAssetsEditMode = false;
static int coreAssetsChoice = 0;
static char coreAssetsQuery[128] = "";
static char* coreAssetsList = "core1;core2";//TODO: dynamic
// - - template asset selector
static bool templateAssetsEditMode = false;
static int templateAssetsChoice = 0;
static char templateAssetsQuery[128] = "";
static char* templateAssetsList = "template1;template2;template3";//TODO: dynamic
// - - reload formats button
static bool reloadAssetsPressed = false;
// - player zone
// - - player 1 name input
static bool p1NameEditMode = false;
static char p1Name[128] = "Player 1";
// - - player 1 fighter selector
static bool p1FighterEditMode = false;
static int p1FighterChoice = 0;
static char p1FighterQuery[128] = "";
static char* p1FighterList = "Mario;Luigi;Bowser;Peach;Daisy;Bowser Jr.;Pirahna Plant;Wario";//TODO: dynamic
// - - player 1 costume selector
static bool p1CostumeEditMode = false;
static int p1CostumeActive = 0;
static char* p1CostumeList = "";
// - - player 2 name input
static bool p2NameEditMode = false;
static char p2Name[128] = "Player 2";
// - - player 2 fighter selector
static bool p2FighterEditMode = false;
static int p2FighterChoice = 0;
static char p2FighterQuery[128] = "";
static char* p2FighterList = "Mario;Luigi;Bowser;Peach;Daisy;Bowser Jr.;Pirahna Plant;Wario";//TODO: dynamic
// - - player 2 costume selector
static bool p2CostumeEditMode = false;
static int p2CostumeActive = 0;
static char* p2CostumeList = "";
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
static bool clearPressed = false;
static bool generatePressed = false;
// - preview zone
static RenderTexture2D previewRenderTexture = { 0 };
// - batch zone
static char batchPanelText[128] = "Drag batch files here";
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
void ClearInputFields()
{
    // TODO: implement
    return;
}

void DrawEditorView()
{
    // lock the GUI if any boxes are being edited
    if (coreAssetsEditMode || templateAssetsEditMode
     || p1FighterEditMode || p2FighterEditMode
     || p1CostumeEditMode || p2CostumeEditMode
    ) GuiLock();
    
    // draw the window itself
    if (windowIsOpen) windowIsOpen = !GuiWindowBox(windowDims, NULL);
    if (GuiButton(helpRect, GuiIconText(RAYGUI_ICON_HELP_SMALL, NULL))) helpPressed = !helpPressed;
    
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
    if (GuiButton(clearRect, GuiIconText(RAYGUI_ICON_CLEAR, "Clear Fields"))) clearPressed = !clearPressed;
    if (GuiButton(generateRect, GuiIconText(RAYGUI_ICON_PLAYER_PLAY, "Generate"))) generatePressed = !generatePressed;
    
    // draw match information boxes
    if (GuiTextBox(p1NameRect, p1Name, 128, p1NameEditMode)) p1NameEditMode = !p1NameEditMode;
    if (GuiTextBox(p2NameRect, p2Name, 128, p2NameEditMode)) p2NameEditMode = !p2NameEditMode;
    if (GuiTextBox(tournamentNameRect, tournamentNameText, 128, tournamentNameEditMode)) tournamentNameEditMode = !tournamentNameEditMode;
    if (GuiValueBox(tournamentNumberRect, "", &tournamentNumberValue, 0, 999, tournamentNumberEditMode)) tournamentNumberEditMode = !tournamentNumberEditMode;
    if (GuiTextBox(matchDesignationRect, matchDesignationText, 128, matchDesignationEditMode)) matchDesignationEditMode = !matchDesignationEditMode;
    // draw fighter select boxes
    if (GuiSearchDropdownBox(p1FighterRect, p1FighterQuery, 128, p1FighterList, &p1FighterChoice, p1FighterEditMode)) p1FighterEditMode = !p1FighterEditMode;
    if (GuiSearchDropdownBox(p2FighterRect, p2FighterQuery, 128, p2FighterList, &p2FighterChoice, p2FighterEditMode)) p2FighterEditMode = !p2FighterEditMode;
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_CENTER);
    if (GuiDropdownBox(p1CostumeRect, p1CostumeList, &p1CostumeActive, p1CostumeEditMode)) p1CostumeEditMode = !p1CostumeEditMode;
    if (GuiDropdownBox(p2CostumeRect, p2CostumeList, &p2CostumeActive, p2CostumeEditMode)) p2CostumeEditMode = !p2CostumeEditMode;
    GuiSetStyle(DROPDOWNBOX, TEXT_ALIGNMENT, GUI_TEXT_ALIGN_LEFT);
    
    // draw format selection boxes
    if (GuiButton(reloadAssetsRect, GuiIconText(RAYGUI_ICON_RELOAD, NULL))) reloadAssetsPressed = !reloadAssetsPressed;
    if (GuiSearchDropdownBox(coreAssetsRect, coreAssetsQuery, 128, coreAssetsList, &coreAssetsChoice, coreAssetsEditMode)) coreAssetsEditMode = !coreAssetsEditMode;
    if (GuiSearchDropdownBox(templateAssetsRect, templateAssetsQuery, 128, templateAssetsList, &templateAssetsChoice, templateAssetsEditMode)) templateAssetsEditMode = !templateAssetsEditMode;
    
    // unlock the GUI if it was locked for input
    GuiUnlock();
}





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
    
    // Load resources
    //--------------------------------------------------------------------------------------
    // TODO: implement
    int stat = 0;//TGLoadResources();
    if (stat) {
        printf("failed with error code %i\n", stat);
        return 1;
    }
    //--------------------------------------------------------------------------------------
    
    // Load format selections and persistant settings
    //--------------------------------------------------------------------------------------
    // TODO: implement
    //load settings
    //load format selections
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
        if (helpPressed) {
            OpenURLHidden("https://github.com/Augumat/tgen/wiki");
            helpPressed = !helpPressed;
        }
        // handle switching format info
        //TODO: fix this, it's definitely not what it's supposed to be
        if (!coreAssetsEditMode) /*stub*/;
        if (!templateAssetsEditMode) /*stub*/;
        // handle player info input
        //stub
        // handle action buttons
        if (clearPressed) {
            ClearInputFields();
            clearPressed = !clearPressed;
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
    //save format selections
    //save settings
    //
    //--------------------------------------------------------------------------------------
    
    // Cleanup
    //--------------------------------------------------------------------------------------
    CloseWindow();
    UnloadRenderTexture(previewRenderTexture);
    TGUnloadResources();
    //--------------------------------------------------------------------------------------

    return 0;
}
