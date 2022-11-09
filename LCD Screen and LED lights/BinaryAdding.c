#include <xc.h>
#include "BinaryAdding.h"
#include "ScreenOperations.h"

char* modeBinary[]={"Counter Up Slow","Counter Up Fast","Counter Down Slow","Counter Down Fast"};

void binaryAdding(int* factor,char* mode, char* control)
{
    unsigned char address=0xa8;//Initial address for second line
    while(!PORTBbits.RB11 && !PORTBbits.RB9)//RB11 (SW5) is on -> break, RB9 (SW7) is on -> exit program
    {
        if(!PORTFbits.RF3 || PORTFbits.RF5 || PORTFbits.RF4)//Other Priority comes up while committing functionality, or switching off
        {
            PORTA=0;
            break;
        }
        if(PORTDbits.RD14)// in case RD14 (SW4) is on -> dividing wait time in factor
        {
            if(PORTDbits.RD15)
                setWholeScreen(mode,modeBinary[3],control,&address);
            else
                setWholeScreen(mode,modeBinary[1],control,&address);
            *factor=2; 
        }  
        else// in case RD14 (SW4) is off- -> dividing wait time in factor
        {
            if(PORTDbits.RD15)
                setWholeScreen(mode,modeBinary[2],control,&address);
            else
                setWholeScreen(mode,modeBinary[0],control,&address);
            *factor=1;
        }
        if(PORTDbits.RD15)
            PORTA--;//in case RD15 (SW3) is on -> activating reverse action
        else
            PORTA++;//in case RD15 (SW3) is off -> regular activity
        for(int i=0;i<SLEEP/(*factor);i++);
    }
}