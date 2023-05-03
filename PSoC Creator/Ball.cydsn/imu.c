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

void init_imu() {
    MPU6050_I2C_Start();
    MPU6050_init();
    MPU6050_initialize();
    MPU6050_setDLPFMode(IMU_DLP_MODE);
}

void construct_imu(IMU* imu) {
    imu->ax = 0;
    imu->ay = 0;
    imu->az = 0;
    imu->gx = 0;
    imu->gy = 0;
    imu->gz = 0;
}

void get_imu_values(IMU* imu) {
    int16 ax, ay, az;
    int16 gx, gy, gz;
    MPU6050_getMotion6(&ay, &ax, &az, &gx, &gy, &gz);
    imu->ax = ax/IMU_SCALE;
    imu->ay = ay/IMU_SCALE;
    imu->az = az/IMU_SCALE;
    imu->gx = gx/IMU_SCALE;
    imu->gy = gy/IMU_SCALE;
    imu->gz = gz/IMU_SCALE;
}
/* [] END OF FILE */
