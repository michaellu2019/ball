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
#include "dc_motor.h"

const int NUM_DC_MOTORS = 3;
const float DC_MOTOR_SPEED_TOLERANCE = 0.0;
const float DC_MOTOR_POS_TOLERANCE = 2.0;

// initialize all PSoC components for DC motors
void init_dc_motors() {
    Drive_DC_Motor_PWM_Start();
    Drive_DC_Motor_Quad_Dec_Start();
    Drive_DC_Motor_Quad_Dec_SetCounter(0);
    
    Pendulum_DC_Motor_PWM_Start();
    Pendulum_DC_Motor_Quad_Dec_Start();
    Pendulum_DC_Motor_Quad_Dec_SetCounter(0);
    
    Flywheel_DC_Motor_PWM_Start();
}

// build a DC motor struct object with the specified settings
void construct_dc_motor(DC_MOTOR *dc_motor, uint8 id, int16 max_pwm, uint16 counts_per_rot, float max_pos, float min_pos, PID *pid) {
    dc_motor->id = id;
    dc_motor->max_pwm = max_pwm;
    dc_motor->pwm = 0;
    dc_motor->counts_per_rot = counts_per_rot;
    dc_motor->enc_count = 0;
    dc_motor->max_pos = max_pos;
    dc_motor->min_pos = min_pos;
    dc_motor->pos = 0;
    dc_motor->prev_pos = 0;
    dc_motor->vel = 0;
    dc_motor->prev_vel = 0;
    dc_motor->vel_filt = 0;
    dc_motor->pid = pid;
}

// drive the DC motor with a specific PWM
void set_dc_motor_pwm(DC_MOTOR *dc_motor, int16 pwm) {
    // if the PWM value provided is outside of the max PWM value of the motor, clip it
    if (abs(pwm) > dc_motor->max_pwm) {
        if (pwm < 0) 
            dc_motor->pwm = -dc_motor->max_pwm;
        if (pwm > 0) 
            dc_motor->pwm = dc_motor->max_pwm;
    } else {
        dc_motor->pwm = pwm;
    }
    
    // write the PWM value to the appropriate pins for the correct DC motor, based on the DBH-12V motor driver specs
    if (dc_motor->id == DRIVE_DC_MOTOR) {
        if (pwm < 0) {
            Drive_DC_Motor_PWM_WriteCompare1(abs(dc_motor->pwm));
            Drive_DC_Motor_PWM_WriteCompare2(0);
        } else if (pwm > 0) {
            Drive_DC_Motor_PWM_WriteCompare1(0);
            Drive_DC_Motor_PWM_WriteCompare2(dc_motor->pwm);
        } else {
            Drive_DC_Motor_PWM_WriteCompare1(0);
            Drive_DC_Motor_PWM_WriteCompare2(0);
        }
    } else if (dc_motor->id == PENDULUM_DC_MOTOR) {
        if (pwm < 0) {
            Pendulum_DC_Motor_PWM_WriteCompare1(abs(dc_motor->pwm));
            Pendulum_DC_Motor_PWM_WriteCompare2(0);
        } else if (pwm > 0) {
            Pendulum_DC_Motor_PWM_WriteCompare1(0);
            Pendulum_DC_Motor_PWM_WriteCompare2(dc_motor->pwm);
        } else {
            Pendulum_DC_Motor_PWM_WriteCompare1(0);
            Pendulum_DC_Motor_PWM_WriteCompare2(0);
        }
    } else if (dc_motor->id == FLYWHEEL_DC_MOTOR) {
        if (pwm < 0) {
            Flywheel_DC_Motor_PWM_WriteCompare1(abs(dc_motor->pwm));
            Flywheel_DC_Motor_PWM_WriteCompare2(0);
        } else if (pwm > 0) {
            Flywheel_DC_Motor_PWM_WriteCompare1(0);
            Flywheel_DC_Motor_PWM_WriteCompare2(dc_motor->pwm);
        } else {
            Flywheel_DC_Motor_PWM_WriteCompare1(0);
            Flywheel_DC_Motor_PWM_WriteCompare2(0);
        }
    }
}

// read the encoder value of a DC motor to get the shaft angle
void get_dc_motor_pos(DC_MOTOR *dc_motor) {
    // read the appropriate encoder
    if (dc_motor->id == DRIVE_DC_MOTOR) {
        dc_motor->enc_count = Drive_DC_Motor_Quad_Dec_GetCounter();
    } else if (dc_motor->id == PENDULUM_DC_MOTOR) {
        dc_motor->enc_count = Pendulum_DC_Motor_Quad_Dec_GetCounter();
    }
    
    // calculate the shaft angle from the encoder count
    dc_motor->pos = (dc_motor->enc_count * 2 * M_PI * COUNTS_PER_QUADRATURE)/dc_motor->counts_per_rot * (180.0/M_PI);
}

// move the DC motor shaft to a certain position
void set_dc_motor_pos(DC_MOTOR *dc_motor, float target_pos, float dt, uint8 controller) {
    if (dc_motor->id == PENDULUM_DC_MOTOR) {
        if (controller == BANG_BANG_CONTROLLER) {
            // set the PWM value of the DC motor to PID Kp and get the current shaft angle
            const int pwm = (int) dc_motor->pid->kp;
            get_dc_motor_pos(dc_motor);
            
            // in BANG BANG (bang bang bang) control, drive the DC motor at a set PWM until it hits
            // the setpoint plus/minus some tolerance, otherwise shut it off
            if (dc_motor->pos > target_pos + DC_MOTOR_POS_TOLERANCE) {
                set_dc_motor_pwm(dc_motor, -pwm);
            } else if (dc_motor->pos < target_pos - DC_MOTOR_POS_TOLERANCE) {
                set_dc_motor_pwm(dc_motor, pwm);
            } else {
                set_dc_motor_pwm(dc_motor, 0);
            }
        } else if (controller == PID_CONTROLLER) {
            // get the DC motor shaft angle and compute the PID output
            get_dc_motor_pos(dc_motor);
            get_pid_output(dc_motor->pid, target_pos, dc_motor->pos, DC_MOTOR_POS_TOLERANCE, dt);
            int pwm = (int) dc_motor->pid->output;
            
            // if the PWM is too low, don't bother
            if (abs(pwm) < 20) {
                set_dc_motor_pwm(dc_motor, 0);
            } else {
                set_dc_motor_pwm(dc_motor, pwm);
            }
        }
    }
}

// set the drive speed of the DC motor via PWM
void set_dc_motor_speed(DC_MOTOR *dc_motor, float target_vel, float dt) {
    get_dc_motor_pos(dc_motor);
    float vel = (dc_motor->pos - dc_motor->prev_pos)/dt;
    dc_motor->prev_pos = dc_motor->pos;
    float af = 0.893;
    float bf = (1.0 - af)/2.0;
    dc_motor->vel_filt = af * dc_motor->vel_filt + bf * vel + bf * dc_motor->prev_vel;
    
    get_pid_output(dc_motor->pid, target_vel, dc_motor->vel_filt, DC_MOTOR_SPEED_TOLERANCE, dt);
    int16 pwm = (int16) dc_motor->pid->output;
    set_dc_motor_pwm(dc_motor, pwm);
}

/* [] END OF FILE */
