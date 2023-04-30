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
*/

#include <project.h>

const int RC_TIMER_PERIOD = 194967296 - 1;
static volatile uint32 rc_timer_count;
static int rc_timer_int_flag = 0;

CY_ISR(RC_Timer_ISR_Handler)
{
    /*uint16 capture = RC_Timer_ReadCapture(); 
    char data_buf[64] = "";
    sprintf(data_buf, "Timer Capture: %d\r\n", capture);
    USBUART_PutString(data_buf);*/
    
    rc_timer_int_flag = RC_Timer_ReadStatusRegister();	//	Read and clear interrupt status
    //onboard_led_Write(~onboard_led_Read());					        
    if(rc_timer_int_flag & RC_Timer_STATUS_CAPTURE)
        rc_timer_count = RC_TIMER_PERIOD - RC_Timer_ReadCapture();    
    if(rc_timer_int_flag & RC_Timer_STATUS_TC)			//	Did we overflow??
        rc_timer_count = 0;
    RC_Timer_Stop();                         // stop and setup for next round
    RC_Timer_WriteCounter(RC_TIMER_PERIOD);		//	Rewrite counter for next cycle
}

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    USBUART_Start(0, USBUART_5V_OPERATION);
    
    while (USBUART_GetConfiguration() == 0) {}
       
    Motor_A_PWM_Start();
    RC_PWM_Start();
    RC_Counter_Start();
    
    RC_Timer_Interrupt_StartEx(RC_Timer_ISR_Handler);
    RC_Timer_Start();
    
    int compare_value = 127;
    char data_buf[64] = "";
    
    int count_value = 0;
    
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
        
        //CyDelay(200);
        //count_value = RC_Counter_ReadCounter();
        //count_value = RC_Timer_ReadCounter();
        if (rc_timer_int_flag) {
        sprintf(data_buf,"RC %s Time= %ld\r\n",(rc_timer_int_flag & RC_Timer_STATUS_CAPTURE)?"Capture":"Overflow", rc_timer_count);
        //sprintf(data_buf, "RC Duty Cycle: %d\r\n", count_value);
        USBUART_PutString(data_buf);
        rc_timer_int_flag = 0;
        RC_Timer_Start();
        }
        
        CyDelay(10);
        int value = RC_Count_2_Read();
        //sprintf(data_buf, "Val: %d\r\n", value);
        //USBUART_PutString(data_buf);
        //LED_Write();
    }
}

/* [] END OF FILE */
