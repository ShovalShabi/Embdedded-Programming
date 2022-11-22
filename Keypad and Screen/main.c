#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/attribs.h>
#include <p32xxxx.h>
#include "BinaryAdding.h"
#include "ShiftingLights.h"
#include "FanMovement.h"
#include "ScreenOperations.h"
#include "Keypad.h"

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

int parser( int a);
void main()
{ 
    TRISA &= 0xff00;
    ////////////////////Switches and LEDS Configuration/////////////////////////
    TRISBbits.TRISB10 = 1; // RB10 (SW6) configured as input
    ANSELBbits.ANSB10 = 0; //Analog for RB11 configured as 0 for binary
    ANSELBbits.ANSB14 = 0; //Analog for RB11 configured as 0 for binary
    TRISBbits.TRISB14 = 0; //RB14 configured as output
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    //////////////////////Keypad Configuration//////////////////////////////////
    TRISCbits.TRISC2=0;//RC2
    TRISCbits.TRISC1=0;//RC1
    TRISCbits.TRISC4=0;//RC4            
    TRISGbits.TRISG6=0;//RG6
    ANSELGbits.ANSG6=0;//???????
    TRISCbits.TRISC3 =1;//RC3
    CNPUCbits.CNPUC3;               
    TRISGbits.TRISG7=1;//RG7
    ANSELGbits.ANSG7=0;
    CNPUGbits.CNPUG7;                  
    TRISGbits.TRISG8=1;//RG8
    ANSELGbits.ANSG8=0;//???????
    CNPUGbits.CNPUG8;//?????
    TRISGbits.TRISG9=1;//RG9
    ANSELGbits.ANSG9=0;//???????
    CNPUGbits.CNPUG9;//????? 

    ////////////////////Defining Numbers By  Key pad 4x4 Keypad/////////////////
    char scan_key[]={0x44,0x01,//1
                     0x34,0x02,//2
                     0x24,0x03,//3
                     0x43,0x04,//4
                     0x33,0x05,//5
                     0x23,0x06,//6
                     0x42,0x07,//7
                     0x32,0x08,//8
                     0x22,0x09,//9
                     0x41,0};//0
    //output
    PORTCbits.RC2=0; //x0
    PORTCbits.RC1=0; //x1
    PORTCbits.RC4=0; //x2
    PORTGbits.RG6=0; //x3
    //input
    PORTAbits.RA0=PORTCbits.RC3; //y0
    PORTAbits.RA1=PORTGbits.RG7; //y1
    PORTAbits.RA2=PORTGbits.RG8; //y2 
    PORTAbits.RA3=PORTGbits.RG9; //y3
    int factor=1,pressed=0;
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
    char choice=0xff,val;
    while(1)//In case RB9 (SW7) is off -> run program, otherwise -> exit program
    {
        val=readKey(&pressed);
        if(pressed)
        {
            choice=actualValue(val,scan_key);
            pressed=0;
        }
        if(choice!=HALT)//In case RB11 (SW5) is on -> skipping block, otherwise -> commit
        {
            /***Binary Adding***/
            if(choice==MODE_0)
            { 
                binaryAdding(&factor,modes[0],(char*)control,&choice,scan_key,&pressed);
            }
            /***Shifting Lights***/
            else if(choice==MODE_1)
            {
                shiftingLights(&factor,modes[1],(char*)control,&choice,scan_key,&pressed);
            }
            /***Fan Movement***/
            else if(choice==MODE_2)
            {
                fanMovement(&factor,modes[2],(char*)control,&choice,scan_key,&pressed);
            }
            /***Beep Mode***/
            else if(choice==BEEP)
            {
                setWholeScreen(modes[4],strings[3],(char*)control,&address);
                for(int j=0;j<2*SLEEP;j++);
                PORTBbits.RB14=0;
                for(int j=0;j<20000/factor;j++);
                PORTBbits.RB14=1;
                if(choice!=BEEP)
                    break;
            }
            /***Exiting Program Display***/
            else if(choice==EXIT)
            {
                address=0xa8;//Beginning  of second line cursor
                setWholeScreen(modes[5],strings[4],(char*)control,&address);
                PORTA=0;
                return;
            }
            else if(!pressed)//All switches are off
            {
                setWholeScreen(strings[0],strings[1],(char*)control,&address);
                for(int i=0;i<2*SLEEP;i++);//Busy waiting for user to choose option
            }
            /***Halt Mode Display***/
            if(choice==HALT)//In case RB11 (SW5) is on
            {
                address=0xa8;//Beginning  of second line cursor
                setWholeScreen(modes[3],strings[2],(char*)control,&address);
            }
        }
    }
    PORTA=0; //Turning off all the led lights
}

