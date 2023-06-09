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
#include "pid.h"
#include "math.h"

#define DRIVE_DC_MOTOR 0
#define PENDULUM_DC_MOTOR 1
#define FLYWHEEL_DC_MOTOR 2

#define COUNTS_PER_QUADRATURE 4.0
#define DRIVE_DC_MOTOR_COUNTS_PER_ROT 8400.0
#define PENDULUM_DC_MOTOR_COUNTS_PER_ROT 3200.0
#define FLYWHEEL_DC_MOTOR_COUNTS_PER_ROT 1200.0

#define DRIVE_DC_MOTOR_MAX_PWM 240
#define PENDULUM_DC_MOTOR_MAX_PWM 70
#define FLYWHEEL_DC_MOTOR_MAX_PWM 240

#define PENDULUM_DC_MOTOR_MAX_POS 2.4
#define PENDULUM_DC_MOTOR_MIN_POS -2.4

#define BANG_BANG_CONTROLLER 0
#define PID_CONTROLLER 1

const int NUM_DC_MOTORS;
const float DC_MOTOR_SPEED_TOLERANCE;
const float DC_MOTOR_POS_TOLERANCE;

typedef struct dc_motor {
    uint8 id;
    int16 pwm;
    int16 max_pwm;
    uint16 counts_per_rot;
    int16 enc_count;
    
    float max_pos;
    float min_pos;
    float pos;
    float prev_pos;
    float vel;
    float prev_vel;
    float vel_filt;
    
    PID *pid;
} DC_MOTOR;

void init_dc_motors();

void construct_dc_motor(DC_MOTOR *dc_motor, uint8 id, int16 max_pwm, uint16 counts_per_rot, float max_pos, float min_pos, PID *PID);

void set_dc_motor_pwm(DC_MOTOR *dc_motor, int16 pwm);

void get_dc_motor_pos(DC_MOTOR *dc_motor);

void set_dc_motor_pos(DC_MOTOR *dc_motor, float target_pos, float dt, uint8 controller);

void set_dc_motor_speed(DC_MOTOR *dc_motor, float target_v, float dt);

/* [] END OF FILE */
