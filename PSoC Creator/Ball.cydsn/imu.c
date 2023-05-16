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
#include "imu.h"
#include "mpu6050.h"

const float IMU_SCALE = 1600.0;
const float IMU_TILT_TOLERANCE = 6.0;

// initialize all PSoC components for the IMU
void init_imu() {
    MPU6050_I2C_Start();
    MPU6050_init();
    MPU6050_initialize();
    MPU6050_setDLPFMode(IMU_DLP_MODE);
}

// construct an IMU object with the specified parameters
void construct_imu(IMU* imu) {
    int i;
    for (i = 0; i < NUM_IMU_AVG_SAMPLES; i++) {
        imu->ax[i] = 0.0;
        imu->ay[i] = 0.0;
        imu->az[i] = 0.0;
    }
    
    imu->a = 0.9;
}

// get the IMU acceleration values and calculate the roll and pitch angle
void get_imu_values(IMU* imu) {
    int16 ax, ay, az;
    int16 gx, gy, gz;
    MPU6050_getMotion6(&ay, &ax, &az, &gx, &gy, &gz);
    
    // compute a running average of 4 samples to smooth out the data
    int i;
    float sum_ax = 0.0;
    float sum_ay = 0.0;
    float sum_az = 0.0;
    for (i = NUM_IMU_AVG_SAMPLES - 1; i > 0; i--) {
        imu->ax[i] = imu->ax[i - 1];
        imu->ay[i] = imu->ay[i - 1];
        imu->az[i] = imu->az[i - 1];
        sum_ax += imu->ax[i];
        sum_ay += imu->ay[i];
        sum_az += imu->az[i];
    }
    
    imu->ax[0] = (sum_ax + ax/IMU_SCALE)/NUM_IMU_AVG_SAMPLES;
    imu->ay[0] = (sum_ay + ay/IMU_SCALE)/NUM_IMU_AVG_SAMPLES;
    imu->az[0] = (sum_az + az/IMU_SCALE)/NUM_IMU_AVG_SAMPLES;
    
    // compute the roll and pitch angle of the IMU from the acceleration values
    // as there will always be an acceleration vector pointing towards the core of the earth
    // at 9.8 m/s^2
    imu->roll = atan2(imu->az[0], imu->ay[0]) * (180.0/M_PI) + 90.0;
    imu->pitch = atan2(imu->az[0], imu->ax[0]) * (180.0/M_PI) + 90.0;
}
/* [] END OF FILE */
