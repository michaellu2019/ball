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

#define DRIVE_DC_MOTOR 0
#define PENDULUM_DC_MOTOR 1
#define FLYWHEEL_DC_MOTOR 2

const int NUM_DC_MOTORS;

typedef struct dc_motor {
    uint8 motor_id;
    int16 motor_pwm;
} DC_MOTOR;

void init_dc_motors();

void construct_dc_motor(DC_MOTOR *dc_motor, uint8 motor_id);

void run_motor(DC_MOTOR *dc_motor, int16 motor_pwm);

/* [] END OF FILE */
