#ifndef KEYPAD    /* Guard against multiple inclusion */
#define KEYPAD
#endif
/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Constants                                                         */
/* ************************************************************************** */
/* ************************************************************************** */
#define BASE 10
/* ************************************************************************** */
/** Descriptive Constant Name
  @Summary
    Value for base number, used for itoa function.
 */

// *****************************************************************************
// *****************************************************************************
// Section: Interface Functions
// *****************************************************************************
// *****************************************************************************

char readKey(int* pressed);
//void eventKey(char* numConfigs, int sizeNumConfigs, int value,char* strFirstLine, char* strSecondLine, char* control,unsigned char* address);
void leaveKey();
int parser(int num);
char actualValue(char num,char* numbers);