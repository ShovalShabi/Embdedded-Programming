#include <xc.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Fish.h"
#include "ScreenOperations.h"

#pragma config JTAGEN = OFF     
#pragma config FWDTEN = OFF
#pragma config FNOSC =	FRCPLL
#pragma config FSOSCEN =OFF
#pragma config POSCMOD =EC
#pragma config OSCIOFNC =ON
#pragma config FPBDIV =DIV_1
#pragma config FPLLIDIV =DIV_2
#pragma config FPLLMUL =MUL_20
#pragma config FPLLODIV =DIV_1

void main()
{ 
    TRISFbits.TRISF3 = 1; // RF3 (SW0) configured as input
    TRISFbits.TRISF5 = 1; // RF5 (SW1) configured as input
    ANSELBbits.ANSB14 = 0; //Analog for RB14 configured as 0 for binary (Buzzer)
    TRISBbits.TRISB14 = 0; //RB14 configured as output
    TRISBbits.TRISB9 = 1; // RB9 (SW7) configured as input
    ANSELBbits.ANSB9 = 0; // RB9 (SW7) disabled analog
    char control[]={0x38,0x38,0x38,0xe,0x6,0x1,0x40};//Configuration for the control, 0x1c
    //For Setting CGRAM is 0x40
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
    
    PORTBbits.RB15=0;//rs=0 -> Instruction mode for the control
    PORTDbits.RD5=0;//w=0
    configControl(control);
    /***Fish Movement***/
    fishMovement((char*)control);
}
