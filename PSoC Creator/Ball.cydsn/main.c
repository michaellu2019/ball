/* ========================================
 *
 * Copyright YOUR COMPANY, THE YEAR
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF your company.
 *
 * ========================================
*/
/*
 * References:
 * https://iotexpert.com/psoc-5-timer-circuit-debug/
 * https://youtu.be/nJs6Q4ekVjw
 * https://socmaker.com/?p=296
 * https://github.com/embeddedprogrammer/psoc-pid-controller
 * https://www.hackster.io/hima/mpu6050-psoc6-interface-7c505e
*/

#include "project.h"
#include "milliseconds.h"
#include "rc_ch.h"

#define DEBUG 1
char debug_data_buf[64] = "";

/*
 * Channel 1: Right Joystick X
 * Channel 2: Right Joystick Y
 * Channel 3: Left Joystick Y
 * Channel 4: Left Joystick X
 * Channel 5: Right Knob
 * Channel 6: Left Knob
*/

void run_motor();

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    USBUART_Start(0, USBUART_5V_OPERATION);
    
    while (USBUART_GetConfiguration() == 0) {}
    USBUART_PutString("Init...");
    
    init_milliseconds();
       
    Motor_A_PWM_Start();
    
    init_rc_channels();
    
    RC_CH rc_ch1;
    construct_rc_ch(&rc_ch1, 1);
    RC_CH rc_ch2;
    construct_rc_ch(&rc_ch2, 2);
    RC_CH rc_ch3;
    construct_rc_ch(&rc_ch3, 3);
    RC_CH rc_ch4;
    construct_rc_ch(&rc_ch4, 4);
    
    for(;;)
    {
        get_rc_ch_value(&rc_ch1);
        //get_rc_ch_value(&rc_ch2);
        //get_rc_ch_value(&rc_ch3);
        //get_rc_ch_value(&rc_ch4);
        
        /*if (rc_ch2.connected && rc_ch2.value != 0) {
            run_motor(rc_ch2.value);
        } else {
            run_motor(0);
        }*/
    }
}

void run_motor(motor_pwm) {
    if (motor_pwm < 0) {
        Motor_A_PWM_WriteCompare1(abs(motor_pwm));
        Motor_A_PWM_WriteCompare2(0);
    } else if (motor_pwm > 0) {
        Motor_A_PWM_WriteCompare1(0);
        Motor_A_PWM_WriteCompare2(motor_pwm);
    } else {
        Motor_A_PWM_WriteCompare1(0);
        Motor_A_PWM_WriteCompare2(0);
    }
}

/* [] END OF FILE */
