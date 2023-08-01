/*******************************************************************************
* File Name: Drive_DC_Motor_PWM_PM.c
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

#include "Drive_DC_Motor_PWM.h"

static Drive_DC_Motor_PWM_backupStruct Drive_DC_Motor_PWM_backup;


/*******************************************************************************
* Function Name: Drive_DC_Motor_PWM_SaveConfig
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
*  Drive_DC_Motor_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Drive_DC_Motor_PWM_SaveConfig(void) 
{

    #if(!Drive_DC_Motor_PWM_UsingFixedFunction)
        #if(!Drive_DC_Motor_PWM_PWMModeIsCenterAligned)
            Drive_DC_Motor_PWM_backup.PWMPeriod = Drive_DC_Motor_PWM_ReadPeriod();
        #endif /* (!Drive_DC_Motor_PWM_PWMModeIsCenterAligned) */
        Drive_DC_Motor_PWM_backup.PWMUdb = Drive_DC_Motor_PWM_ReadCounter();
        #if (Drive_DC_Motor_PWM_UseStatus)
            Drive_DC_Motor_PWM_backup.InterruptMaskValue = Drive_DC_Motor_PWM_STATUS_MASK;
        #endif /* (Drive_DC_Motor_PWM_UseStatus) */

        #if(Drive_DC_Motor_PWM_DeadBandMode == Drive_DC_Motor_PWM__B_PWM__DBM_256_CLOCKS || \
            Drive_DC_Motor_PWM_DeadBandMode == Drive_DC_Motor_PWM__B_PWM__DBM_2_4_CLOCKS)
            Drive_DC_Motor_PWM_backup.PWMdeadBandValue = Drive_DC_Motor_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Drive_DC_Motor_PWM_KillModeMinTime)
             Drive_DC_Motor_PWM_backup.PWMKillCounterPeriod = Drive_DC_Motor_PWM_ReadKillTime();
        #endif /* (Drive_DC_Motor_PWM_KillModeMinTime) */

        #if(Drive_DC_Motor_PWM_UseControl)
            Drive_DC_Motor_PWM_backup.PWMControlRegister = Drive_DC_Motor_PWM_ReadControlRegister();
        #endif /* (Drive_DC_Motor_PWM_UseControl) */
    #endif  /* (!Drive_DC_Motor_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_PWM_RestoreConfig
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
*  Drive_DC_Motor_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Drive_DC_Motor_PWM_RestoreConfig(void) 
{
        #if(!Drive_DC_Motor_PWM_UsingFixedFunction)
            #if(!Drive_DC_Motor_PWM_PWMModeIsCenterAligned)
                Drive_DC_Motor_PWM_WritePeriod(Drive_DC_Motor_PWM_backup.PWMPeriod);
            #endif /* (!Drive_DC_Motor_PWM_PWMModeIsCenterAligned) */

            Drive_DC_Motor_PWM_WriteCounter(Drive_DC_Motor_PWM_backup.PWMUdb);

            #if (Drive_DC_Motor_PWM_UseStatus)
                Drive_DC_Motor_PWM_STATUS_MASK = Drive_DC_Motor_PWM_backup.InterruptMaskValue;
            #endif /* (Drive_DC_Motor_PWM_UseStatus) */

            #if(Drive_DC_Motor_PWM_DeadBandMode == Drive_DC_Motor_PWM__B_PWM__DBM_256_CLOCKS || \
                Drive_DC_Motor_PWM_DeadBandMode == Drive_DC_Motor_PWM__B_PWM__DBM_2_4_CLOCKS)
                Drive_DC_Motor_PWM_WriteDeadTime(Drive_DC_Motor_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Drive_DC_Motor_PWM_KillModeMinTime)
                Drive_DC_Motor_PWM_WriteKillTime(Drive_DC_Motor_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Drive_DC_Motor_PWM_KillModeMinTime) */

            #if(Drive_DC_Motor_PWM_UseControl)
                Drive_DC_Motor_PWM_WriteControlRegister(Drive_DC_Motor_PWM_backup.PWMControlRegister);
            #endif /* (Drive_DC_Motor_PWM_UseControl) */
        #endif  /* (!Drive_DC_Motor_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Drive_DC_Motor_PWM_Sleep
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
*  Drive_DC_Motor_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Drive_DC_Motor_PWM_Sleep(void) 
{
    #if(Drive_DC_Motor_PWM_UseControl)
        if(Drive_DC_Motor_PWM_CTRL_ENABLE == (Drive_DC_Motor_PWM_CONTROL & Drive_DC_Motor_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Drive_DC_Motor_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Drive_DC_Motor_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Drive_DC_Motor_PWM_UseControl) */

    /* Stop component */
    Drive_DC_Motor_PWM_Stop();

    /* Save registers configuration */
    Drive_DC_Motor_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_PWM_Wakeup
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
*  Drive_DC_Motor_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Drive_DC_Motor_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Drive_DC_Motor_PWM_RestoreConfig();

    if(Drive_DC_Motor_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Drive_DC_Motor_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
