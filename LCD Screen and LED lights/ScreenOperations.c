#include <xc.h>
#include <string.h>
#include "ScreenOperations.h"

void busy()
{
    char RD,RS;
    int STATUS_TRISE;
    int portMap;
    RD=PORTDbits.RD5;
    RS=PORTBbits.RB15;
    STATUS_TRISE=TRISE;
    PORTDbits.RD5 = 1;//w/r
    PORTBbits.RB15 = 0;//rs 
    portMap = TRISE;
    portMap |= 0x80;
    TRISE = portMap;
    do
    {
        PORTDbits.RD4=1;//enable=1
        PORTDbits.RD4=0;//enable=0
    }
    while(PORTEbits.RE7); // BF Register
    PORTDbits.RD5=RD; 
    PORTBbits.RB15=RS;
    TRISE=STATUS_TRISE;   
 }

void configControl(char* control)
{
    for(int i=0;i<6;i++)
    {
        PORTE=control[i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    }
}

void cleanScreen(char* control)
{
    PORTBbits.RB15=0;//rs=0 -> Instruction load for the control
    PORTDbits.RD5=0;//w=0
    configControl(control);
    PORTBbits.RB15=1;//rs=1 -> Data load for control to display
    PORTDbits.RD5=0;//w=0;
}

void displayOnScreen(char* word)
{
    for(int i=0;i<(int)strlen(word);i++)
    {
        PORTE=word[i];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
    }
}

void setWholeScreen(char* strFirstLine, char* strSecondLine, char* control,unsigned char* address)
{
    if(*address==0xc0)//In case starting to print on the first line shift to address second line
        *address=0xa8;
    cleanScreen(control);//Cleaning screen
    displayOnScreen(strFirstLine);//Printing to first line
    shiftCursor(address);//Shifting cursor to second line with target address for cursor
    displayOnScreen(strSecondLine);//Printing to second line
    (*address)++;
    for(int i=0;i<SLEEP;i++);//Busy waiting for text visibility to the user
}

void shiftCursor(unsigned char* address)
{
    PORTBbits.RB15=0;//rs=0 -> Instruction load for the control
    PORTDbits.RD5=0;//w=0
    PORTE=*address;//Configuring DDRAM address to 0xA8 for stating line 2 from beginning
    PORTDbits.RD4=1; //Enable
    PORTDbits.RD4=0; //Enable
    busy();
    PORTBbits.RB15=1;//rs=1 -> Data load for control to display
    PORTDbits.RD5=0;//w=0;
}