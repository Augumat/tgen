#include <stdlib.h>
#include <ctype.h>
#include "ray/raylib.h"
#include "ray/raygui.h"





// Custom Searchable Dropdown Box control
// NOTE: Returns mouse click
bool GuiSearchDropdownBox(Rectangle bounds, char* query, int queryMaxLength, const char* elements, int* choice, bool editMode)
{
    // set up temporary vars
    GuiControlState state = guiState;
    int cellSelected = 0;
    int cellFocused = -1;
    bool pressed = false;
    
    // find current query string length
    int qLen = 0;
    while (query[qLen]) { qLen++; }
    
    // find length and element count of elements
    int eLen = 0;
    int eCount = 1;
    while (elements[eLen]) {
        if (elements[eLen] == ';') { eCount++; }
        eLen++;
    }
    
    // ELEMENT LIST CULLING (SEARCH) ------------------------------------------
    // remove elements that do not contain query as a substring (only matching
    // elements should be displayed in the dropdown) and keep track of the
    // original indexes of each element in the output list
    char* visibleElements = (char*)(calloc(eLen, 1));
    int visibleCount = eCount;
    char** elemStrLoc = (char**)(calloc(eCount, sizeof(char*)));
    int* idxInOriginal = (int*)(calloc(eCount, sizeof(int)));
    bool curMatches = false;
    int runMatches = 0;
    int srcElem = 0;
    int dstElem = 0;
    int curLen = 0;
    int srcIdx = 0;
    int dstIdx = 0;
    // if qLen is 0, curMatches will remain true through the entire loop (since
    // all elements should be displayed while the query is empty)
    if (qLen == 0) { curMatches = true; }
    for (; srcIdx < eLen; ++srcIdx) {
        // when the end of the current element is reached, reset trackers and
        // the output cursor if there was no matching run in this element
        if (elements[srcIdx] == ';') {
            if (curMatches) {
                visibleElements[dstIdx] = '\0';
                if (srcElem == *choice) { cellSelected = dstElem; }
                elemStrLoc[dstElem] = visibleElements + (dstIdx - curLen);
                idxInOriginal[dstElem] = srcElem;
                dstElem++;
                dstIdx++;
            } else {
                dstIdx -= curLen;
            }
            runMatches = 0;
            if (qLen != 0) { curMatches = false; }
            //curMatches = false;
            curLen = 0;
            srcElem++;
            continue;
        }
        // attempt to match the current element to the query
        char lowerSrc = tolower(elements[srcIdx]);
        char lowerQ = tolower(query[runMatches]);
        if (curMatches || lowerSrc < 'a' || lowerSrc > 'z') {
            // if the current character isn't alphabetical (queries only match
            // to plaintext) or already has a matching run, simply continue to
            // copy the element over
        } else if (lowerSrc == lowerQ) {
            // start or continue a matching run between the current element and
            // the query string
            runMatches++;
            if (runMatches == qLen) curMatches = true;
        } else {
            // character mismatch, reset any active run
            runMatches = 0;
        }
        // copy the current character and increment counters (copies the
        // original case of the character, not the lowercase used for matching)
        visibleElements[dstIdx] = elements[srcIdx];
        curLen++;
        dstIdx++;
    }
    // record the index of the last element
    if (curMatches) {
        visibleElements[dstIdx] = elements[srcIdx];
        if (srcElem == *choice) { cellSelected = dstElem; }
        elemStrLoc[dstElem] = visibleElements + (dstIdx - curLen);
        idxInOriginal[dstElem] = srcElem;
        dstElem++;
    }
    // store how many elements need to be displayed
    visibleCount = dstElem;
    // ------------------------------------------------------------------------
    
    // define bounds for open and closed state
    Rectangle itemBounds = bounds;
    Rectangle boundsOpen = bounds;
    boundsOpen.height = (visibleCount + 1)*(bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_PADDING));
    
    // set up the cursor for the query input area
    Rectangle cursor = {
        bounds.x + GuiGetStyle(TEXTBOX, TEXT_PADDING) + GetTextWidth(query) + 2,
        bounds.y + bounds.height/2 - GuiGetStyle(DEFAULT, TEXT_SIZE) + 4,
        4,
        (float)((GuiGetStyle(DEFAULT, TEXT_SIZE)*2) - 8)
    };

    // UPDATE CONTROL ---------------------------------------------------------
    // text
    Vector2 mousePoint = GetMousePosition();
    if (editMode) {
        state = GUI_STATE_PRESSED;
        // updates for the text box
        int key = GetCharPressed();
        int keyCount = (int)strlen(query);
        // Only allow keys in range [32..125]
        if (keyCount < (queryMaxLength - 1)) {
            float maxWidth = (bounds.width - (GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING) * 2));
            if ((GetTextWidth(query) < (maxWidth - GuiGetStyle(DEFAULT, TEXT_SIZE))) && (key >= 32)) {
                int byteSize = 0;
                const char* textUTF8 = CodepointToUTF8(key, &byteSize);
                for (int i = 0; i < byteSize; ++i) {
                    query[keyCount] = textUTF8[i];
                    keyCount++;
                }
                query[keyCount] = '\0';
            }
        }
        // Delete text
        if (keyCount > 0) {
            if (IsKeyPressed(KEY_BACKSPACE)) {
                keyCount--;
                query[keyCount] = '\0';
                if (keyCount < 0) keyCount = 0;
            }
        }
        // use arrow keys to move up or down the list of visible options
        if (IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_DOWN)) {
            if (IsKeyPressed(KEY_UP) && cellSelected > 0) cellSelected--;
            if (IsKeyPressed(KEY_DOWN) && cellSelected < (visibleCount - 1)) cellSelected++;
            *choice = idxInOriginal[cellSelected];
        }
        // autofill first matching choice if hitting enter or clicking off
        if (IsKeyPressed(KEY_ENTER) || (!CheckCollisionPointRec(mousePoint, boundsOpen) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON))) {
            *choice = idxInOriginal[cellSelected];
            query[0] = '\0';
            pressed = true;
        }
        // check text alignment to position cursor properly
        int textAlignment = GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT);
        if (textAlignment == GUI_TEXT_ALIGN_CENTER) {
            cursor.x = bounds.x + (GetTextWidth(query) / 2) + (bounds.width / 2) + 1;
        } else if (textAlignment == GUI_TEXT_ALIGN_RIGHT) {
            cursor.x = bounds.x + bounds.width - GuiGetStyle(TEXTBOX, TEXT_INNER_PADDING);
        }
        
        // updates for the expanded list panel
        if ((state != GUI_STATE_DISABLED) && (editMode || !guiLocked) && (eCount > 1)) {
            state = GUI_STATE_PRESSED;
            // Check if mouse has been pressed or released outside limits
            if (!CheckCollisionPointRec(mousePoint, boundsOpen)) {
                if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) || IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) pressed = true;
            }
            // Check if already selected item has been pressed again
            if (CheckCollisionPointRec(mousePoint, bounds) && IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) pressed = true;
            // Check focused and selected item
            for (int i = 0; i < eCount; ++i) {
                // Update item rectangle y position for next item
                itemBounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_PADDING));
                if (CheckCollisionPointRec(mousePoint, itemBounds)) {
                    cellFocused = i;
                    if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON)) {
                        cellSelected = i;
                        // record which element the user selected
                        *choice = idxInOriginal[cellSelected];
                        // clear the query string so it will be empty next time
                        // the box is selected
                        query[0] = '\0';
                        // set the return flag
                        pressed = true;
                    }
                    break;
                }
            }
            itemBounds = bounds;
        }
    } else {
        // if the box isn't currently selected, check if it was clicked
        if (CheckCollisionPointRec(mousePoint, bounds)) {
            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                pressed = true;
                state = GUI_STATE_PRESSED;
            } else {
                state = GUI_STATE_FOCUSED;
            }
        }
    }
    // ------------------------------------------------------------------------
    
    // DRAW CONTROL -----------------------------------------------------------
    if (editMode) {
        // draw the full dropdown panel
        GuiPanel(boundsOpen);
        // draw the textbox
        GuiDrawRectangle(
            bounds,
            GuiGetStyle(TEXTBOX, BORDER_WIDTH),
            Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER + (state*3))), guiAlpha),
            Fade(GetColor(GuiGetStyle(TEXTBOX, BASE_COLOR_PRESSED)), guiAlpha)
        );
        // draw query
        GuiDrawText(
            query,
            GetTextBounds(TEXTBOX, bounds),
            GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT),
            Fade(GetColor(GuiGetStyle(TEXTBOX, TEXT + state*3)), guiAlpha)
        );
        // draw query box cursor
        GuiDrawRectangle(
            cursor,
            0,
            BLANK,
            Fade(GetColor(GuiGetStyle(TEXTBOX, BORDER_COLOR_PRESSED)), guiAlpha)
        );
        // draw visible list elements
        for (int i = 0; i < visibleCount; ++i) {
            // Update element rectangle position for next element
            itemBounds.y += (bounds.height + GuiGetStyle(DROPDOWNBOX, DROPDOWN_ITEMS_PADDING));
            if (i == cellSelected) {
                GuiDrawRectangle(
                    itemBounds,
                    GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH),
                    Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_PRESSED)), guiAlpha),
                    Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_PRESSED)), guiAlpha)
                );
                GuiDrawText(
                    elemStrLoc[i],
                    GetTextBounds(TEXTBOX, itemBounds),
                    GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT),
                    Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_PRESSED)), guiAlpha)
                );
            } else if (i == cellFocused) {
                GuiDrawRectangle(
                    itemBounds,
                    GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH),
                    Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BORDER_COLOR_FOCUSED)), guiAlpha),
                    Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BASE_COLOR_FOCUSED)), guiAlpha)
                );
                GuiDrawText(
                    elemStrLoc[i],
                    GetTextBounds(TEXTBOX, itemBounds),
                    GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT),
                    Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_FOCUSED)), guiAlpha)
                );
            } else {
                GuiDrawText(
                    elemStrLoc[i],
                    GetTextBounds(TEXTBOX, itemBounds),
                    GuiGetStyle(DROPDOWNBOX, TEXT_ALIGNMENT),
                    Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT_COLOR_NORMAL)), guiAlpha)
                );
            }
        }
    } else {
        // draw the closed dropdown box
        GuiDrawRectangle(
            bounds,
            GuiGetStyle(DROPDOWNBOX, BORDER_WIDTH),
            Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BORDER + state*3)), guiAlpha),
            Fade(GetColor(GuiGetStyle(DROPDOWNBOX, BASE + state*3)), guiAlpha)
        );
        // draw selected element
        GuiDrawText(
            elemStrLoc[*choice],
            GetTextBounds(TEXTBOX, bounds),
            GuiGetStyle(TEXTBOX, TEXT_ALIGNMENT),
            Fade(GetColor(GuiGetStyle(TEXTBOX, TEXT + state*3)), guiAlpha)
        );
        // draw dropdown arrow
        GuiDrawText(
            "#120#",
            RAYGUI_CLITERAL(Rectangle){ bounds.x + bounds.width - GuiGetStyle(DROPDOWNBOX, ARROW_PADDING), bounds.y + bounds.height/2 - 6, 10, 10 },
            GUI_TEXT_ALIGN_CENTER,
            Fade(GetColor(GuiGetStyle(DROPDOWNBOX, TEXT + (state*3))), guiAlpha)
        );
    }
    // ------------------------------------------------------------------------
    
    // clean up and return
    free(visibleElements);
    free(elemStrLoc);
    free(idxInOriginal);
    return pressed;
}

//stub
bool GuiIconDropdownBox()
{
    //stub
    return false;
}

//stub
bool GuiHoverButton()
{
    //stub
    return false;
}

//stub
bool GuiHoverToggleBox()
{
    //stub
    return false;
}