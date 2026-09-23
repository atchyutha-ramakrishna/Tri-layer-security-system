#include <LPC21xx.h>
#include "delay.h"
#include "dc_motor.h"					  

#define MOTOR_IN1 5
#define MOTOR_IN2 6
#define MOTOR_EN  7

void init_dc_motor(void)
{
    /* P0.22, P0.23, P0.24 as GPIO */
    PINSEL0&= ~((3 << 10) |
                 (3 << 12) |
                 (3 << 14));

    /* Motor pins as output */
    IODIR0 |= (1 << MOTOR_IN1);
    IODIR0 |= (1 << MOTOR_IN2);
    IODIR0 |= (1 << MOTOR_EN);

    dc_motor_stop();
}

void dc_motor_forward(void)
{
    IOCLR0 = (1 << MOTOR_IN2);
    IOSET0 = (1 << MOTOR_IN1);
    IOSET0 = (1 << MOTOR_EN);
}

void dc_motor_reverse(void)
{
    IOCLR0 = (1 << MOTOR_IN1);
    IOSET0 = (1 << MOTOR_IN2);
    IOSET0 = (1 << MOTOR_EN);
}

void dc_motor_stop(void)
{
    IOCLR0 = (1 << MOTOR_EN);										 
    IOCLR0 = (1 << MOTOR_IN1);
    IOCLR0 = (1 << MOTOR_IN2);
}

void dc_motor_unlock_action(void)
{
    dc_motor_forward();
    delay_ms(3000);
    dc_motor_stop();
}

