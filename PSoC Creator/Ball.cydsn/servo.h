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
#include "math.h"

#define HEAD_SERVO 0

#define NUM_COUNTS_MS 4000

typedef struct servo {
    uint8 id;
    uint8 pos;
} SERVO;

void init_servos();

void construct_servo(SERVO *servo, uint8 id, int8 pos);

void set_servo_pos(SERVO *servo, int16 pos);

/* [] END OF FILE */
