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

#define IMU_DLP_MODE 6
#define NUM_IMU_AVG_SAMPLES 4

const float IMU_SCALE;
const float IMU_TILT_TOLERANCE;

typedef struct imu {
    float ax[NUM_IMU_AVG_SAMPLES];
    float ay[NUM_IMU_AVG_SAMPLES];
    float az[NUM_IMU_AVG_SAMPLES];
    float gx[NUM_IMU_AVG_SAMPLES];
    float gy[NUM_IMU_AVG_SAMPLES];
    float gz[NUM_IMU_AVG_SAMPLES];
    
    float pitch;
    float roll;
    
    float a;
} IMU;

void init_imu();

void construct_imu(IMU* imu);

void get_imu_values(IMU* imu);

/* [] END OF FILE */
