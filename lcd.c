#include<LPC21xx.h>
#include "defines.h"
#include "delay.h"
#include "lcd_define.h"
void WriteLCD(unsigned char anyByte){

IOCLR0=1<<LCD_RW; // SELECTING WRITE OPERATIONS
WRITEBYTE(IOPIN1,LCD_DATA,anyByte);// Write data/command byte to LCD data lines

IOSET0=1<<LCD_EN;
delay_us(1);
IOCLR0=1<<LCD_EN;// FOR HIGHTO LOW PULSE to EN

delay_us(37);
}
void CmdLCD(unsigned char cmd){
    IOCLR0=1<<LCD_RS; //SELECTING 
    //WRITE CMD TO RG
    WriteLCD(cmd);
    delay_ms(2);

}
void CharLCD(unsigned char asciiVal){
    // select data register
    IOSET0=1<<LCD_RS;
    //write ascii vio data reg to ddram
    WriteLCD(asciiVal);

}

void Init_LCD (void) {
    // Configure P0.8–P0.15 as data output
    // Configure P0.16 (RS), P0.17 (RW), P0.18 (EN) as control output
    IODIR0|=(1 << LCD_RS) | (1 << LCD_RW) | (1 << LCD_EN);
	  IODIR1|=(0xFFU << LCD_DATA) ;
    delay_ms(15);         // Wait for LCD power-on stabilization

    // Reset sequence as per HD44780 datasheet
    CmdLCD(0x30);         // Function Set (8-bit)
    delay_ms(5);  
    CmdLCD(0x30); 
    delay_us(100);
    CmdLCD(0x30); 

    // Now send actual initialization commands
    CmdLCD(MODE_8BIT_2LINE);     // 8-bit, 2-line display
    CmdLCD(DSP_ON_CUR_ON);    // Display ON, Cursor ON, Blinking ON
    CmdLCD(CLEAR_LCD);           // Clear display
    CmdLCD(SHIFT_CUR_RIGHT);     // Set cursor move direction
}

void Str_LCD(char*p){
    while(*p){
        CharLCD((*p++));
    }
}

void u32lcd(unsigned int num){
    unsigned char a[10];
    int i = 0;

    if(num == 0){
        CharLCD('0');
        return;
    }

    while(num > 0){
        a[i++] = (num % 10) + '0';
        num /= 10;
    }

    while(i--){
        CharLCD(a[i]);
    }
}

void StrLCD(int snum){
    if(snum<0){
        CharLCD('-');
        snum=-snum;
    }
    u32lcd(snum);

}

void F32LCD(float fNum, unsigned char nDP){
	unsigned char i;
    unsigned int intPart;
    if(fNum < 0){
        CharLCD('-');
        fNum = -fNum;
    }
	 intPart = (unsigned int)fNum;
    u32lcd(intPart);

    CharLCD('.');

    fNum = fNum - intPart;

    for(i = 0; i < nDP; i++){
        fNum *= 10;
        intPart = (unsigned int)fNum;
        CharLCD(intPart + '0');
        fNum -= intPart;
    }
}

void BuildCGRAM(unsigned char *p,unsigned char nBytes){
    unsigned int i;
    //goto CGRAM START
    CmdLCD(GOTO_CGRAM_START);
    //select data register
    IOSET0=1<<LCD_RS;
    for(i=0;i<nBytes;i++){
        WriteLCD(p[i]);
    }
    // return back to ddram
    CmdLCD(GOTO_LINE1_POS0);

}


