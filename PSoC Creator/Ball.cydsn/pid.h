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

const int NUM_DC_MOTORS;
const float DC_MOTOR_POS_TOLERANCE;

typedef struct pid {
    float kp;
    float ki;
    float kd;
    
    float error;
    float prev_error;
    float d_error;
    float sum_error;
    
    float setpoint;
    float value;
    float dt;
    float output;
} PID;

void init_pid();

void construct_pid(PID *pid, float kp, float ki, float kd);

void get_pid_output(PID *pid, float setpoint, float value, float dt);

void reset_pid_sum_error(PID *pid);

/* [] END OF FILE */
