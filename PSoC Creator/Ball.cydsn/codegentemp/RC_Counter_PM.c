/*******************************************************************************
* File Name: RC_Counter_PM.c  
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

#include "RC_Counter.h"

static RC_Counter_backupStruct RC_Counter_backup;


/*******************************************************************************
* Function Name: RC_Counter_SaveConfig
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
*  RC_Counter_backup:  Variables of this global structure are modified to 
*  store the values of non retention configuration registers when Sleep() API is 
*  called.
*
*******************************************************************************/
void RC_Counter_SaveConfig(void) 
{
    #if (!RC_Counter_UsingFixedFunction)

        RC_Counter_backup.CounterUdb = RC_Counter_ReadCounter();

        #if(!RC_Counter_ControlRegRemoved)
            RC_Counter_backup.CounterControlRegister = RC_Counter_ReadControlRegister();
        #endif /* (!RC_Counter_ControlRegRemoved) */

    #endif /* (!RC_Counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RC_Counter_RestoreConfig
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
*  RC_Counter_backup:  Variables of this global structure are used to 
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RC_Counter_RestoreConfig(void) 
{      
    #if (!RC_Counter_UsingFixedFunction)

       RC_Counter_WriteCounter(RC_Counter_backup.CounterUdb);

        #if(!RC_Counter_ControlRegRemoved)
            RC_Counter_WriteControlRegister(RC_Counter_backup.CounterControlRegister);
        #endif /* (!RC_Counter_ControlRegRemoved) */

    #endif /* (!RC_Counter_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RC_Counter_Sleep
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
*  RC_Counter_backup.enableState:  Is modified depending on the enable 
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void RC_Counter_Sleep(void) 
{
    #if(!RC_Counter_ControlRegRemoved)
        /* Save Counter's enable state */
        if(RC_Counter_CTRL_ENABLE == (RC_Counter_CONTROL & RC_Counter_CTRL_ENABLE))
        {
            /* Counter is enabled */
            RC_Counter_backup.CounterEnableState = 1u;
        }
        else
        {
            /* Counter is disabled */
            RC_Counter_backup.CounterEnableState = 0u;
        }
    #else
        RC_Counter_backup.CounterEnableState = 1u;
        if(RC_Counter_backup.CounterEnableState != 0u)
        {
            RC_Counter_backup.CounterEnableState = 0u;
        }
    #endif /* (!RC_Counter_ControlRegRemoved) */
    
    RC_Counter_Stop();
    RC_Counter_SaveConfig();
}


/*******************************************************************************
* Function Name: RC_Counter_Wakeup
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
*  RC_Counter_backup.enableState:  Is used to restore the enable state of 
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RC_Counter_Wakeup(void) 
{
    RC_Counter_RestoreConfig();
    #if(!RC_Counter_ControlRegRemoved)
        if(RC_Counter_backup.CounterEnableState == 1u)
        {
            /* Enable Counter's operation */
            RC_Counter_Enable();
        } /* Do nothing if Counter was disabled before */    
    #endif /* (!RC_Counter_ControlRegRemoved) */
    
}


/* [] END OF FILE */
