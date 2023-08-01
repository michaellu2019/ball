/*******************************************************************************
* File Name: Motor_A_PWM_PM.c
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

#include "Motor_A_PWM.h"

static Motor_A_PWM_backupStruct Motor_A_PWM_backup;


/*******************************************************************************
* Function Name: Motor_A_PWM_SaveConfig
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
*  Motor_A_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Motor_A_PWM_SaveConfig(void) 
{

    #if(!Motor_A_PWM_UsingFixedFunction)
        #if(!Motor_A_PWM_PWMModeIsCenterAligned)
            Motor_A_PWM_backup.PWMPeriod = Motor_A_PWM_ReadPeriod();
        #endif /* (!Motor_A_PWM_PWMModeIsCenterAligned) */
        Motor_A_PWM_backup.PWMUdb = Motor_A_PWM_ReadCounter();
        #if (Motor_A_PWM_UseStatus)
            Motor_A_PWM_backup.InterruptMaskValue = Motor_A_PWM_STATUS_MASK;
        #endif /* (Motor_A_PWM_UseStatus) */

        #if(Motor_A_PWM_DeadBandMode == Motor_A_PWM__B_PWM__DBM_256_CLOCKS || \
            Motor_A_PWM_DeadBandMode == Motor_A_PWM__B_PWM__DBM_2_4_CLOCKS)
            Motor_A_PWM_backup.PWMdeadBandValue = Motor_A_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Motor_A_PWM_KillModeMinTime)
             Motor_A_PWM_backup.PWMKillCounterPeriod = Motor_A_PWM_ReadKillTime();
        #endif /* (Motor_A_PWM_KillModeMinTime) */

        #if(Motor_A_PWM_UseControl)
            Motor_A_PWM_backup.PWMControlRegister = Motor_A_PWM_ReadControlRegister();
        #endif /* (Motor_A_PWM_UseControl) */
    #endif  /* (!Motor_A_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Motor_A_PWM_RestoreConfig
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
*  Motor_A_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Motor_A_PWM_RestoreConfig(void) 
{
        #if(!Motor_A_PWM_UsingFixedFunction)
            #if(!Motor_A_PWM_PWMModeIsCenterAligned)
                Motor_A_PWM_WritePeriod(Motor_A_PWM_backup.PWMPeriod);
            #endif /* (!Motor_A_PWM_PWMModeIsCenterAligned) */

            Motor_A_PWM_WriteCounter(Motor_A_PWM_backup.PWMUdb);

            #if (Motor_A_PWM_UseStatus)
                Motor_A_PWM_STATUS_MASK = Motor_A_PWM_backup.InterruptMaskValue;
            #endif /* (Motor_A_PWM_UseStatus) */

            #if(Motor_A_PWM_DeadBandMode == Motor_A_PWM__B_PWM__DBM_256_CLOCKS || \
                Motor_A_PWM_DeadBandMode == Motor_A_PWM__B_PWM__DBM_2_4_CLOCKS)
                Motor_A_PWM_WriteDeadTime(Motor_A_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Motor_A_PWM_KillModeMinTime)
                Motor_A_PWM_WriteKillTime(Motor_A_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Motor_A_PWM_KillModeMinTime) */

            #if(Motor_A_PWM_UseControl)
                Motor_A_PWM_WriteControlRegister(Motor_A_PWM_backup.PWMControlRegister);
            #endif /* (Motor_A_PWM_UseControl) */
        #endif  /* (!Motor_A_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Motor_A_PWM_Sleep
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
*  Motor_A_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Motor_A_PWM_Sleep(void) 
{
    #if(Motor_A_PWM_UseControl)
        if(Motor_A_PWM_CTRL_ENABLE == (Motor_A_PWM_CONTROL & Motor_A_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Motor_A_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Motor_A_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Motor_A_PWM_UseControl) */

    /* Stop component */
    Motor_A_PWM_Stop();

    /* Save registers configuration */
    Motor_A_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Motor_A_PWM_Wakeup
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
*  Motor_A_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Motor_A_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Motor_A_PWM_RestoreConfig();

    if(Motor_A_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Motor_A_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
