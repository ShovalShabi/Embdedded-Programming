#ifndef SCREEN_OPERATIONS    /* Guard against multiple inclusion */
#define SCREEN_OPERATIONS
#endif
#define SLEEP 64000
// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

void busy();
void configControl(char* control);
void cleanScreen(char* control);
void displayOnScreen(char* word);
void setWholeScreen(char* strFirstLine, char* strSecondLine, char* control,unsigned char* address);
void shiftCursor(unsigned char* address);