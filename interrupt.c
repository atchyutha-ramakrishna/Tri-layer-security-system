#include <lpc21xx.h>
#include "pin_function_defines.h"
#include "defines.h"
#include "types.h"

u8 i_flag=0;

void eint0_isr(void) __irq
{
	i_flag=1;
	SCLRBIT(EXTINT,1);//clear flag
	VICVectAddr=0;//dummy write;
}	

void Enable_EINT0(void)
{

	PINSEL0|=0x20000000;
	SSETBIT(VICIntEnable,15);
	VICVectCntl2=0x20|15;
	VICVectAddr2=(unsigned)eint0_isr;
	
	//Enable EINT0 
	//SSETBIT(EXTINT,0) //default
	
	//EINT0 as EDGE_TRIG
  SETBIT(EXTMODE,1);
	//EINT0 as REDGE
  //SETBIT(EXTPOLAR,1);	
}	
