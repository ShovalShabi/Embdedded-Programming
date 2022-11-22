#ifndef SCREEN_OPERATIONS    /* Guard against multiple inclusion */
#define SCREEN_OPERATIONS
#endif
#define SLEEP 64000
#define MODE_0 0
#define MODE_1 1
#define MODE_2 2
#define DIRECTION 3
#define SPEED 4
#define HALT 5
#define BEEP 6
#define EXIT 7
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