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
#include "project.h"
#include "milliseconds.h"
#include "rc_ch.h"

const int RC_TIMER_PERIOD = 500000 - 1;
const int RC_TIMER_MAX_VALUE = 45000;
const int RC_TIMER_MID_VALUE = 33000;
const int RC_TIMER_MIN_VALUE = 24000;
const int RC_TIMER_MID_TOLERANCE = 600;
const int RC_CONNECTION_TIMEOUT = 50;

const int RC_MAX_VALUE = 240;
const int RC_MIN_VALUE = 0;

static volatile uint32 isr_rc_timer_count[NUM_RC_CHANNELS];
static uint8 isr_rc_timer_int_flag[NUM_RC_CHANNELS];

CY_ISR(RC_Ch1_Timer_ISR_Handler)
{
    rc_ch_timer_isr_handler(1);
}

CY_ISR(RC_Ch2_Timer_ISR_Handler)
{
    rc_ch_timer_isr_handler(2);
}

CY_ISR(RC_Ch3_Timer_ISR_Handler)
{
    rc_ch_timer_isr_handler(3);
}

CY_ISR(RC_Ch4_Timer_ISR_Handler)
{
    rc_ch_timer_isr_handler(4);
}

void rc_ch_timer_isr_handler(uint8 ch_number) {
    isr_rc_timer_int_flag[ch_number - 1] = ch_number == 1 ? RC_Ch1_Timer_ReadStatusRegister() : 
                                           ch_number == 2 ? RC_Ch2_Timer_ReadStatusRegister() : 
                                           ch_number == 3 ? RC_Ch3_Timer_ReadStatusRegister() : 
                                                            RC_Ch4_Timer_ReadStatusRegister(); 
    const uint8 rc_ch_timer_status_capture = ch_number == 1 ? RC_Ch1_Timer_STATUS_CAPTURE : 
                                             ch_number == 2 ? RC_Ch2_Timer_STATUS_CAPTURE : 
                                             ch_number == 3 ? RC_Ch3_Timer_STATUS_CAPTURE : 
                                                              RC_Ch4_Timer_STATUS_CAPTURE;
    const uint8 rc_ch_timer_status_tc = ch_number == 1 ? RC_Ch1_Timer_STATUS_TC : 
                                        ch_number == 2 ? RC_Ch2_Timer_STATUS_TC : 
                                        ch_number == 3 ? RC_Ch3_Timer_STATUS_TC : 
                                                         RC_Ch4_Timer_STATUS_TC;
                                                        
    if (isr_rc_timer_int_flag[ch_number - 1] & rc_ch_timer_status_capture)
        isr_rc_timer_count[ch_number - 1] = RC_TIMER_PERIOD - RC_Ch2_Timer_ReadCapture();    
    if (isr_rc_timer_int_flag[ch_number - 1] & rc_ch_timer_status_tc)
        isr_rc_timer_count[ch_number - 1] = 0;
    
    if (ch_number == 1) {
        RC_Ch1_Timer_Stop();
        RC_Ch1_Timer_WriteCounter(RC_TIMER_PERIOD);
    } else if (ch_number == 2) {
        RC_Ch2_Timer_Stop();
        RC_Ch2_Timer_WriteCounter(RC_TIMER_PERIOD);
    } else if (ch_number == 3) {
        RC_Ch3_Timer_Stop();
        RC_Ch3_Timer_WriteCounter(RC_TIMER_PERIOD);
    } else if (ch_number == 4) {
        RC_Ch4_Timer_Stop();
        RC_Ch4_Timer_WriteCounter(RC_TIMER_PERIOD);
    }
}

void init_rc_channels() {
    RC_Ch1_Timer_Interrupt_StartEx(RC_Ch1_Timer_ISR_Handler);
    RC_Ch1_Timer_Start();
    RC_Ch2_Timer_Interrupt_StartEx(RC_Ch2_Timer_ISR_Handler);
    RC_Ch2_Timer_Start();
    RC_Ch3_Timer_Interrupt_StartEx(RC_Ch3_Timer_ISR_Handler);
    RC_Ch3_Timer_Start();
    RC_Ch4_Timer_Interrupt_StartEx(RC_Ch4_Timer_ISR_Handler);
    RC_Ch4_Timer_Start();
}

void construct_rc_ch(RC_CH* rc_ch, uint8 ch_number) {
    rc_ch->ch_number = ch_number;
    rc_ch->connected = 0;
    rc_ch->value = 0;
    rc_ch->last_connected = 0;
    rc_ch->last_timer_count = 0;
}

void get_rc_ch_value(RC_CH* rc_ch) {
    int16 ch_value = 0;
    uint8 rc_ch_read_value = rc_ch->ch_number == 1 ? RC_Ch1_Read() :
                             rc_ch->ch_number == 2 ? RC_Ch2_Read() :
                             rc_ch->ch_number == 3 ? RC_Ch3_Read() :
                                                     RC_Ch4_Read();
    if (rc_ch_read_value) {
        rc_ch->last_connected = MILLISECONDS;
        rc_ch->connected = 1;
    } else {
        if (MILLISECONDS - rc_ch->last_connected > RC_CONNECTION_TIMEOUT) {
            rc_ch->connected = 0;
        }
    }
    
    if (rc_ch->connected && isr_rc_timer_int_flag[rc_ch->ch_number - 1]) {
        uint32 rc_ch_timer_count = (isr_rc_timer_count[rc_ch->ch_number - 1] < RC_TIMER_MIN_VALUE || 
                                    isr_rc_timer_count[rc_ch->ch_number - 1] > RC_TIMER_MAX_VALUE) ? 
                                    rc_ch->last_timer_count : isr_rc_timer_count[rc_ch->ch_number - 1];
        if (abs(RC_TIMER_MID_VALUE - rc_ch_timer_count) < RC_TIMER_MID_TOLERANCE) {
            rc_ch->value = RC_MIN_VALUE;
        } else {
            const int RC_TIMER_AMPLITUDE = rc_ch_timer_count > RC_TIMER_MID_VALUE ? RC_TIMER_MAX_VALUE - RC_TIMER_MID_VALUE : RC_TIMER_MID_VALUE - RC_TIMER_MIN_VALUE;
            rc_ch->value = (RC_MAX_VALUE - RC_MIN_VALUE) * ((int32) rc_ch_timer_count - RC_TIMER_MID_VALUE)/RC_TIMER_AMPLITUDE;
            //USBUART_PutString("fc\r\n");
        }
        
        rc_ch->last_timer_count = rc_ch_timer_count;
        isr_rc_timer_int_flag[rc_ch->ch_number - 1] = 0;
        
        if (rc_ch->ch_number == 1)
            RC_Ch1_Timer_Start();
        if (rc_ch->ch_number == 2)
            RC_Ch2_Timer_Start();
        if (rc_ch->ch_number == 3)
            RC_Ch3_Timer_Start();
        if (rc_ch->ch_number == 4)
            RC_Ch4_Timer_Start();
        
        if (rc_ch->value != 0) {
            char debug[64] = "";
            sprintf(debug, "[%ld] RC CH %d: %ld --> %d \r\n", MILLISECONDS, rc_ch->ch_number, rc_ch_timer_count, rc_ch->value);
            USBUART_PutString(debug);
        }
    }
}

/* [] END OF FILE */
