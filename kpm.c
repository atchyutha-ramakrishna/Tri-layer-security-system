#include"types.h"
#include<LPC21XX.h>
#include<string.h>
#include"defines.h"
#include"kpm_defines.h"
#include "lcd.h"
#include "delay.h"
#include "i2c_eeprom.h"
u8 kpmlut[4][4]={{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};
extern u8 r_pass[5];
u32 c;
extern u8 i_flag;
void init_kpm(void)
{
    WRITENIBBLE(IODIR1,ROWS_4,15);
}
u32 colscan(void)
{
    return(READNIBBLE(IOPIN1,COLS_4)<15)?0:1;
}
u32 rowcheck(void)
{
    u32 r;
	for(r=0;r<=3;r++)
	{
	    WRITENIBBLE(IOPIN1,ROWS_4,~(1<<r));
	    if(colscan()==0)
		{
	      break;
		}
	}
	WRITENIBBLE(IOPIN1,ROWS_4,0);
	return r;
}
u32 colcheck(void)
{
   u32 c;
   for(c=0;c<=3;c++)
   {
      if(READBIT(IOPIN1,col0+c)==0)
	  break;
   }
   	  return c;

}
u32 keyscan(void)
{
  u32 r,c,keyvalue;
  while(colscan());
  r=rowcheck();
  c=colcheck();
  keyvalue=kpmlut[r][c];
  return keyvalue;
}

void Readnum(u8 *a)
{
	u8 key;
	s32 r=0;
	CmdLCD(0xc0);
	while(1)
	{
		key=keyscan();
		while(colscan()==0);
		delay_ms(200);
		if((key>='0') && (key<='9'))
		{
			a[r]=key;
			CmdLCD(0xc0+r);
			CharLCD(a[r]);
			r++;	
		}
		else if(key=='D')
		{
			a[r]='\0';
			break;
		}
		else if(key=='C')
		{
			if(r>=0)
			{
				r--;
				CmdLCD(0xc0+r);
				CharLCD(' ');
				CmdLCD(0xc0+r);
			}
		}
	}
}

void change_pass(void)
{
	u8 pass[5],n_pass[5];
	CmdLCD(0x01);
	StrLCD("Enter old pass");
	Readnum(pass);
	if(strcmp((const char *)pass,(const char *)r_pass)==0)
	{
		CmdLCD(0x01);
		StrLCD("Correct pass..");
		delay_ms(1000);
		CmdLCD(0x01);
		StrLCD("Enter new pass");
		Readnum(pass);
		CmdLCD(0x01);
		StrLCD("Cnfrm new pass");
		Readnum(n_pass);
		if(strcmp((const char *)n_pass,(const char *)pass)==0)
		{
			i2c_eeprom_page_write(0x50,0x00,n_pass,4);
			CmdLCD(0x01);
			StrLCD("pass updated");
			delay_ms(1000);
			CmdLCD(0x01);
			i_flag=0;	
		}
		else
		{
			CmdLCD(0x01);
			StrLCD("new & cnfrm pass");
			CmdLCD(0xc0);
			StrLCD("are not matched");	
			delay_ms(1000);
			CmdLCD(0x01);		
		}
				
	}
	else
	{
		CmdLCD(0x01);
		StrLCD("Invalid old pass");
		delay_ms(1000);
		CmdLCD(0x01);
	}
}
