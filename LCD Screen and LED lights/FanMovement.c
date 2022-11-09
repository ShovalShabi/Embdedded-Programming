#include <xc.h>
#include "FanMovement.h"
#include "ScreenOperations.h"

char* modeFan[]={"Swing Down Slow","Swing Down Fast","Swing Up Slow","Swing Up Fast"};

void fanMovement(int* factor,char* mode, char* control)
{
    unsigned char presentation[]={0x18,0x24,0x42,0x81};
    int sizePresention=4;
    unsigned char address=0xa8;//Initial address for second line
     while(!PORTBbits.RB11 && !PORTBbits.RB9)//RB11 (SW5) is on -> break, RB9 (SW7) is on -> exit program 
    {
        if(!PORTFbits.RF4)//Switching off
        {
            PORTA=0;
            break;
        }

        if(PORTDbits.RD14) // in case RD14 (SW4) is on,dividing wait time in factor
        {
            if(PORTDbits.RD15)
                setWholeScreen(mode,modeFan[3],control,&address);
            else
                setWholeScreen(mode,modeFan[1],control,&address);
            *factor=2;  
        }
        else // in case RD14 (SW4) is off,dividing wait time in factor
        {
            if(PORTDbits.RD15)
                setWholeScreen(mode,modeFan[2],control,&address);
            else
                setWholeScreen(mode,modeFan[0],control,&address);
            *factor=1;
        }
        
        if(!PORTDbits.RD15)//in case RD15 (SW3) is off -> regular activity
        {
            for(int i=0;i<sizePresention;i++)
            {
                PORTA=presentation[i];
                for(int j=0;j<SLEEP/(*factor);j++);
            }
        }
        else//in case RD15 (SW3) is on -> activating reverse action
        {
            for(int i=sizePresention-1;i>=0;i--)
            {
                PORTA=presentation[i];
                for(int j=0;j<SLEEP/(*factor);j++);
            }
        }
    }
}