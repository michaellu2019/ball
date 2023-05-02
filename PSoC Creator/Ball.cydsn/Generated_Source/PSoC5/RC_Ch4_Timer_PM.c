/*******************************************************************************
* File Name: RC_Ch4_Timer_PM.c
* Version 2.70
*
*  Description:
*     This file provides the power management source code to API for the
*     Timer.
*
*   Note:
*     None
*
*******************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
********************************************************************************/

#include "RC_Ch4_Timer.h"

static RC_Ch4_Timer_backupStruct RC_Ch4_Timer_backup;


/*******************************************************************************
* Function Name: RC_Ch4_Timer_SaveConfig
********************************************************************************
*
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
*  RC_Ch4_Timer_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void RC_Ch4_Timer_SaveConfig(void) 
{
    #if (!RC_Ch4_Timer_UsingFixedFunction)
        RC_Ch4_Timer_backup.TimerUdb = RC_Ch4_Timer_ReadCounter();
        RC_Ch4_Timer_backup.InterruptMaskValue = RC_Ch4_Timer_STATUS_MASK;
        #if (RC_Ch4_Timer_UsingHWCaptureCounter)
            RC_Ch4_Timer_backup.TimerCaptureCounter = RC_Ch4_Timer_ReadCaptureCount();
        #endif /* Back Up capture counter register  */

        #if(!RC_Ch4_Timer_UDB_CONTROL_REG_REMOVED)
            RC_Ch4_Timer_backup.TimerControlRegister = RC_Ch4_Timer_ReadControlRegister();
        #endif /* Backup the enable state of the Timer component */
    #endif /* Backup non retention registers in UDB implementation. All fixed function registers are retention */
}


/*******************************************************************************
* Function Name: RC_Ch4_Timer_RestoreConfig
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
*  RC_Ch4_Timer_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RC_Ch4_Timer_RestoreConfig(void) 
{   
    #if (!RC_Ch4_Timer_UsingFixedFunction)

        RC_Ch4_Timer_WriteCounter(RC_Ch4_Timer_backup.TimerUdb);
        RC_Ch4_Timer_STATUS_MASK =RC_Ch4_Timer_backup.InterruptMaskValue;
        #if (RC_Ch4_Timer_UsingHWCaptureCounter)
            RC_Ch4_Timer_SetCaptureCount(RC_Ch4_Timer_backup.TimerCaptureCounter);
        #endif /* Restore Capture counter register*/

        #if(!RC_Ch4_Timer_UDB_CONTROL_REG_REMOVED)
            RC_Ch4_Timer_WriteControlRegister(RC_Ch4_Timer_backup.TimerControlRegister);
        #endif /* Restore the enable state of the Timer component */
    #endif /* Restore non retention registers in the UDB implementation only */
}


/*******************************************************************************
* Function Name: RC_Ch4_Timer_Sleep
********************************************************************************
*
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
*  RC_Ch4_Timer_backup.TimerEnableState:  Is modified depending on the
*  enable state of the block before entering sleep mode.
*
*******************************************************************************/
void RC_Ch4_Timer_Sleep(void) 
{
    #if(!RC_Ch4_Timer_UDB_CONTROL_REG_REMOVED)
        /* Save Counter's enable state */
        if(RC_Ch4_Timer_CTRL_ENABLE == (RC_Ch4_Timer_CONTROL & RC_Ch4_Timer_CTRL_ENABLE))
        {
            /* Timer is enabled */
            RC_Ch4_Timer_backup.TimerEnableState = 1u;
        }
        else
        {
            /* Timer is disabled */
            RC_Ch4_Timer_backup.TimerEnableState = 0u;
        }
    #endif /* Back up enable state from the Timer control register */
    RC_Ch4_Timer_Stop();
    RC_Ch4_Timer_SaveConfig();
}


/*******************************************************************************
* Function Name: RC_Ch4_Timer_Wakeup
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
*  RC_Ch4_Timer_backup.enableState:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RC_Ch4_Timer_Wakeup(void) 
{
    RC_Ch4_Timer_RestoreConfig();
    #if(!RC_Ch4_Timer_UDB_CONTROL_REG_REMOVED)
        if(RC_Ch4_Timer_backup.TimerEnableState == 1u)
        {     /* Enable Timer's operation */
                RC_Ch4_Timer_Enable();
        } /* Do nothing if Timer was disabled before */
    #endif /* Remove this code section if Control register is removed */
}


/* [] END OF FILE */
