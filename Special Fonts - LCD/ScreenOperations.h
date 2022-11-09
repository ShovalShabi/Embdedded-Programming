#ifndef SCREEN_OPERATIONS    /* Guard against multiple inclusion */
#define SCREEN_OPERATIONS
#endif
// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void busy();
void configControl(char* control);
void cleanScreen(char* control);
void setFonts(char* bodyPart);
void displayOnScreen(char* bodyParts);
void setWholeScreen(char* body, char* head, char* control, char* address);
void shiftCursor(unsigned char* address);