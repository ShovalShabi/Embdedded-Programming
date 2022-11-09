#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include "BinaryAdding.h"
#include "ShiftingLights.h"
#include "FanMovement.h"
#include "ScreenOperations.h"

#pragma config JTAGEN = OFF
#pragma config FWDTEN = OFF
#pragma config FNOSC = FRCPLL
#pragma config FSOSCEN = OFF
#pragma config POSCMOD = EC
#pragma config OSCIOFNC = ON
#pragma config FPBDIV = DIV_1
#pragma config FPLLIDIV = DIV_2
#pragma config FPLLMUL = MUL_20
#pragma config FPLLODIV = DIV_1
void main()
{ 
    int j;
    TRISA &= 0xff00;
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    TRISFbits.TRISF4 = 1; // RF4 (SW2) configured as input
    TRISDbits.TRISD15 = 1; // RD15 (SW3) configured as input
    TRISDbits.TRISD14 = 1; // RD14 (SW4) configured as input
    TRISBbits.TRISB11 = 1; // RB11 (SW5) configured as input
    ANSELBbits.ANSB11 = 0; // RB11 (SW5) disabled analog
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = 0; //Analog for RB11 configured as 0 for binary
    ANSELBbits.ANSB14 = 0; //Analog for RB11 configured as 0 for binary
    TRISBbits.TRISB14 = 0; //RB14 configured as output
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    int factor=1;
    char* strings[]={"Welcome!","Choose Operation","Halt","Beep Mode","Exit"};
    char* modes[]= {"Mode 0: ","Mode 1:","Mode 2:",
                    "Mode 5:","Mode 6:","Mode 7:"};
    char control[]={0x38,0x38,0x38,0xe,0x6,0x1};//Configuration for the control
    TRISBbits.TRISB15 = 0; // RB15 (DISP_RS) set as an output
    ANSELBbits.ANSB15 = 0; // disable analog functionality on RB15 (DISP_RS)
    TRISDbits.TRISD5 = 0; // RD5 (DISP_RW) set as an output
    TRISDbits.TRISD4 = 0; // RD4 (DISP_EN) set as an output
    TRISE&=0xff00;
    ANSELEbits.ANSE2 = 0;
    ANSELEbits.ANSE4 = 0;
    ANSELEbits.ANSE5 = 0;
    ANSELEbits.ANSE6 = 0;
    ANSELEbits.ANSE7 = 0;
    cleanScreen((char*)control);
    unsigned char address=0xa8;//Address of DDRAM to start of second line cell on LCD
    PORTA=0;//Reorder the LED presentations
    while(!PORTBbits.RB9)//In case RB9 (SW7) is off -> run program, otherwise -> exit program
    {
        if(!PORTBbits.RB11)//In case RB11 (SW5) is on -> skipping block, otherwise -> commit
        {
            /***Binary Adding***/
            if(PORTFbits.RF3 && !PORTFbits.RF5 && !PORTFbits.RF4 )//Priority: RF41 (SW2)>> RF5 (SW1) >> RF3 (SW0)
            { 
                binaryAdding(&factor,modes[0],(char*)control);
            }
            /***Shifting Lights***/
            else if(PORTFbits.RF5 && !PORTFbits.RF4)//Priority: RF4 (SW2)>> RF5 (SW1)
            {
                shiftingLights(&factor,modes[1],(char*)control);
            }
            /***Fan Movement***/
            else if(PORTFbits.RF4)// Max priority: RF4 (SW2)
            {
                fanMovement(&factor,modes[2],(char*)control);
            }
            /***Beep Mode***/
            else if(PORTBbits.RB10)
            {
                while(1)
                {
                    setWholeScreen(modes[3],strings[3],(char*)control,&address);
                    for(int j=0;j<2*SLEEP;j++);
                    PORTBbits.RB14=0;
                    for(int j=0;j<20000/factor;j++);
                    PORTBbits.RB14=1;
                    if(!PORTBbits.RB10)
                        break;
                }
            }
            else if(!PORTFbits.RF3 && !PORTFbits.RF5 && !PORTFbits.RF4 )//All switches are off
            {
                setWholeScreen(strings[0],strings[1],(char*)control,&address);
                for(int i=0;i<2*SLEEP;i++);//Busy waiting for user to choose option
            }
                
            /***Halt Mode Display***/
            if(PORTBbits.RB11)//In case RB11 (SW5) is on
            {
                address=0xa8;//Beginning  of second line cursor
                setWholeScreen(modes[4],strings[2],(char*)control,&address);
            }
            
            /***Exiting Program Display***/
            if(PORTBbits.RB9)
            {
                address=0xa8;//Beginning  of second line cursor
                setWholeScreen(modes[5],strings[4],(char*)control,&address);
            }

        }
    }
    PORTA=0; // Turning off all the led lights
}
