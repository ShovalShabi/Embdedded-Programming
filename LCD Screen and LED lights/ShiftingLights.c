#include <xc.h>
#include "ShiftingLights.h"
#include "ScreenOperations.h"

char* modeShift[]={"Shift Left Slow","Shift Left Fast","Shift Right Slow","Shift Right Fast"};

void shiftingLights(int* factor,char* mode,char* control)
{
    displayOnScreen(modeShift[0]);
    PORTAbits.RA0=0x01;
    unsigned char address=0xa8;//Initial address for second line
    while(!PORTBbits.RB11 && !PORTBbits.RB9)//RB11 (SW5) is on -> break, RB9 (SW7) is on -> exit program
    {
        if(!PORTFbits.RF5 || PORTFbits.RF4)//Other Priority comes up while committing functionality, or switching off
        {
            PORTA=0;
            break;
        }
        if(PORTDbits.RD14)// in case RD14 (SW4) is on,dividing wait time in factor
        {
            if(PORTDbits.RD15)//In case fast mode is one
                setWholeScreen(mode,modeShift[3],control,&address);
            else//In case fast mode is off
                setWholeScreen(mode,modeShift[1],control,&address);
            *factor=2;  
        }
        else// in case RD14 (SW4) is off,dividing wait time in factor
        {
            if(PORTDbits.RD15)
                setWholeScreen(mode,modeShift[2],control,&address);
            else
                setWholeScreen(mode,modeShift[0],control,&address);
            *factor=1;
        }
        if(!PORTDbits.RD15)
            PORTA *=2;//in case RD15 (SW3) is off -> regular activity
        else
            PORTA/=2;//in case RD15 (SW3) is on -> activating reverse action

        if(!PORTAbits.RA7 && !PORTDbits.RD15)
            PORTAbits.RA0=0x01;//In case that the last led is off and in case RD15 (SW3) is off -> regular activity
        else if (!PORTAbits.RA0 && PORTDbits.RD15)
            PORTAbits.RA7 = 0x01;//In case that the first led is off and in case RD15 (SW3) is on -> activating reverse
        for(int j=0;j<SLEEP/(*factor);j++);
    }
}