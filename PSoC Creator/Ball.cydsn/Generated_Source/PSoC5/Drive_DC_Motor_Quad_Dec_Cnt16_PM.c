/*******************************************************************************
* File Name: Drive_DC_Motor_Quad_Dec_Cnt16_PM.c  
* Version 3.0
*
*  Description:
*    This file provides the power management source code to API for the
*    Counter.  
*
*   Note:
*     None
*
********************************************************************************
* Copyright 2008-2012, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#include "Drive_DC_Motor_Quad_Dec_Cnt16.h"

static Drive_DC_Motor_Quad_Dec_Cnt16_backupStruct Drive_DC_Motor_Quad_Dec_Cnt16_backup;


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Cnt16_SaveConfig
********************************************************************************
* Summary:
*     Save the current user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Drive_DC_Motor_Quad_Dec_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Cnt16_SaveConfig(void) 
{
    #if (!Drive_DC_Motor_Quad_Dec_Cnt16_UsingFixedFunction)

        Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterUdb = Drive_DC_Motor_Quad_Dec_Cnt16_ReadCounter();

        #if(!Drive_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved)
            Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterControlRegister = Drive_DC_Motor_Quad_Dec_Cnt16_ReadControlRegister();
        #endif /* (!Drive_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved) */

    #endif /* (!Drive_DC_Motor_Quad_Dec_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Cnt16_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration.
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Drive_DC_Motor_Quad_Dec_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Cnt16_RestoreConfig(void) 
{      
    #if (!Drive_DC_Motor_Quad_Dec_Cnt16_UsingFixedFunction)

       Drive_DC_Motor_Quad_Dec_Cnt16_WriteCounter(Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterUdb);

        #if(!Drive_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved)
            Drive_DC_Motor_Quad_Dec_Cnt16_WriteControlRegister(Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterControlRegister);
        #endif /* (!Drive_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved) */

    #endif /* (!Drive_DC_Motor_Quad_Dec_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Cnt16_Sleep
********************************************************************************
* Summary:
*     Stop and Save the user configuration
*
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Drive_DC_Motor_Quad_Dec_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Cnt16_Sleep(void) 
{
    #if(!Drive_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Drive_DC_Motor_Quad_Dec_Cnt16_CTRL_ENABLE == (Drive_DC_Motor_Quad_Dec_Cnt16_CONTROL & Drive_DC_Motor_Quad_Dec_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState = 1u;
        if(Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState != 0u)
        {
            Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!Drive_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved) */
    
    Drive_DC_Motor_Quad_Dec_Cnt16_Stop();
    Drive_DC_Motor_Quad_Dec_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Cnt16_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration
*  
* Parameters:  
*  void
*
* Return: 
*  void
*
* Global variables:
*  Drive_DC_Motor_Quad_Dec_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Cnt16_Wakeup(void) 
{
    Drive_DC_Motor_Quad_Dec_Cnt16_RestoreConfig();
    #if(!Drive_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved)
        if(Drive_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Drive_DC_Motor_Quad_Dec_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Drive_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */
