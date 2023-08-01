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
#include "servo.h"

// initialize all PSoC components for servos
void init_servos() {
    Head_Servo_PWM_Start();
}

// construct a servo struct object
void construct_servo(SERVO *servo, uint8 id, int8 init_pos) {
    servo->id = id;
    set_servo_pos(servo, init_pos);
}

// write a PWM value to the servo based on the provided position
void set_servo_pos(SERVO *servo, int16 pos) {
    servo->pos = pos;
    if (servo->id == HEAD_SERVO) {
        int pos_microseconds = (pos/180.0 * NUM_COUNTS_MS/2) + NUM_COUNTS_MS/2;
        if (pos_microseconds > NUM_COUNTS_MS/2 && pos_microseconds < NUM_COUNTS_MS)
            Head_Servo_PWM_WriteCompare(pos_microseconds);
    }
}

/* [] END OF FILE */
