 ## try-layer security system videos and photos
https://drive.google.com/drive/folders/1bnvftaU0Rn5BqzUiF9pvZ_ST62k63F7D

TRI-LAYER SECURE ENTRY SYSTEM
AIM:
Security breaches, unauthorized access, and identity misuse have become major concerns in residential, commercial, and industrial environments. Traditional single-factor or password-based authentication methods are no longer sufficient to protect sensitive areas due to risks such as password guessing, card duplication, and system manipulation. To address these challenges, this project presents a Tri-Layer Secure Entry System, integrating three authentication stages: RFID-based user identification, password verification, and time-bound One-Time Password (OTP) confirmation.
BLOCK DIAGRAM:

<img width="638" height="475" alt="image" src="https://github.com/user-attachments/assets/6a5827fa-3520-4db4-a037-55961b762c3d" />

 
REQUIREMENTS:
HARDWARE REQUIREMENTS:
➢	LPC 2148
➢	GSM MODULE (M660A)
➢	16X2 LCD
➢	SWITCH
➢	4X4 MATRIX KEYPAD
➢	LED/BULB/DC MOTOR (WITH L293D CIRCUIT)
➢	AT24C256
➢	RFID READER
➢	RFID CARDS


SOFTWARE REQUIREMENTS:
➢	PROGRAMMING IN EMBEDDED C
➢	KEIL-C COMPILER
➢	FLASH MAGIC

Steps to be followed to complete your project:
➢	Create New Folder in your server save that folder with your project name
➢	Copy what you done files like lcd.c, lcd.h, delay,c, delay.h, uart.c, uart.h, keypad.c, keypad.h, i2c.c and i2c.h into project folder.
➢	Individually can check each and every module.
➢	First check lcd to display character constant, string constant and integer constant.
➢	Next check keypad peripheral by displaying key values on LCD.
➢	Next write n bytes into EEPROM and read that n number of bytes from EEPROM and display on LCD.
Note: Use BYTE WRITE and BYTE READ functions or PAGE WRITE and PAGE READ functions
➢	Next check external interrupt with the help of led support.
➢	Read inbuilt RTC VALUES and display it on LCD.
➢	Next check uart peripheral by transmitting character constant, string constant and receive
string constant using UART logic. (Note: use uart interrupt concept)
➢	Connect RFID READER D0 pin to USB to UART converter and test the working condition.
➢	Then write logic for reading the card number using UART0 interrupt and display it on LCD.
➢	Next connect gsm module to PC and check the gsm module working condition with the help of below mentioned AT commands.
AT
ATE0
AT+CMGF=1
AT+CMGS=”Mobile Number” then enter
Gsm will give the reply like ‘>’ then you have to type the message content and press ctrl+z from the keyboard to send the SMS. This process you have to do from the hyper terminal application which is available in windows XP.
➢	Next Check the GSM module working condition by developing gsm_init() function and send_sms() function.
Note: GSM interfacing program has to develop with the help of UART interrupts only.
➢	If above steps are completed, create new file with projectmain.c add all peripheral definition files and write below steps in projectmain.c file
➢	Initialize all necessary peripherals
➢	Initially your application program is waiting for user card. Once valid card is placed then asking for password from the user. If user entered the password from keypad, then read the current password from the EEPROM and validate that entered password. If entered password is correct, generate the OTP and send it to user registered mobile number. Then store the OTP sent time by using on-chip RTC information. Then user need to enter the OTP from the keypad with in specific time. If user is not entered the OTP within specific time or user entered the wrong OTP then application is terminated and again it is waiting for the password.
➢	Whenever interrupt is generated, user has to change the password by following proper password modification process. Once password is modified successfully then updated password is saved in to EEPROM specified memory locations. To change the specific user password, user need to place the card for password modification. For each user, fix the specific memory locations for easy implementation.
➢	If you’re getting this output then your project is completed.


