/*******************************************************************************
* File Name: Head_Servo_PWM_PM.c
* Version 3.30
*
* Description:
*  This file provides the power management source code to API for the
*  PWM.
*
* Note:
*
********************************************************************************
* Copyright 2008-2014, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "Head_Servo_PWM.h"

static Head_Servo_PWM_backupStruct Head_Servo_PWM_backup;


/*******************************************************************************
* Function Name: Head_Servo_PWM_SaveConfig
********************************************************************************
*
* Summary:
*  Saves the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Head_Servo_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Head_Servo_PWM_SaveConfig(void) 
{

    #if(!Head_Servo_PWM_UsingFixedFunction)
        #if(!Head_Servo_PWM_PWMModeIsCenterAligned)
            Head_Servo_PWM_backup.PWMPeriod = Head_Servo_PWM_ReadPeriod();
        #endif /* (!Head_Servo_PWM_PWMModeIsCenterAligned) */
        Head_Servo_PWM_backup.PWMUdb = Head_Servo_PWM_ReadCounter();
        #if (Head_Servo_PWM_UseStatus)
            Head_Servo_PWM_backup.InterruptMaskValue = Head_Servo_PWM_STATUS_MASK;
        #endif /* (Head_Servo_PWM_UseStatus) */

        #if(Head_Servo_PWM_DeadBandMode == Head_Servo_PWM__B_PWM__DBM_256_CLOCKS || \
            Head_Servo_PWM_DeadBandMode == Head_Servo_PWM__B_PWM__DBM_2_4_CLOCKS)
            Head_Servo_PWM_backup.PWMdeadBandValue = Head_Servo_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Head_Servo_PWM_KillModeMinTime)
             Head_Servo_PWM_backup.PWMKillCounterPeriod = Head_Servo_PWM_ReadKillTime();
        #endif /* (Head_Servo_PWM_KillModeMinTime) */

        #if(Head_Servo_PWM_UseControl)
            Head_Servo_PWM_backup.PWMControlRegister = Head_Servo_PWM_ReadControlRegister();
        #endif /* (Head_Servo_PWM_UseControl) */
    #endif  /* (!Head_Servo_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Head_Servo_PWM_RestoreConfig
********************************************************************************
*
* Summary:
*  Restores the current user configuration of the component.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Head_Servo_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Head_Servo_PWM_RestoreConfig(void) 
{
        #if(!Head_Servo_PWM_UsingFixedFunction)
            #if(!Head_Servo_PWM_PWMModeIsCenterAligned)
                Head_Servo_PWM_WritePeriod(Head_Servo_PWM_backup.PWMPeriod);
            #endif /* (!Head_Servo_PWM_PWMModeIsCenterAligned) */

            Head_Servo_PWM_WriteCounter(Head_Servo_PWM_backup.PWMUdb);

            #if (Head_Servo_PWM_UseStatus)
                Head_Servo_PWM_STATUS_MASK = Head_Servo_PWM_backup.InterruptMaskValue;
            #endif /* (Head_Servo_PWM_UseStatus) */

            #if(Head_Servo_PWM_DeadBandMode == Head_Servo_PWM__B_PWM__DBM_256_CLOCKS || \
                Head_Servo_PWM_DeadBandMode == Head_Servo_PWM__B_PWM__DBM_2_4_CLOCKS)
                Head_Servo_PWM_WriteDeadTime(Head_Servo_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Head_Servo_PWM_KillModeMinTime)
                Head_Servo_PWM_WriteKillTime(Head_Servo_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Head_Servo_PWM_KillModeMinTime) */

            #if(Head_Servo_PWM_UseControl)
                Head_Servo_PWM_WriteControlRegister(Head_Servo_PWM_backup.PWMControlRegister);
            #endif /* (Head_Servo_PWM_UseControl) */
        #endif  /* (!Head_Servo_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Head_Servo_PWM_Sleep
********************************************************************************
*
* Summary:
*  Disables block's operation and saves the user configuration. Should be called
*  just prior to entering sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Head_Servo_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Head_Servo_PWM_Sleep(void) 
{
    #if(Head_Servo_PWM_UseControl)
        if(Head_Servo_PWM_CTRL_ENABLE == (Head_Servo_PWM_CONTROL & Head_Servo_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Head_Servo_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Head_Servo_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Head_Servo_PWM_UseControl) */

    /* Stop component */
    Head_Servo_PWM_Stop();

    /* Save registers configuration */
    Head_Servo_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Head_Servo_PWM_Wakeup
********************************************************************************
*
* Summary:
*  Restores and enables the user configuration. Should be called just after
*  awaking from sleep.
*
* Parameters:
*  None
*
* Return:
*  None
*
* Global variables:
*  Head_Servo_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Head_Servo_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Head_Servo_PWM_RestoreConfig();

    if(Head_Servo_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Head_Servo_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
