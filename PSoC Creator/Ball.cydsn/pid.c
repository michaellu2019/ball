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
#include "pid.h"

void init_pid() {
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
}

void construct_pid(PID *pid, float kp, float ki, float kd) {
    pid->kp = kp;
    pid->ki = ki;
    pid->kd = kd;
    pid->error = 0;
    pid->prev_error = 0;
    pid->d_error = 0;
    pid->sum_error = 0;
    pid->setpoint = 0;
    pid->value = 0;
    pid->dt = 0;
    pid->output = 0;
}

void get_pid_output(PID *pid, float setpoint, float value, float dt) {
    pid->setpoint = setpoint;
    pid->value = value;
    pid->dt = dt;
    
    pid->error = pid->setpoint - pid->value;
    pid->sum_error = pid->sum_error + pid->error * pid->dt;
    pid->d_error = (pid->error - pid->prev_error)/pid->dt;
    pid->prev_error = pid->error;
    
    pid->output = pid->kp * pid->error + pid->ki * pid->sum_error + pid->kd * pid->d_error;
}

void reset_pid_sum_error(PID *pid) {
    pid->sum_error = 0.0;
}

/* [] END OF FILE */