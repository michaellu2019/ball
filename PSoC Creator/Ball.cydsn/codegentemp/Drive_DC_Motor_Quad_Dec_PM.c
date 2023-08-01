/*******************************************************************************
* File Name: Drive_DC_Motor_Quad_Dec_PM.c
* Version 3.0
*
* Description:
*  This file contains the setup, control and status commands to support 
*  component operations in low power mode.  
*
* Note:
*  None.
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Drive_DC_Motor_Quad_Dec.h"

static Drive_DC_Motor_Quad_Dec_BACKUP_STRUCT Drive_DC_Motor_Quad_Dec_backup = {0u};


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_SaveConfig
********************************************************************************
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_SaveConfig(void) 
{
    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
        Drive_DC_Motor_Quad_Dec_Cnt8_SaveConfig();
    #else 
        /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) || 
         * (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
         */
        Drive_DC_Motor_Quad_Dec_Cnt16_SaveConfig();
    #endif /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_RestoreConfig
********************************************************************************
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_RestoreConfig(void) 
{
    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
        Drive_DC_Motor_Quad_Dec_Cnt8_RestoreConfig();
    #else 
        /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) || 
         * (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT) 
         */
        Drive_DC_Motor_Quad_Dec_Cnt16_RestoreConfig();
    #endif /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT) */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Sleep
********************************************************************************
* 
* Summary:
*  Prepare Quadrature Decoder Component goes to sleep.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Drive_DC_Motor_Quad_Dec_backup - modified when non-retention registers are saved.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Sleep(void) 
{
    if (0u != (Drive_DC_Motor_Quad_Dec_SR_AUX_CONTROL & Drive_DC_Motor_Quad_Dec_INTERRUPTS_ENABLE))
    {
        Drive_DC_Motor_Quad_Dec_backup.enableState = 1u;
    }
    else /* The Quadrature Decoder Component is disabled */
    {
        Drive_DC_Motor_Quad_Dec_backup.enableState = 0u;
    }

    Drive_DC_Motor_Quad_Dec_Stop();
    Drive_DC_Motor_Quad_Dec_SaveConfig();
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Wakeup
********************************************************************************
*
* Summary:
*  Prepare Quadrature Decoder Component to wake up.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global Variables:
*  Drive_DC_Motor_Quad_Dec_backup - used when non-retention registers are restored.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Wakeup(void) 
{
    Drive_DC_Motor_Quad_Dec_RestoreConfig();

    if (Drive_DC_Motor_Quad_Dec_backup.enableState != 0u)
    {
        #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
            Drive_DC_Motor_Quad_Dec_Cnt8_Enable();
        #else 
            /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) || 
            *  (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT) 
            */
            Drive_DC_Motor_Quad_Dec_Cnt16_Enable();
        #endif /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT) */

        /* Enable component's operation */
        Drive_DC_Motor_Quad_Dec_Enable();
    } /* Do nothing if component's block was disabled before */
}


/* [] END OF FILE */

