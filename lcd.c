//lcd.c
#include <lpc21xx.h>
#include "types.h"
#include "delay.h"
#include "defines.h"
#include "lcd.h"
#include "lcd_defines.h"

void WriteLCD(u8 byte)
{
  //select a operation
	IOCLR0=1<<LCD_RW;
	//write byte to lcd data pins
	WRITEBYTE(IOPIN0,LCD_DATA,byte);
	//provide high to lo pulse
	IOSET0=1<<LCD_EN;
	delay_us(1);
	IOCLR0=1<<LCD_EN;
	delay_ms(2);
}

void CmdLCD(u8 cmdByte)
{
    //set rs pin for cmd/inst reg
		IOCLR0=1<<LCD_RS;
		//write cmd byte to cmd reg
		WriteLCD(cmdByte);
}

void Init_LCD(void)
{
   //cfg datapins,rs,rw and en pins as gpio output pins
	 IODIR0|=((1<<LCD_RS)|(1<<LCD_RW)|(1<<LCD_EN));
	 IODIR0|=0XFF<<LCD_DATA;
	 //power on delay
	 delay_ms(15);
	 CmdLCD(0x30);
	 delay_ms(4);
	 delay_us(100);
	 CmdLCD(0x30);
	 delay_us(100);
	 CmdLCD(0x30);
	 CmdLCD(MODE_8BIT_2LINE);
	 CmdLCD(DSP_ON_CUR_ON);
	 CmdLCD(CLEAR_LCD);
 	 CmdLCD(SHIFT_CUR_RIGHT);
	 
}

void CharLCD(u8 asciiVal)
{
   //sel data reg
	 IOSET0=1<<LCD_RS;
	 //write ascii value via dat reg TO DDRAM
	 WriteLCD(asciiVal);

}

void StrLCD(s8 *Str)
{
  while(*Str)
	   CharLCD(*Str++);
}

void U32LCD(u32 n)
{
    s32 i=0;
		u8 a[10];
		if(n==0)
		{
		  CharLCD('0');
		}
		else
		{
		  while(n>0)
			{
			  a[i++]=(n%10)+48;
				n/=10;
			}
			for(--i;i>=0;i--)
			  CharLCD(a[i]);
		}
}

void S32LCD(s32 num)
{
  if(num<0)
	{
	   CharLCD('-');
		 num=-num;
	}
	U32LCD(num);
}

void BuildCGRAM(u8 *p,u8 nBytes)
{
  u32 i;
  CmdLCD(GOTO_CGRAM_START);
  IOSET0=1<<LCD_RS;
  for(i=0;i<nBytes;i++)
  {
    WriteLCD(p[i]);
  }
  CmdLCD(GOTO_LINE1_POS0);
}
