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
*/

#include <project.h>

#define DEBUG 1
char debug_data_buf[64] = "";

const int RC_TIMER_PERIOD = 500000 - 1;
const int RC_TIMER_MAX_VALUE = 46000;
const int RC_TIMER_MID_VALUE = 34000;
const int RC_TIMER_MIN_VALUE = 24000;
const int RC_TIMER_MID_TOLERANCE = 600;

static volatile uint32 rc_timer_count;
static uint8 rc_timer_int_flag = 0;
uint32 last_rc_timer_count = 0;

static volatile uint32 rc_connection_timer_count;
static uint8 rc_connection_timer_int_flag = 0;

const int MAX_MOTOR_PWM_VALUE = 240;
const int MIN_MOTOR_PWM_VALUE = 0;

int16 motor_pwm = 0;

CY_ISR(RC_Connection_ISR_Handler)
{
    rc_connection_timer_int_flag = RC_Connection_Timer_ReadStatusRegister();	//	Read and clear interrupt status      
    if(rc_connection_timer_int_flag & RC_Connection_Timer_STATUS_CAPTURE)
        rc_connection_timer_count = RC_TIMER_PERIOD - RC_Connection_Timer_ReadCapture();    
    if(rc_connection_timer_int_flag & RC_Connection_Timer_STATUS_TC)			//	Did we overflow??
        rc_connection_timer_count = 0;
   
    RC_Connection_Timer_Stop();
    RC_Connection_Timer_WriteCounter(RC_TIMER_PERIOD);
    USBUART_PutString("FCK...");
}

CY_ISR(RC_Ch2_Timer_ISR_Handler)
{
    rc_timer_int_flag = RC_Ch2_Timer_ReadStatusRegister();	//	Read and clear interrupt status      
    if(rc_timer_int_flag & RC_Ch2_Timer_STATUS_CAPTURE)
        rc_timer_count = RC_TIMER_PERIOD - RC_Ch2_Timer_ReadCapture();    
    if(rc_timer_int_flag & RC_Ch2_Timer_STATUS_TC)			//	Did we overflow??
        rc_timer_count = 0;
    
    RC_Ch2_Timer_Stop();                         // stop and setup for next round
    RC_Ch2_Timer_WriteCounter(RC_TIMER_PERIOD);		//	Rewrite counter for next cycle
}

void get_rc_value();
void run_motor();

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    USBUART_Start(0, USBUART_5V_OPERATION);
    
    while (USBUART_GetConfiguration() == 0) {}
    USBUART_PutString("Init...");
       
    Motor_A_PWM_Start();
    
    RC_Connection_Timer_Interrupt_StartEx(RC_Connection_ISR_Handler);
    RC_Connection_Timer_Start();
    
    RC_Ch2_Timer_Interrupt_StartEx(RC_Ch2_Timer_ISR_Handler);
    RC_Ch2_Timer_Start();
    
    int compare_value = 127;
    
    int count_value = 0;
    int a = 0;
    int b = 0;
    
    for(;;)
    {
        /* Place your application code here. */
        /*Motor_A_PWM_WriteCompare1(compare_value);
        Motor_A_PWM_WriteCompare2(0);
        CyDelay(1000);
        compare_value = 0;
        Motor_A_PWM_WriteCompare1(compare_value);
        Motor_A_PWM_WriteCompare2(0);
        CyDelay(1000);
        compare_value = 64;
        sprintf(data_buf, "Forward PWM: %d\r\n", compare_value);
        USBUART_PutString(data_buf);
        
        Motor_A_PWM_WriteCompare1(0);
        Motor_A_PWM_WriteCompare2(compare_value);
        CyDelay(1000);
        compare_value = 0;
        Motor_A_PWM_WriteCompare1(0);
        Motor_A_PWM_WriteCompare2(compare_value);
        CyDelay(1000);
        compare_value = 127;
        
        sprintf(data_buf, "Reverse PWM: %d\r\n", compare_value);
        USBUART_PutString(data_buf);*/
        
        get_rc_value();
        run_motor();
        
        //sprintf(debug_data_buf, "RC: %ld --> Motor: %d \r\n", rc_timer_count, motor_pwm);
        //USBUART_PutString(debug_data_buf);
    }
}

void get_rc_value() {
    if (rc_timer_int_flag) {
        rc_timer_count = (rc_timer_count < RC_TIMER_MIN_VALUE || rc_timer_count > RC_TIMER_MAX_VALUE) ? 
                            last_rc_timer_count : rc_timer_count;
        if (abs(RC_TIMER_MID_VALUE - rc_timer_count) < RC_TIMER_MID_TOLERANCE) {
            motor_pwm = MIN_MOTOR_PWM_VALUE;
        } else {
            const int RC_TIMER_AMPLITUDE = rc_timer_count > RC_TIMER_MID_VALUE ? RC_TIMER_MAX_VALUE - RC_TIMER_MID_VALUE : RC_TIMER_MID_VALUE - RC_TIMER_MIN_VALUE;
            motor_pwm = (MAX_MOTOR_PWM_VALUE - MIN_MOTOR_PWM_VALUE) * ((int32) rc_timer_count - RC_TIMER_MID_VALUE)/RC_TIMER_AMPLITUDE;
        }
        
        last_rc_timer_count = rc_timer_count;
        rc_timer_int_flag = 0;
        RC_Ch2_Timer_Start();
        
        if (motor_pwm != 0) {
            sprintf(debug_data_buf, "RC: %ld --> Motor: %d \r\n", rc_timer_count, motor_pwm);
            USBUART_PutString(debug_data_buf);
        }
    } else {
       //motor_pwm = MIN_MOTOR_PWM_VALUE;
    }
}

void run_motor() {
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
