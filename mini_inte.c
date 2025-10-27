#include <LPC21xx.h>
#include "mini_inte.h"
#include "LCD.h"
#include "lcd.h"
#include "delay.h"
#include "rtc.h"
#include "kpm.h"
#include "Pin_connect_block.h"
#include "type.h"
#include "lcd_define.h"
extern volatile u32 eint0_flag;
char inp(){
char key='\0';
do{
key=keyScan();
while(ColScan==0);
delay_ms(400);
CharLCD(key);
}while(key=='\0');

return key;
}

u32 readnum(){
  int num=0,key;
	while(1){
		key='\0';
    key=keyScan();
    while(ColScan()==0);
		delay_ms(200);
    
    if(key>='0' && key<='9'){
			CharLCD(key);
      num=num*10+(key-'0');
    }
    
     if(key=='='){
      break;
    }
  }
  return num;
}
int  check_val(u32 *x,int low,int high){
  if(!(*x>=low && *x<=high)){
    return 0;
  }
  return 1;
}
void genral(){
      CmdLCD(CLEAR_LCD);
      Str_LCD("enter val again");
      delay_ms(700);
}

void  handleMenu(){
char opt1='\0';
unsigned int menu=1; // for continues running default menu is 0
unsigned int val;
while(menu!=0){
     CmdLCD(CLEAR_LCD);
  Str_LCD("1.HR 2.MIN 3.SEC");
  CmdLCD(GOTO_LINE2_POS0);
  Str_LCD("4.DY 5.DM 6.yr");
	opt1=inp();
  delay_ms(1500);
	
if(opt1!='\0'){
	CmdLCD(CLEAR_LCD);
  Str_LCD("ent value:");
}
	switch(opt1){
		case '1': 
    val=readnum();
    if(!check_val(&val,0,23)){
			genral();
      break;
    }
    SetRTCHour(val);
    break;
    case '2': 
    val=readnum();
    if(!check_val(&val,0,59)){
			genral();
      break;
    }
    SetRTCMin(val);
    break;
    case '3': 
    val=readnum();
    if(!check_val(&val,0,59)){
			genral();
      break;
    }
    SetRTCSec(val);
    break;
    case '4': 
    val=readnum();
    if(!check_val(&val,1,31)){
			genral();
      break;
    }
    SetRTCDate(val);
    break;
    case '5': 
    val=readnum();
    if(!check_val(&val,1,12)){
			genral();
      break;
    }
    SetRTCMonth(val);
    break;
    case '6': 
    val=readnum();
    if(!check_val(&val,1000,9999)){
			genral();
      break;
    }
    SetRTCYear(val);
    break;
    default :
			CmdLCD(CLEAR_LCD);
    Str_LCD("exiting");
		menu=0;// menu 0 for existing
    delay_ms(500);    
	
  }
}
}



void eint0qw(void)__irq
{
    eint0_flag = 1;      
	
  // set flag for main
    EXTINT = 1<<0;         // clear EINT0
    VICVectAddr = 0;       // Acknowledge
}
//Enable EINT0
void enable_eint0(void)
{
    // Configure P0.16 as EINT0
    PINSEL1 |= (1<<0);
    PINSEL1 &= ~(1<<1);

    VICIntEnable = 1<<EINT0;                  // enable EINT0
    VICVectCntl0 = (1<<5) | EINT0;            // slot enable
    VICVectAddr0 = (unsigned int)eint0qw;   // ISR address

    EXTMODE = 1<<0;        // edge triggered
    //EXTPOLAR &= ~(1<<0);   // falling edge
}























