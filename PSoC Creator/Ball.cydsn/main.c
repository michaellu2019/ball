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
 * https://socmaker.com/?p=296
 * https://github.com/embeddedprogrammer/psoc-pid-controller
 * https://www.hackster.io/hima/mpu6050-psoc6-interface-7c505e
*/

#include "project.h"
#include "milliseconds.h"
#include "rc_ch.h"
#include "dc_motor.h"
#include "imu.h"

#define DEBUG 1
char debug_data_buf[64] = "";

/*
 * Channel 1: Right Joystick X
 * Channel 2: Right Joystick Y
 * Channel 3: Left Joystick Y
 * Channel 4: Left Joystick X
 * Channel 5: Right Knob
 * Channel 6: Left Knob
*/

int main()
{
    CyGlobalIntEnable; /* Enable global interrupts. */

    /* Place your initialization/startup code here (e.g. MyInst_Start()) */
    USBUART_Start(0, USBUART_5V_OPERATION);
    
    while (USBUART_GetConfiguration() == 0) {}
    USBUART_PutString("Init...");
    
    init_milliseconds();
    uint32 prev_time_ms = MILLISECONDS;
    
    init_pid();
    PID drive_dc_motor_pid;
    construct_pid(&drive_dc_motor_pid, 1.0, 0.0, 1.0);
    PID pendulum_dc_motor_pid;
    construct_pid(&pendulum_dc_motor_pid, 1.0, 0.0, 1.0);
    PID flywheel_dc_motor_pid;
    construct_pid(&flywheel_dc_motor_pid, 1.0, 0.0, 1.0);
       
    init_dc_motors();
    DC_MOTOR drive_dc_motor;
    construct_dc_motor(&drive_dc_motor, DRIVE_DC_MOTOR, DRIVE_DC_MOTOR_MAX_PWM, DRIVE_DC_MOTOR_COUNTS_PER_ROT, MAXFLOAT, -MAXFLOAT, &drive_dc_motor_pid);
    DC_MOTOR pendulum_dc_motor;
    construct_dc_motor(&pendulum_dc_motor, PENDULUM_DC_MOTOR, PENDULUM_DC_MOTOR_MAX_PWM, PENDULUM_DC_MOTOR_COUNTS_PER_ROT, PENDULUM_DC_MOTOR_MAX_POS, PENDULUM_DC_MOTOR_MIN_POS, &pendulum_dc_motor_pid);
    DC_MOTOR flywheel_dc_motor;
    construct_dc_motor(&flywheel_dc_motor, FLYWHEEL_DC_MOTOR, FLYWHEEL_DC_MOTOR_MAX_PWM, FLYWHEEL_DC_MOTOR_COUNTS_PER_ROT, MAXFLOAT, -MAXFLOAT, &flywheel_dc_motor_pid);
    
    init_rc_channels();
    RC_CH rc_ch1;
    construct_rc_ch(&rc_ch1, 1);
    RC_CH rc_ch2;
    construct_rc_ch(&rc_ch2, 2);
    RC_CH rc_ch3;
    construct_rc_ch(&rc_ch3, 3);
    RC_CH rc_ch4;
    construct_rc_ch(&rc_ch4, 4);
    
    init_imu();
    IMU imu;
    construct_imu(&imu);
    
    for(;;)
    {
        get_rc_ch_value(&rc_ch1);
        get_rc_ch_value(&rc_ch2);
        get_rc_ch_value(&rc_ch3);
        get_rc_ch_value(&rc_ch4);
        
        if (rc_ch2.connected && rc_ch2.value != 0) {
            set_dc_motor_pwm(&drive_dc_motor, rc_ch2.value);
        } else {
            set_dc_motor_pwm(&drive_dc_motor, 0);
        }
        if (rc_ch1.connected && rc_ch1.value != 0) {
            // set_dc_motor_pwm(&pendulum_dc_motor, rc_ch1.value);
            float pendulum_angle = rc_ch1.value/100.0;
            set_dc_motor_pos(&pendulum_dc_motor, pendulum_angle);
        } else {
            // run_dc_motor(&pendulum_dc_motor, 0);
            set_dc_motor_pos(&pendulum_dc_motor, 0);
        }
        if (rc_ch4.connected && rc_ch4.value != 0) {
            set_dc_motor_pwm(&flywheel_dc_motor, rc_ch4.value);
        } else {
            set_dc_motor_pwm(&flywheel_dc_motor, 0);
        }
        
        float dt = (float) (MILLISECONDS - prev_time_ms);
        prev_time_ms = MILLISECONDS;
        
        //set_dc_motor_speed(&drive_dc_motor, 0.0, dt);
        
        /*get_dc_motor_pos(&drive_dc_motor);
        get_dc_motor_pos(&pendulum_dc_motor);
        get_dc_motor_pos(&flywheel_dc_motor);*/
        
        get_imu_values(&imu);

        if (rc_ch1.value != 0 || rc_ch2.value != 0 || rc_ch3.value != 0 || rc_ch3.value != 0) {
            char debug[64] = "";
            sprintf(debug, "[%ld] %d %d %d %d - %d %d %d - %d %d \r\n", MILLISECONDS, rc_ch1.value, rc_ch2.value, rc_ch3.value, rc_ch4.value,
                                                                            (int) imu.ax, (int) imu.ay, (int) imu.az,
                                                                            (int) (drive_dc_motor.pos * 100), (int) (pendulum_dc_motor.pos * 100));
            USBUART_PutString(debug);
        }
    }
}

/* [] END OF FILE */
