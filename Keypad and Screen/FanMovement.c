#include <xc.h>
#include "FanMovement.h"
#include "ScreenOperations.h"
#include "Keypad.h"

char* modeFan[]={"Swing Down Slow","Swing Down Fast","Swing Up Slow","Swing Up Fast"};

void fanMovement(int* factor,char* mode,char* control,char* choice,char* numbers,int* pressed)
{
    unsigned char presentation[]={0x18,0x24,0x42,0x81};
    int sizePresention=4;
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
        if(*choice==MODE_1 || *choice==MODE_0 || *choice==BEEP)//Other events comes up while committing functionality
        {
            PORTA=0;
            break;
        }

        if(speed) //In case keypad 4 got pressed, dividing the wait time with factor*2
        {
            if(direction)
                setWholeScreen(mode,modeFan[3],control,&address);
            else
                setWholeScreen(mode,modeFan[1],control,&address);
            *factor=2;  
        }
        else //In case keypad 4 not pressed, dividing the wait time with factor
        {
            if(direction)
                setWholeScreen(mode,modeFan[2],control,&address);
            else
                setWholeScreen(mode,modeFan[0],control,&address);
            *factor=1;
        }
        
        if(!direction)//In case keypad 3 is not pressed
        {
            for(int i=0;i<sizePresention;i++)
            {
                PORTA=presentation[i];
                for(int j=0;j<SLEEP/(*factor);j++);
            }
        }
        else//In case keypad 3 is pressed
        {
            for(int i=sizePresention-1;i>=0;i--)
            {
                PORTA=presentation[i];
                for(int j=0;j<SLEEP/(*factor);j++);
            }
        }
    }
}