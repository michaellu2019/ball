/*******************************************************************************
* File Name: Pendulum_DC_Motor_Quad_Dec_Cnt16_PM.c  
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

#include "Pendulum_DC_Motor_Quad_Dec_Cnt16.h"

static Pendulum_DC_Motor_Quad_Dec_Cnt16_backupStruct Pendulum_DC_Motor_Quad_Dec_Cnt16_backup;


/*******************************************************************************
* Function Name: Pendulum_DC_Motor_Quad_Dec_Cnt16_SaveConfig
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
*  Pendulum_DC_Motor_Quad_Dec_Cnt16_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void Pendulum_DC_Motor_Quad_Dec_Cnt16_SaveConfig(void) 
{
    #if (!Pendulum_DC_Motor_Quad_Dec_Cnt16_UsingFixedFunction)

        Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterUdb = Pendulum_DC_Motor_Quad_Dec_Cnt16_ReadCounter();

        #if(!Pendulum_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved)
            Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterControlRegister = Pendulum_DC_Motor_Quad_Dec_Cnt16_ReadControlRegister();
        #endif /* (!Pendulum_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved) */

    #endif /* (!Pendulum_DC_Motor_Quad_Dec_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Pendulum_DC_Motor_Quad_Dec_Cnt16_RestoreConfig
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
*  Pendulum_DC_Motor_Quad_Dec_Cnt16_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Pendulum_DC_Motor_Quad_Dec_Cnt16_RestoreConfig(void) 
{      
    #if (!Pendulum_DC_Motor_Quad_Dec_Cnt16_UsingFixedFunction)

       Pendulum_DC_Motor_Quad_Dec_Cnt16_WriteCounter(Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterUdb);

        #if(!Pendulum_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved)
            Pendulum_DC_Motor_Quad_Dec_Cnt16_WriteControlRegister(Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterControlRegister);
        #endif /* (!Pendulum_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved) */

    #endif /* (!Pendulum_DC_Motor_Quad_Dec_Cnt16_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Pendulum_DC_Motor_Quad_Dec_Cnt16_Sleep
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
*  Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Pendulum_DC_Motor_Quad_Dec_Cnt16_Sleep(void) 
{
    #if(!Pendulum_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved)
        /* Save Counter's enable state */
        if(Pendulum_DC_Motor_Quad_Dec_Cnt16_CTRL_ENABLE == (Pendulum_DC_Motor_Quad_Dec_Cnt16_CONTROL & Pendulum_DC_Motor_Quad_Dec_Cnt16_CTRL_ENABLE))
        {
            /* Counter is enabled */
            Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState = 0u;
        }
    #else
        Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState = 1u;
        if(Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState != 0u)
        {
            Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState = 0u;
        }
    #endif /* (!Pendulum_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved) */
    
    Pendulum_DC_Motor_Quad_Dec_Cnt16_Stop();
    Pendulum_DC_Motor_Quad_Dec_Cnt16_SaveConfig();
}


/*******************************************************************************
* Function Name: Pendulum_DC_Motor_Quad_Dec_Cnt16_Wakeup
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
*  Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Pendulum_DC_Motor_Quad_Dec_Cnt16_Wakeup(void) 
{
    Pendulum_DC_Motor_Quad_Dec_Cnt16_RestoreConfig();
    #if(!Pendulum_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved)
        if(Pendulum_DC_Motor_Quad_Dec_Cnt16_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            Pendulum_DC_Motor_Quad_Dec_Cnt16_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!Pendulum_DC_Motor_Quad_Dec_Cnt16_ControlRegRemoved) */
    
}


/* [] END OF FILE */
