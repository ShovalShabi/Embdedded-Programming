#include <xc.h>
#include "ShiftingLights.h"
#include "ScreenOperations.h"
#include "Keypad.h"

char* modeShift[]={"Shift Left Slow","Shift Left Fast","Shift Right Slow","Shift Right Fast"};

void shiftingLights(int* factor,char* mode,char* control,char* choice,char* numbers,int* pressed)
{
    PORTAbits.RA0=0x01;
    unsigned char address=0xa8;//Initial address for second line
    char val;
    int direction=0, speed=0;
    while(*choice!=EXIT && *choice!=HALT)//While keypad 5 is not pressed -> break or keypad 7 is not pressed -> exit program
    {
        val=readKey(pressed);
        if(*pressed)
        {
            *choice=actualValue(val,numbers);
            if(*choice==DIRECTION)//Handling Speed event
                if(direction)
                    direction=0;
                else
                    direction=1;
            if(*choice==SPEED)//Handling direction event
                if(speed)
                    speed=0;
                else
                    speed=1;
            *pressed=0;
        }
        if(*choice==MODE_2 || *choice==MODE_0 || *choice==BEEP)//Other events comes up while committing functionality
        {
            PORTA=0;
            break;
        }
        if(speed)//In case keypad 4 got pressed, dividing the wait time with factor*2
        {
            if(direction)
                setWholeScreen(mode,modeShift[3],control,&address);
            else
                setWholeScreen(mode,modeShift[1],control,&address);
            *factor=2;  
        }
        else//In case keypad 4 not pressed, dividing the wait time with factor
        {
            if(direction)
                setWholeScreen(mode,modeShift[2],control,&address);
            else
                setWholeScreen(mode,modeShift[0],control,&address);
            *factor=1;
        }
        if(!direction)
            PORTA *=2;//In case keypad 3 is not pressed -> regular activity
        else
            PORTA/=2;//In case keypad 3 is pressed -> activating reverse action

        if(!PORTAbits.RA7 && !direction)
            PORTAbits.RA0=0x01;//In case that the last led is off and keypad 3 is not pressed -> regular activity
        else if (!PORTAbits.RA0 && direction)
            PORTAbits.RA7 = 0x01;//In case that the first led is off and keypad 3 is pressed -> activating reverse
        for(int j=0;j<SLEEP/(*factor);j++);
    }
}