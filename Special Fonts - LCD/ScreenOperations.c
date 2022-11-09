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
    int index=0;
    while(control[index])
    {
        PORTE=control[index];
        PORTDbits.RD4=1;
        PORTDbits.RD4=0;
        busy();
        index++;
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

void setFonts(char* bodyParts)
{
    PORTBbits.RB15 = 1;//rs
    for(int i = 0;i < (int)strlen(bodyParts);i++)
    {
        PORTE=bodyParts[i];
        PORTDbits.RD4=1;//enable=1
        PORTDbits.RD4=0;//enable=0
        busy();
    }
}

void displayOnScreen(char* bodyPart)
{
    PORTBbits.RB15=1;//rs=1 -> Data load for control to display
    PORTE=*bodyPart; //single char
    PORTDbits.RD4=1;
    PORTDbits.RD4=0;
    busy();
}

void setWholeScreen(char* body, char* head, char* control, char* address)
{
    cleanScreen(control);//Cleaning screen
    shiftCursor(address);//Shifting cursor to second line with target address for cursor
    displayOnScreen(body);//Printing on screen in one of each line by cases
    displayOnScreen(head);//Printing on screen in one of each line by cases
    (*address)++;//because of the two parts of the fish
}

void shiftCursor(unsigned char* address)
{
    PORTBbits.RB15=0;//rs=0 -> Instruction load for the control
    PORTDbits.RD5=0;//w=0
    PORTE=*address;//Configuring DDRAM address to 0xA8 for stating line 2 from beginning
    PORTDbits.RD4=1; //Enable
    PORTDbits.RD4=0; //Enable
    busy();
    if (PORTFbits.RF5 && PORTE >0x90)
        *address=0x80;
    if(!PORTFbits.RF5 && (PORTE >0xb8 || PORTE<0xa8))
        *address=0xa8;
    PORTBbits.RB15=1;//rs=1 -> Data load for control to display
    PORTDbits.RD5=0;//w=0;
}