#include <lpc21xx.h>
#include "lcd.h"
#include "delay.h"
#define row 16
#define col 20
const unsigned char arr[4][4]=
{
	{'7','8','9','/'},
	{'4','5','6','*'},
	{'1','2','3','-'},
	{'c','0','=','+'},
};
void Init_kpm(void)
{
	 IODIR1|=15<<row;
}
unsigned int ColScan(void)
{
	if(((IOPIN1>>col)&15)<15)
		return 0;
	else
		return 1;
}
unsigned int row_cheak(void)
{
	unsigned int i;
	for(i=0;i<4;i++)
	{
		IOPIN1=((IOPIN1&~(15<<row))|(~(1<<i)<<row));
		if(ColScan()==0)
			break;
	}
			IOPIN1=((IOPIN1&~(15<<row))|(0<<row));
	return i;
}
unsigned int col_cheak(void)
{
	unsigned int i;
	for(i=0;i<4;i++)
	{
		if(((IOPIN1>>(col+i))&1)==0)
			break;
	}
	return i;
}

unsigned char keyScan(void)
{
	unsigned char rno,cno,keyv;
	while(ColScan());
	rno=row_cheak();
	cno=col_cheak();
	keyv=arr[rno][cno];
   while(!ColScan());
	return keyv;
}
void Read_Number(unsigned int*num,char *key)
{
	
	*num=0;
	while(1){
     while(ColScan());
	  *key=keyScan();
	if(*key>='0'&&*key<='9')
	{
		*num=(*num*10+(*key-48));
	}
	else
		break;
 }
}
	

