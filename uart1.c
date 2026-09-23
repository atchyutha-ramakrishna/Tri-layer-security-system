#include <LPC21xx.H>  /* LPC21xx definitions         */
#include <string.h>

//#define UART_INT_ENABLE 1

char card[10],dummy1;
unsigned char j=0,ch1,r_flag;

void UART1_isr(void) __irq
{
  if((U1IIR & 0x04)) //check if receive interrupt
  {
		ch1= U1RBR;	/* Read to Clear Receive Interrupt */
		if(ch1 == 0x02)
		{
			r_flag=1;
		}
		else if((ch1!=0x03)&&(r_flag==1))
		{
			card[j++] = ch1;
		} 
		else
		{
			card[j] = '\0';
			j=0;
			r_flag = 2;
		}
  }
  else
  {
      dummy1=U1IIR; //Read to Clear transmit interrupt
  
  }
   VICVectAddr = 0; /* dummy write */
}

void InitUART1 (void) /* Initialize Serial Interface       */ 
{  
            		
  PINSEL0|=0x00050000; /* Enable RxD0 and TxD0              */
  U1LCR = 0x83;         /* 8 bits, no Parity, 1 Stop bit     */
  U1DLL = 97;           /* 9600 Baud Rate @ CCLK/4 VPB Clock  */
  U1LCR = 0x03;         /* DLAB = 0  */
  
  //#if UART_INT_ENABLE > 0

  VICIntSelect = 0x00000000; // IRQ
  VICVectAddr1 = (unsigned)UART1_isr;
  VICVectCntl1 = 0x20 | 7; /* UART0 Interrupt */
  VICIntEnable = 1 << 7;   /* Enable UART0 Interrupt */
 
 // U0IIR = 0xc0;
 // U0FCR = 0xc7;
  U1IER = 0x03;       /* Enable UART0 RX and THRE Interrupts */   
						
}

void UART1_Tx(char ch)  /* Write character to Serial Port    */  
{ 
  while (!(U1LSR & 0x20));
  U1THR = ch;                
}

char UART1_Rx(void)    /* Read character from Serial Port   */
{                     
  while (!(U1LSR & 0x01));
  return (U1RBR);
}

void UART1_Str(char *s)
{
   while(*s)
       UART1_Tx(*s++);
}

void UART1_Int(unsigned int n)
{
  unsigned char a[10]={0,0,0,0,0,0,0,0,0,0};
  int i=0;
  if(n==0)
  {
    UART1_Tx('0');
	return;
  }
  else
  {
     while(n>0)
	 {
	   a[i++]=(n%10)+48;
	   n=n/10;
	 }
	 --i;
	 for(;i>=0;i--)
	 {
	   UART1_Tx(a[i]);
	 }
   }
}
