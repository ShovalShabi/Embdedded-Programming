#include <xc.h>
#include "Fish.h"
#include "ScreenOperations.h"

#define SLEEP 64000
#define BEEP_SLEEP 20000

char* modeBinary[]={"Counter Up Slow","Counter Up Fast","Counter Down Slow","Counter Down Fast"};

void fishMovement(char* control)
{
    unsigned char address=0xa8;//0xa8 is the address of DDRAM to start second second start point on LCD,
                               //0x80 is for the start of the upper line     
    char bodyParts[]={0x13,0x17,0x1f,0x1f,0x1f,0x1f,0x17,0x13,//0->regular body
                 0x30,0x38,0x3c,0x34,0x3c,0x3c,0x38,0x30,//1->head           
                 0x53,0x5f,0x5f,0x5f,0x53,0x53,0x43,0x41,//2->body up
                 0x61,0x63,0x73,0x77,0x7f,0x7f,0x7f,0x73};//3->body down
    
    setFonts((char*)bodyParts);
    char movement[]={0,2,3,1};//0-> regular body, 2-> body up,3-> body down, 1-> head
    int indexBodyParts=0, indexHead=3;
    while(1)// RF3 (SW0) if SW0 is on the fish moves otherwise it stops
    {
        if(!PORTFbits.RF3)//In case RF3 (SW0) is off -> commit
        {
            setWholeScreen(&movement[indexBodyParts],&movement[indexHead],control,&address);
            indexBodyParts++;
            if(indexBodyParts==indexHead)
                indexBodyParts=0;
        }
        /***Beep Mode***/
        if(PORTBbits.RB9)//In case RB9 (SW7) is On -> beep, otherwise -> silence
        {
            while(1)
            {
                PORTBbits.RB14=0;
                for(int j=0;j<5*BEEP_SLEEP;j++);
                PORTBbits.RB14=1;
                if(!PORTBbits.RB10)
                    break;
            }
        }
        else
            for(int i=0;i<5*SLEEP;i++);//Busy waiting for text visibility to the user    
    }
}