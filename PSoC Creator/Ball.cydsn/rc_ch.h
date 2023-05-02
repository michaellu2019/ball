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
#pragma once
#include "project.h"

#define NUM_RC_CHANNELS 4

const int RC_TIMER_PERIOD;
const int RC_TIMER_MAX_VALUE;
const int RC_TIMER_MID_VALUE;
const int RC_TIMER_MIN_VALUE;
const int RC_TIMER_MID_TOLERANCE;
const int RC_CONNECTION_TIMEOUT;

const int RC_MAX_VALUE;
const int RC_MIN_VALUE;

typedef struct rc_ch {
    uint8 ch_number;
    uint8 connected;
    uint32 last_connected;
    uint32 last_timer_count;
    int16 value;
} RC_CH;

void rc_ch_timer_isr_handler(uint8 ch_number);

void init_rc_channels();

void construct_rc_ch(RC_CH* rc_ch, uint8 ch_number);

void get_rc_ch_value(RC_CH* rc_ch);

/* [] END OF FILE */
