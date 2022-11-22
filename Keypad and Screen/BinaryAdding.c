#include <xc.h>
#include "BinaryAdding.h"
#include "ScreenOperations.h"
#include "Keypad.h"

char* modeBinary[]={"Counter Up Slow","Counter Up Fast","Counter Down Slow","Counter Down Fast"};

void binaryAdding(int* factor,char* mode,char* control ,char* choice ,char* numbers,int* pressed)
{
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
        if(*choice==MODE_1 || *choice==MODE_2 || *choice==BEEP)//Other events comes up while committing functionality
        {
            PORTA=0;
            break;
        }
        if(speed)//In case keypad 4 got pressed, dividing the wait time with factor*2
        {
            if(direction)
                setWholeScreen(mode,modeBinary[3],control,&address);
            else
                setWholeScreen(mode,modeBinary[1],control,&address);
            *factor=2; 
        }  
        else//In case keypad 4 not pressed, dividing the wait time with factor
        {
            if(direction)
                setWholeScreen(mode,modeBinary[2],control,&address);
            else
                setWholeScreen(mode,modeBinary[0],control,&address);
            *factor=1;
        }
        if(direction)
            PORTA--;//In case keypad 3 got pressed -> activating reverse action
        else
            PORTA++;//In case keypad 3 is not pressed -> regular activity
        for(int i=0;i<SLEEP/(*factor);i++);
    }
}