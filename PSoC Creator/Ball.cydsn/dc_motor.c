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

void init_dc_motors() {
    Drive_DC_Motor_PWM_Start();
    Pendulum_DC_Motor_PWM_Start();
    Flywheel_DC_Motor_PWM_Start();
}

void construct_dc_motor(DC_MOTOR *dc_motor, uint8 motor_id) {
    dc_motor->motor_id = motor_id;
    dc_motor->motor_pwm = 0;
}

void run_motor(DC_MOTOR *dc_motor, int16 motor_pwm) {
    if (dc_motor->motor_id == DRIVE_DC_MOTOR) {
        if (motor_pwm < 0) {
            Drive_DC_Motor_PWM_WriteCompare1(abs(motor_pwm));
            Drive_DC_Motor_PWM_WriteCompare2(0);
        } else if (motor_pwm > 0) {
            Drive_DC_Motor_PWM_WriteCompare1(0);
            Drive_DC_Motor_PWM_WriteCompare2(motor_pwm);
        } else {
            Drive_DC_Motor_PWM_WriteCompare1(0);
            Drive_DC_Motor_PWM_WriteCompare2(0);
        }
    } else if (dc_motor->motor_id == PENDULUM_DC_MOTOR) {
        if (motor_pwm < 0) {
            Pendulum_DC_Motor_PWM_WriteCompare1(abs(motor_pwm));
            Pendulum_DC_Motor_PWM_WriteCompare2(0);
        } else if (motor_pwm > 0) {
            Pendulum_DC_Motor_PWM_WriteCompare1(0);
            Pendulum_DC_Motor_PWM_WriteCompare2(motor_pwm);
        } else {
            Pendulum_DC_Motor_PWM_WriteCompare1(0);
            Pendulum_DC_Motor_PWM_WriteCompare2(0);
        }
    } else if (dc_motor->motor_id == FLYWHEEL_DC_MOTOR) {
        if (motor_pwm < 0) {
            Flywheel_DC_Motor_PWM_WriteCompare1(abs(motor_pwm));
            Flywheel_DC_Motor_PWM_WriteCompare2(0);
        } else if (motor_pwm > 0) {
            Flywheel_DC_Motor_PWM_WriteCompare1(0);
            Flywheel_DC_Motor_PWM_WriteCompare2(motor_pwm);
        } else {
            Flywheel_DC_Motor_PWM_WriteCompare1(0);
            Flywheel_DC_Motor_PWM_WriteCompare2(0);
        }
    }
}
/* [] END OF FILE */
