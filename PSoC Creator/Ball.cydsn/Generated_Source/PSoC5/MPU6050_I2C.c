/*******************************************************************************
* File Name: MPU6050_I2C.c
* Version 3.50
*
* Description:
*  This file provides the source code of APIs for the I2C component.
*  The actual protocol and operation code resides in the interrupt service
*  routine file.
*
*******************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "MPU6050_I2C_PVT.h"


/**********************************
*      System variables
**********************************/

uint8 MPU6050_I2C_initVar = 0u; /* Defines if component was initialized */

volatile uint8 MPU6050_I2C_state;  /* Current state of I2C FSM */


/*******************************************************************************
* Function Name: MPU6050_I2C_Init
********************************************************************************
*
* Summary:
*  Initializes I2C registers with initial values provided from customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MPU6050_I2C_Init(void) 
{
#if (MPU6050_I2C_FF_IMPLEMENTED)
    /* Configure fixed function block */
    MPU6050_I2C_CFG_REG  = MPU6050_I2C_DEFAULT_CFG;
    MPU6050_I2C_XCFG_REG = MPU6050_I2C_DEFAULT_XCFG;
    MPU6050_I2C_ADDR_REG = MPU6050_I2C_DEFAULT_ADDR;
    MPU6050_I2C_CLKDIV1_REG = LO8(MPU6050_I2C_DEFAULT_DIVIDE_FACTOR);
    MPU6050_I2C_CLKDIV2_REG = HI8(MPU6050_I2C_DEFAULT_DIVIDE_FACTOR);

#else
    uint8 intState;

    /* Configure control and interrupt sources */
    MPU6050_I2C_CFG_REG      = MPU6050_I2C_DEFAULT_CFG;
    MPU6050_I2C_INT_MASK_REG = MPU6050_I2C_DEFAULT_INT_MASK;

    /* Enable interrupt generation in status */
    intState = CyEnterCriticalSection();
    MPU6050_I2C_INT_ENABLE_REG |= MPU6050_I2C_INTR_ENABLE;
    CyExitCriticalSection(intState);

    /* Configure bit counter */
    #if (MPU6050_I2C_MODE_SLAVE_ENABLED)
        MPU6050_I2C_PERIOD_REG = MPU6050_I2C_DEFAULT_PERIOD;
    #endif  /* (MPU6050_I2C_MODE_SLAVE_ENABLED) */

    /* Configure clock generator */
    #if (MPU6050_I2C_MODE_MASTER_ENABLED)
        MPU6050_I2C_MCLK_PRD_REG = MPU6050_I2C_DEFAULT_MCLK_PRD;
        MPU6050_I2C_MCLK_CMP_REG = MPU6050_I2C_DEFAULT_MCLK_CMP;
    #endif /* (MPU6050_I2C_MODE_MASTER_ENABLED) */
#endif /* (MPU6050_I2C_FF_IMPLEMENTED) */

#if (MPU6050_I2C_TIMEOUT_ENABLED)
    MPU6050_I2C_TimeoutInit();
#endif /* (MPU6050_I2C_TIMEOUT_ENABLED) */

    /* Configure internal interrupt */
    CyIntDisable    (MPU6050_I2C_ISR_NUMBER);
    CyIntSetPriority(MPU6050_I2C_ISR_NUMBER, MPU6050_I2C_ISR_PRIORITY);
    #if (MPU6050_I2C_INTERN_I2C_INTR_HANDLER)
        (void) CyIntSetVector(MPU6050_I2C_ISR_NUMBER, &MPU6050_I2C_ISR);
    #endif /* (MPU6050_I2C_INTERN_I2C_INTR_HANDLER) */

    /* Set FSM to default state */
    MPU6050_I2C_state = MPU6050_I2C_SM_IDLE;

#if (MPU6050_I2C_MODE_SLAVE_ENABLED)
    /* Clear status and buffers index */
    MPU6050_I2C_slStatus = 0u;
    MPU6050_I2C_slRdBufIndex = 0u;
    MPU6050_I2C_slWrBufIndex = 0u;

    /* Configure matched address */
    MPU6050_I2C_SlaveSetAddress(MPU6050_I2C_DEFAULT_ADDR);
#endif /* (MPU6050_I2C_MODE_SLAVE_ENABLED) */

#if (MPU6050_I2C_MODE_MASTER_ENABLED)
    /* Clear status and buffers index */
    MPU6050_I2C_mstrStatus = 0u;
    MPU6050_I2C_mstrRdBufIndex = 0u;
    MPU6050_I2C_mstrWrBufIndex = 0u;
#endif /* (MPU6050_I2C_MODE_MASTER_ENABLED) */
}


/*******************************************************************************
* Function Name: MPU6050_I2C_Enable
********************************************************************************
*
* Summary:
*  Enables I2C operations.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  None.
*
*******************************************************************************/
void MPU6050_I2C_Enable(void) 
{
#if (MPU6050_I2C_FF_IMPLEMENTED)
    uint8 intState;

    /* Enable power to block */
    intState = CyEnterCriticalSection();
    MPU6050_I2C_ACT_PWRMGR_REG  |= MPU6050_I2C_ACT_PWR_EN;
    MPU6050_I2C_STBY_PWRMGR_REG |= MPU6050_I2C_STBY_PWR_EN;
    CyExitCriticalSection(intState);
#else
    #if (MPU6050_I2C_MODE_SLAVE_ENABLED)
        /* Enable bit counter */
        uint8 intState = CyEnterCriticalSection();
        MPU6050_I2C_COUNTER_AUX_CTL_REG |= MPU6050_I2C_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    #endif /* (MPU6050_I2C_MODE_SLAVE_ENABLED) */

    /* Enable slave or master bits */
    MPU6050_I2C_CFG_REG |= MPU6050_I2C_ENABLE_MS;
#endif /* (MPU6050_I2C_FF_IMPLEMENTED) */

#if (MPU6050_I2C_TIMEOUT_ENABLED)
    MPU6050_I2C_TimeoutEnable();
#endif /* (MPU6050_I2C_TIMEOUT_ENABLED) */
}


/*******************************************************************************
* Function Name: MPU6050_I2C_Start
********************************************************************************
*
* Summary:
*  Starts the I2C hardware. Enables Active mode power template bits or clock
*  gating as appropriate. It is required to be executed before I2C bus
*  operation.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Side Effects:
*  This component automatically enables its interrupt.  If I2C is enabled !
*  without the interrupt enabled, it can lock up the I2C bus.
*
* Global variables:
*  MPU6050_I2C_initVar - This variable is used to check the initial
*                             configuration, modified on the first
*                             function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MPU6050_I2C_Start(void) 
{
    if (0u == MPU6050_I2C_initVar)
    {
        MPU6050_I2C_Init();
        MPU6050_I2C_initVar = 1u; /* Component initialized */
    }

    MPU6050_I2C_Enable();
    MPU6050_I2C_EnableInt();
}


/*******************************************************************************
* Function Name: MPU6050_I2C_Stop
********************************************************************************
*
* Summary:
*  Disables I2C hardware and disables I2C interrupt. Disables Active mode power
*  template bits or clock gating as appropriate.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void MPU6050_I2C_Stop(void) 
{
    MPU6050_I2C_DisableInt();

#if (MPU6050_I2C_TIMEOUT_ENABLED)
    MPU6050_I2C_TimeoutStop();
#endif  /* End (MPU6050_I2C_TIMEOUT_ENABLED) */

#if (MPU6050_I2C_FF_IMPLEMENTED)
    {
        uint8 intState;
        uint16 blockResetCycles;

        /* Store registers effected by block disable */
        MPU6050_I2C_backup.addr    = MPU6050_I2C_ADDR_REG;
        MPU6050_I2C_backup.clkDiv1 = MPU6050_I2C_CLKDIV1_REG;
        MPU6050_I2C_backup.clkDiv2 = MPU6050_I2C_CLKDIV2_REG;

        /* Calculate number of cycles to reset block */
        blockResetCycles = ((uint16) ((uint16) MPU6050_I2C_CLKDIV2_REG << 8u) | MPU6050_I2C_CLKDIV1_REG) + 1u;

        /* Disable block */
        MPU6050_I2C_CFG_REG &= (uint8) ~MPU6050_I2C_CFG_EN_SLAVE;
        /* Wait for block reset before disable power */
        CyDelayCycles((uint32) blockResetCycles);

        /* Disable power to block */
        intState = CyEnterCriticalSection();
        MPU6050_I2C_ACT_PWRMGR_REG  &= (uint8) ~MPU6050_I2C_ACT_PWR_EN;
        MPU6050_I2C_STBY_PWRMGR_REG &= (uint8) ~MPU6050_I2C_STBY_PWR_EN;
        CyExitCriticalSection(intState);

        /* Enable block */
        MPU6050_I2C_CFG_REG |= (uint8) MPU6050_I2C_ENABLE_MS;

        /* Restore registers effected by block disable. Ticket ID#198004 */
        MPU6050_I2C_ADDR_REG    = MPU6050_I2C_backup.addr;
        MPU6050_I2C_ADDR_REG    = MPU6050_I2C_backup.addr;
        MPU6050_I2C_CLKDIV1_REG = MPU6050_I2C_backup.clkDiv1;
        MPU6050_I2C_CLKDIV2_REG = MPU6050_I2C_backup.clkDiv2;
    }
#else

    /* Disable slave or master bits */
    MPU6050_I2C_CFG_REG &= (uint8) ~MPU6050_I2C_ENABLE_MS;

#if (MPU6050_I2C_MODE_SLAVE_ENABLED)
    {
        /* Disable bit counter */
        uint8 intState = CyEnterCriticalSection();
        MPU6050_I2C_COUNTER_AUX_CTL_REG &= (uint8) ~MPU6050_I2C_CNT7_ENABLE;
        CyExitCriticalSection(intState);
    }
#endif /* (MPU6050_I2C_MODE_SLAVE_ENABLED) */

    /* Clear interrupt source register */
    (void) MPU6050_I2C_CSR_REG;
#endif /* (MPU6050_I2C_FF_IMPLEMENTED) */

    /* Disable interrupt on stop (enabled by write transaction) */
    MPU6050_I2C_DISABLE_INT_ON_STOP;
    MPU6050_I2C_ClearPendingInt();

    /* Reset FSM to default state */
    MPU6050_I2C_state = MPU6050_I2C_SM_IDLE;

    /* Clear busy statuses */
#if (MPU6050_I2C_MODE_SLAVE_ENABLED)
    MPU6050_I2C_slStatus &= (uint8) ~(MPU6050_I2C_SSTAT_RD_BUSY | MPU6050_I2C_SSTAT_WR_BUSY);
#endif /* (MPU6050_I2C_MODE_SLAVE_ENABLED) */
}


/* [] END OF FILE */
