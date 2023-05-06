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

#define IMU_DLP_MODE 6

const float IMU_SCALE;

typedef struct imu {
    float ax;
    float ay;
    float az;
    float gx;
    float gy;
    float gz;
    
    float a;
} IMU;

void init_imu();

void construct_imu(IMU* imu);

void get_imu_values(IMU* imu);

/* [] END OF FILE */
