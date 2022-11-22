#include <sys/attribs.h>
#include <string.h>
#include <p32xxxx.h>
#include "Keypad.h"
#include "ScreenOperations.h"




char readKey(int* pressed)
{
    int xy, i=1;
    PORTCbits.RC2=1;
    PORTCbits.RC1=1;
    PORTCbits.RC4=1;
    PORTGbits.RG6=1;
    
    PORTCbits.RC2=0;
    xy=parser(i);
    if (xy!=0xff)
    {
        *pressed=1;
        return xy;
    }
    PORTCbits.RC2=1;
    i=2;
    PORTCbits.RC1=0;
    xy=parser(i);
    if (xy!=0xff)
    {
        *pressed=1;
        return xy;
    }
    PORTCbits.RC1=1;
    i=3;
    PORTCbits.RC4=0;
    xy=parser(i);
    if (xy!=0xff)
    {
        *pressed=1;
        return xy;
    }
    PORTCbits.RC4=1;
    i=4;
    PORTGbits.RG6=0;
    xy=parser(i);
    if (xy!=0xff)
    {
        *pressed=1;
        return xy;
    }
    PORTGbits.RG6=1;
}

void leaveKey()
{
    PORTCbits.RC2=0;
    PORTCbits.RC1=0;
    PORTCbits.RC4=0;
    PORTGbits.RG6=0;
    while(!PORTCbits.RC3);
    while(!PORTGbits.RG7);
    while(!PORTGbits.RG8);
    while(!PORTGbits.RG9);
}

int parser(int num)
{
    int j=1,flag=0;
    if(!PORTCbits.RC3)
    {
         flag=1;
         j=1;
    }
    else if (!PORTGbits.RG7)
    {
         flag=1;
          j=2;
    }
    else  if(!PORTGbits.RG8)
    {
        flag=1;
        j=3;
    }
    else if(!PORTGbits.RG9)
    {
        flag=1; 
        j=4;
    }
    if(flag==0)
        return(0xff);
    else
        return(j|(num<<4));

}

char actualValue(char num,char* numbers)
{
     for(int i=0;i<strlen(numbers);i+=2)
            if(numbers[i]==num)
                return numbers[i+1];
     return 0xff;
}