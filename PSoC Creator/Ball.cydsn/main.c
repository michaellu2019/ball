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
#include "servo.h"
#include "imu.h"

#define DEBUG 0
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
    
    #if DEBUG
    USBUART_Start(0, USBUART_5V_OPERATION);
    while (USBUART_GetConfiguration() == 0) {}
    USBUART_PutString("Init...");
    #endif
    
    init_milliseconds();
    uint32 prev_time_ms = MILLISECONDS;
    
    Status_LED_Write(0);
    
    init_pid();
    PID drive_dc_motor_pid;
    construct_pid(&drive_dc_motor_pid, 100.0, 0.0, 20.0);
    PID pendulum_dc_motor_pid;
    construct_pid(&pendulum_dc_motor_pid, 160.0, 0.0, 0.0);
    PID flywheel_dc_motor_pid;
    construct_pid(&flywheel_dc_motor_pid, 1.0, 0.0, 1.0);
    
    PID pitch_stabilizer_pid;
    construct_pid(&pitch_stabilizer_pid, 35.0, 0.0, 7.5);
    PID roll_stabilizer_pid;
    construct_pid(&roll_stabilizer_pid, 3.0, 0.0, 0.5);
       
    init_dc_motors();
    DC_MOTOR drive_dc_motor;
    construct_dc_motor(&drive_dc_motor, DRIVE_DC_MOTOR, DRIVE_DC_MOTOR_MAX_PWM, DRIVE_DC_MOTOR_COUNTS_PER_ROT, MAXFLOAT, -MAXFLOAT, &drive_dc_motor_pid);
    DC_MOTOR pendulum_dc_motor;
    construct_dc_motor(&pendulum_dc_motor, PENDULUM_DC_MOTOR, PENDULUM_DC_MOTOR_MAX_PWM, PENDULUM_DC_MOTOR_COUNTS_PER_ROT, PENDULUM_DC_MOTOR_MAX_POS, PENDULUM_DC_MOTOR_MIN_POS, &pendulum_dc_motor_pid);
    DC_MOTOR flywheel_dc_motor;
    construct_dc_motor(&flywheel_dc_motor, FLYWHEEL_DC_MOTOR, FLYWHEEL_DC_MOTOR_MAX_PWM, FLYWHEEL_DC_MOTOR_COUNTS_PER_ROT, MAXFLOAT, -MAXFLOAT, &flywheel_dc_motor_pid);
    
    init_servos();
    SERVO head_servo;
    construct_servo(&head_servo, HEAD_SERVO, 90);
    
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
        float dt = (float) (MILLISECONDS - prev_time_ms);
        prev_time_ms = MILLISECONDS;
        
        if (rc_ch2.value == 0 || abs(rc_ch1.value) < RC_CH1_MIN_VALUE) {
            get_imu_values(&imu);
            /*if (fabs(imu.ax[0]) < 1.0 && fabs(imu.ay[0]) < 1.0 && fabs(imu.az[0]) < 1.0) {
                Status_LED_Write(1);
            } else {
                Status_LED_Write(0);
            }*/
            /*int16 ax, ay, az;
            int16 gx, gy, gz;
            MPU6050_getMotion6(&ay, &ax, &az, &gx, &gy, &gz);*/
        }
        
        get_rc_ch_value(&rc_ch1);
        get_rc_ch_value(&rc_ch2);
        get_rc_ch_value(&rc_ch3);
        get_rc_ch_value(&rc_ch4);
        
        // Drive Channel/Motor
        if (rc_ch2.connected && rc_ch2.value != 0) {
            set_dc_motor_pwm(&drive_dc_motor, rc_ch2.value);
            
            get_dc_motor_pos(&drive_dc_motor);
            if ((int) drive_dc_motor.pos % 360 < 20) {
                Status_LED_Write(1);
            } else {
                Status_LED_Write(0);
            }
        } else {
            float setpoint = 0.0;
            get_pid_output(&pitch_stabilizer_pid, setpoint, imu.pitch, IMU_TILT_TOLERANCE, dt);
            int drive_dc_motor_pwm = (int) pitch_stabilizer_pid.output;
            set_dc_motor_pwm(&drive_dc_motor, drive_dc_motor_pwm);
        }
        
        // Pendulum Channel/Motor
        if (rc_ch1.connected && abs(rc_ch1.value) > RC_CH1_MIN_VALUE) {
            float pendulum_angle = (rc_ch1.value * -100.0)/RC_MAX_VALUE;
            set_dc_motor_pos(&pendulum_dc_motor, pendulum_angle, dt, BANG_BANG_CONTROLLER);
        } else {
            set_dc_motor_pos(&pendulum_dc_motor, 0, dt, BANG_BANG_CONTROLLER);
            
            /*float setpoint = 0.0;
            get_pid_output(&roll_stabilizer_pid, setpoint, imu.roll, IMU_TILT_TOLERANCE, dt);
            int pendulum_dc_motor_pwm = (int) -roll_stabilizer_pid.output;
            set_dc_motor_pwm(&pendulum_dc_motor, pendulum_dc_motor_pwm);*/
        }
        
        // Flywheel Channel/Motor
        if (rc_ch4.connected && rc_ch4.value != 0) {
            set_dc_motor_pwm(&flywheel_dc_motor, rc_ch4.value);
        } else {
            set_dc_motor_pwm(&flywheel_dc_motor, 0);
        }
        
        // Head Channel/Servo
        if (rc_ch3.connected && rc_ch3.value != 0) {
            uint8 head_servo_pos = (rc_ch3.value * 90)/(RC_MAX_VALUE) + 90;
            set_servo_pos(&head_servo, head_servo_pos);
        } else {
            set_servo_pos(&head_servo, 90);
        }
        
        //set_dc_motor_speed(&drive_dc_motor, M_PI/2.0, dt);
        
        /*get_dc_motor_pos(&drive_dc_motor);
        get_dc_motor_pos(&pendulum_dc_motor);
        get_dc_motor_pos(&flywheel_dc_motor);*/

        #if DEBUG
        if (1) {// rc_ch1.value != 0 || rc_ch2.value != 0 || rc_ch3.value != 0 || rc_ch3.value != 0) {
            char debug[64] = "";
            //sprintf(debug, "[%ld] %d %d %d %d", MILLISECONDS, rc_ch1.value, rc_ch2.value, rc_ch3.value, rc_ch4.value);
            //sprintf(debug, "%s - (%d %d %d) %d %d", debug, (int) (imu.ax[0] * 100), (int) (imu.ay[0] * 100), (int) (imu.az[0] * 100), (int) (imu.roll * 1), (int) (imu.pitch * 1));
            //sprintf(debug, "%s - %d %d %d", debug, (int) (drive_dc_motor.pwm * 1), (int) (pendulum_dc_motor.pwm * 1), (int) (head_servo.pos));
            //sprintf(debug, "%s - %d %d", debug, (int) (drive_dc_motor.pos * 1), (int) (pendulum_dc_motor.pos * 1));
            
            
            sprintf(debug, "%s%d", debug, (int) (rc_ch1.value * 1));
            sprintf(debug, "%s + %d", debug, (int) (imu.roll * 1));
            sprintf(debug, "%s -> %d", debug, (int) (pendulum_dc_motor.pwm * 1));
            sprintf(debug, "%s - %d", debug, (int) (pendulum_dc_motor.pos * 1));
            
            sprintf(debug, "%s\r\n", debug);
            USBUART_PutString(debug);
        }
        #endif
    }
}

/* [] END OF FILE */
