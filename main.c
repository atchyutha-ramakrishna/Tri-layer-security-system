//pro_main.c
#include <LPC21xx.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"
#include "delay.h"
#include "lcd.h"
#include "kpm.h"
#include "uart0.h"
#include "uart1.h"
#include "defines.h"
#include "eint.h"
#include "i2c.h"
#include "i2c_eeprom.h"
#include "dc_motor.h"

#define LED 5
#define DOOR_LOCK_SW 4

extern char card[10];
extern unsigned char j, r_flag;

extern char buff[50];
extern unsigned char i;

extern u8 i_flag;

u8 r_pass[5];
u8 c_pass[5];
u8 d_pass[5] = "1234";

u8 r_card[10];

u8 otp[10];
u8 c_otp[5];

u8 phno[] = "9000085307";
u32 num = 0;
static u32 otp_cnt=0,last_otp=0;
void GSM_init(void);
void send_sms(char *, u32);

int main()
{
    int r;
//    int c;
u32 seed;
    int l;
    int min=MIN,sec=SEC;

    /* LED configuration */
    IODIR0 |= 15 << LED;
    IOPIN0 = (IOPIN0 & ~(15 << LED)) | (0x0F << LED);

    /* Initialize peripherals */
    InitUART0();
    InitUART1();

    init_i2c();

    Init_LCD();

    init_kpm();

    Enable_EINT0();

    /* Initialize DC motor */
    init_dc_motor();

    /* Welcome message */
    CmdLCD(0x80);
    StrLCD("TRI-LAYER");
    CmdLCD(0xC0);
  //  StrLCD("SECURITY SYSTEM");

    delay_ms(1000);

    /* Initialize GSM */
    GSM_init();

    delay_ms(1000);

    CmdLCD(0x01);
    i2c_eeprom_page_write(0x50, 0x00, d_pass, 4);
    //i2c_eeprom_page_write(0x50, 0x10, "12546806", 8);
	  i2c_eeprom_page_write(0x50, 0x10, "12623314", 8);

    while(1)
    {
        i2c_eeprom_seq_read(0x50, 0x00, r_pass, 4);
        r_pass[4] = '\0';

        i2c_eeprom_seq_read(0x50, 0x10, r_card, 8);
        r_card[8] = '\0';

        dc_motor_stop();

        IOPIN0 =
            (IOPIN0 & ~(15 << LED)) |
            (0x0F << LED);

        /*
         * WAIT FOR RFID CARD
         */
        CmdLCD(0x01);
        CmdLCD(0x80);
        StrLCD("Waiting for");
        CmdLCD(0xC0);
        StrLCD("RFID CARD");

        i = 0;
        j = 0;
        r_flag = 0;

        memset(card, '\0', 10);
        memset(buff, '\0', 50);

        while(r_flag < 2)
        {
            if(i_flag)
            {
                change_pass();
                r_flag = 2;
            }
        }
        if(r_flag == 2)
        {
            r_flag = 0;

            CmdLCD(0x01);
            StrLCD("Validating");
			delay_ms(1000);
            for(r = 11; r < 16; r++)
            {
                CmdLCD(0x80 + r);
                CharLCD('.');
                delay_ms(50);
            }
            if(strcmp((const char *)card,
                      (const char *)r_card) == 0)
            {
                CmdLCD(0x01);
                StrLCD("Valid card");
                delay_ms(1000);
                CmdLCD(0x01);
                StrLCD("Enter the pass");
                Readnum(c_pass);

                CmdLCD(0x01);
                StrLCD("Validating");

                for(r = 11; r < 16; r++)
                {
                    CmdLCD(0x80 + r);
                    CharLCD('.');
                    delay_ms(50);
                }
                if(strcmp((const char *)c_pass,
                          (const char *)r_pass) == 0)
                {
                    CmdLCD(0x01);
                    StrLCD("Valid password");
                    delay_ms(1000);
                    CmdLCD(0x01);
                    StrLCD("OTP Generating");
                    delay_ms(1000);
					 otp_cnt++;
					 seed=(min*60)+sec;
					 seed=seed+(otp_cnt+137);
                    num = 0;

					for(l=0;l<4;l++)
					{
                        seed = (seed*37)+17;
						num=(num*10)+((seed/10)%10);
					}
						if(num<1000)
						num+=1000;
						if(num==last_otp)
						{
						num=(num+1)%10000;
						if(num<1000)
						num+=1000;

						last_otp=num;
						}
                    send_sms((char *)phno, num);
                    CmdLCD(0x01);
                    StrLCD("Enter the OTP");

                    Readnum(c_otp);

                    CmdLCD(0x01);
                    StrLCD("Validating");

                    for(r = 11; r < 16; r++)
                    {
                        CmdLCD(0x80 + r);
                        CharLCD('.');
                        delay_ms(50);
                    }
                    /*
                     * OTP VALIDATION
                     */
                    if(num == atoi((const char *)c_otp))
                    {
                        CmdLCD(0x01);
                        StrLCD("Valid OTP");
                        delay_ms(1000); 
                        CmdLCD(0x01);
                        StrLCD("DOOR OPENING");
						 delay_ms(1000);
						 dc_motor_forward(); 
						 //dc_motor_reverse();

                       delay_ms(5000);
					    dc_motor_stop();

                        CmdLCD(0x01);
                       StrLCD("DOOR OPENED");
                        delay_ms(1000);
                        CmdLCD(0x01);
                        StrLCD("DOOR CLOSING");
						delay_ms(1000);
						//dc_motor_forward(); 
						dc_motor_reverse();                
                        delay_ms(3000);

                        dc_motor_stop();

                        //CmdLCD(0x01);
                        //StrLCD("DOOR CLOSED");
                        //delay_ms(1000);
                    
                        //CmdLCD(0x01);
                    }
                   else
                    {
                        CmdLCD(0x01);
                        StrLCD("Invalid OTP");
                        delay_ms(1000);
                        CmdLCD(0x01);
						StrLCD("DOOR LOCKED");
                        dc_motor_stop();
                    }
                } 
                else
                {
                    CmdLCD(0x01);
                    StrLCD("Invalid pass");
                    delay_ms(1000);
                    CmdLCD(0x01);

                    //dc_motor_stop();
                }
            }
            else
            {
                CmdLCD(0x01);
                StrLCD("Invalid card");
                delay_ms(1000);
                CmdLCD(0x01);
				//dc_motor_reverse();
              //  dc_motor_stop();
            }
        }

}    
}
