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
#include "milliseconds.h"
#include "rc_ch.h"
#include "dc_motor.h"
#include "servo.h"
#include "imu.h"

// flag to enable UART debugging, must set to 0 if not using 
// or else code will not run without being plugged into computer via USB
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
    
    // configure USB connection for UART debugging
    #if DEBUG
    USBUART_Start(0, USBUART_5V_OPERATION);
    while (USBUART_GetConfiguration() == 0) {}
    USBUART_PutString("Init...");
    #endif
    
    // start the program milliseconds timer
    init_milliseconds();
    uint32 prev_time_ms = MILLISECONDS;
    uint32 prev_log_time_ms = MILLISECONDS;
    
    // turn off the blue status LED
    Status_LED_Write(0);
    
    // construct all the PID controllers for the drive motors
    init_pid();
    PID drive_dc_motor_pid;
    construct_pid(&drive_dc_motor_pid, 100.0, 0.0, 20.0);
    PID pendulum_dc_motor_pid;
    construct_pid(&pendulum_dc_motor_pid, 160.0, 0.0, 0.0);
    PID flywheel_dc_motor_pid;
    construct_pid(&flywheel_dc_motor_pid, 1.0, 0.0, 1.0);
    
    // construct all the PID controllers for the ball stabilization 
    PID pitch_stabilizer_pid;
    construct_pid(&pitch_stabilizer_pid, 40.0, 0.0, 7.5);
    PID roll_stabilizer_pid;
    construct_pid(&roll_stabilizer_pid, 3.0, 0.0, 0.0);
    typedef enum roll_stabilizer_state { IDLE, PUSH, PULL } ROLL_STABILIZER_STATE;
    ROLL_STABILIZER_STATE pendulum_stabilizer_state = IDLE;
    uint32 pendulum_stabilizer_time_ms = MILLISECONDS;
    
    // construct all DC motors
    init_dc_motors();
    DC_MOTOR drive_dc_motor;
    construct_dc_motor(&drive_dc_motor, DRIVE_DC_MOTOR, DRIVE_DC_MOTOR_MAX_PWM, DRIVE_DC_MOTOR_COUNTS_PER_ROT, MAXFLOAT, -MAXFLOAT, &drive_dc_motor_pid);
    DC_MOTOR pendulum_dc_motor;
    construct_dc_motor(&pendulum_dc_motor, PENDULUM_DC_MOTOR, PENDULUM_DC_MOTOR_MAX_PWM, PENDULUM_DC_MOTOR_COUNTS_PER_ROT, PENDULUM_DC_MOTOR_MAX_POS, PENDULUM_DC_MOTOR_MIN_POS, &pendulum_dc_motor_pid);
    DC_MOTOR flywheel_dc_motor;
    construct_dc_motor(&flywheel_dc_motor, FLYWHEEL_DC_MOTOR, FLYWHEEL_DC_MOTOR_MAX_PWM, FLYWHEEL_DC_MOTOR_COUNTS_PER_ROT, MAXFLOAT, -MAXFLOAT, &flywheel_dc_motor_pid);
    
    // construct head servo
    init_servos();
    SERVO head_servo;
    construct_servo(&head_servo, HEAD_SERVO, 90);
    
    // construct all RC channel readers to get remote joystick values
    init_rc_channels();
    RC_CH rc_ch1;
    construct_rc_ch(&rc_ch1, 1);
    RC_CH rc_ch2;
    construct_rc_ch(&rc_ch2, 2);
    RC_CH rc_ch3;
    construct_rc_ch(&rc_ch3, 3);
    RC_CH rc_ch4;
    construct_rc_ch(&rc_ch4, 4);
    
    // construct IMU sensor object
    init_imu();
    IMU imu;
    construct_imu(&imu);
    
    for(;;)
    {
        // track the time difference from the last main loop iteration
        float dt = (float) (MILLISECONDS - prev_time_ms);
        prev_time_ms = MILLISECONDS;
        
        // if the ball is not driving, read IMU values
        if (rc_ch2.value == 0) {
            get_imu_values(&imu);
        }
        
        // get RC remote values
        get_rc_ch_value(&rc_ch1);
        get_rc_ch_value(&rc_ch2);
        get_rc_ch_value(&rc_ch3);
        get_rc_ch_value(&rc_ch4);
        
        // Drive Channel/Motor
        if (rc_ch2.connected && rc_ch2.value != 0) {
            // if the RC channel 2 joystick is moved, drive the motor with a proportional PWM value
            set_dc_motor_pwm(&drive_dc_motor, rc_ch2.value);
            
            // if 1 meter has been traveled, flash the status LED
            get_dc_motor_pos(&drive_dc_motor);
            if ((int) drive_dc_motor.pos % (int) (360 * 1.0/0.508) < 20) {
                Status_LED_Write(1);
            } else {
                Status_LED_Write(0);
            }
        } else {
            // if the RC channel 2 joystick is not moved, stabilize the ball to be upright with PID
            float setpoint = 0.0;
            get_pid_output(&pitch_stabilizer_pid, setpoint, imu.pitch, IMU_TILT_TOLERANCE, dt);
            int drive_dc_motor_pwm = (int) pitch_stabilizer_pid.output;
            set_dc_motor_pwm(&drive_dc_motor, drive_dc_motor_pwm);
        }
        
        // Pendulum Channel/Motor
        if (rc_ch1.connected && abs(rc_ch1.value) > RC_CH1_MIN_VALUE) {
            // if the RC channel 1 joystick is moved, move the pendulum to the specified setpoint angle
            float pendulum_angle = (rc_ch1.value * -100.0)/RC_MAX_VALUE;
            pendulum_dc_motor_pid.kp = 160.0;
            set_dc_motor_pos(&pendulum_dc_motor, pendulum_angle, dt, BANG_BANG_CONTROLLER);
            pendulum_stabilizer_state = IDLE;
        } else {
            // if the RC channel 1 joystick is not moved, moved the pendulum back to going straight down
            float setpoint = 0.0;
            set_dc_motor_pos(&pendulum_dc_motor, setpoint, dt, BANG_BANG_CONTROLLER);
        }
        
        // Flywheel Channel/Motor
        if (rc_ch4.connected && rc_ch4.value != 0) {
            // if the RC channel 4 joystick is moved, spin the flywheel with a proportional PWM value
            set_dc_motor_pwm(&flywheel_dc_motor, rc_ch4.value);
        } else {
            // if the RC channel 4 joystick is not moved, stop the flywheel
            set_dc_motor_pwm(&flywheel_dc_motor, 0);
        }
        
        // Head Channel/Servo
        if (rc_ch3.connected && rc_ch3.value != 0) {
            // if the RC channel 3 joystick is moved, spin the head servo with a proportional PWM value
            uint8 head_servo_pos = (rc_ch3.value * 50)/(RC_MAX_VALUE) + 90;
            set_servo_pos(&head_servo, head_servo_pos);
        } else {
            // if the RC channel 3 joystick is not moved, stop the head servo
            set_servo_pos(&head_servo, 90);
        }

        #if DEBUG
            char debug[64] = "";
            sprintf(debug, "[%ld] %d %d %d %d", MILLISECONDS, rc_ch1.value, rc_ch2.value, rc_ch3.value, rc_ch4.value);
            sprintf(debug, "%s - (%d %d %d) %d %d", debug, (int) (imu.ax[0] * 100), (int) (imu.ay[0] * 100), (int) (imu.az[0] * 100), (int) (imu.roll * 1), (int) (imu.pitch * 1));
            sprintf(debug, "%s - %d %d %d", debug, (int) (drive_dc_motor.pwm * 1), (int) (pendulum_dc_motor.pwm * 1), (int) (head_servo.pos));
            sprintf(debug, "%s - %d %d", debug, (int) (drive_dc_motor.pos * 1), (int) (pendulum_dc_motor.pos * 1));
            
            sprintf(debug, "%s\r\n", debug);
            USBUART_PutString(debug);
        #endif
    }
}

/* [] END OF FILE */
