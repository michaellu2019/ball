/*******************************************************************************
* File Name: Flywheel_DC_Motor_PWM_PM.c
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

#include "Flywheel_DC_Motor_PWM.h"

static Flywheel_DC_Motor_PWM_backupStruct Flywheel_DC_Motor_PWM_backup;


/*******************************************************************************
* Function Name: Flywheel_DC_Motor_PWM_SaveConfig
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
*  Flywheel_DC_Motor_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void Flywheel_DC_Motor_PWM_SaveConfig(void) 
{

    #if(!Flywheel_DC_Motor_PWM_UsingFixedFunction)
        #if(!Flywheel_DC_Motor_PWM_PWMModeIsCenterAligned)
            Flywheel_DC_Motor_PWM_backup.PWMPeriod = Flywheel_DC_Motor_PWM_ReadPeriod();
        #endif /* (!Flywheel_DC_Motor_PWM_PWMModeIsCenterAligned) */
        Flywheel_DC_Motor_PWM_backup.PWMUdb = Flywheel_DC_Motor_PWM_ReadCounter();
        #if (Flywheel_DC_Motor_PWM_UseStatus)
            Flywheel_DC_Motor_PWM_backup.InterruptMaskValue = Flywheel_DC_Motor_PWM_STATUS_MASK;
        #endif /* (Flywheel_DC_Motor_PWM_UseStatus) */

        #if(Flywheel_DC_Motor_PWM_DeadBandMode == Flywheel_DC_Motor_PWM__B_PWM__DBM_256_CLOCKS || \
            Flywheel_DC_Motor_PWM_DeadBandMode == Flywheel_DC_Motor_PWM__B_PWM__DBM_2_4_CLOCKS)
            Flywheel_DC_Motor_PWM_backup.PWMdeadBandValue = Flywheel_DC_Motor_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(Flywheel_DC_Motor_PWM_KillModeMinTime)
             Flywheel_DC_Motor_PWM_backup.PWMKillCounterPeriod = Flywheel_DC_Motor_PWM_ReadKillTime();
        #endif /* (Flywheel_DC_Motor_PWM_KillModeMinTime) */

        #if(Flywheel_DC_Motor_PWM_UseControl)
            Flywheel_DC_Motor_PWM_backup.PWMControlRegister = Flywheel_DC_Motor_PWM_ReadControlRegister();
        #endif /* (Flywheel_DC_Motor_PWM_UseControl) */
    #endif  /* (!Flywheel_DC_Motor_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: Flywheel_DC_Motor_PWM_RestoreConfig
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
*  Flywheel_DC_Motor_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void Flywheel_DC_Motor_PWM_RestoreConfig(void) 
{
        #if(!Flywheel_DC_Motor_PWM_UsingFixedFunction)
            #if(!Flywheel_DC_Motor_PWM_PWMModeIsCenterAligned)
                Flywheel_DC_Motor_PWM_WritePeriod(Flywheel_DC_Motor_PWM_backup.PWMPeriod);
            #endif /* (!Flywheel_DC_Motor_PWM_PWMModeIsCenterAligned) */

            Flywheel_DC_Motor_PWM_WriteCounter(Flywheel_DC_Motor_PWM_backup.PWMUdb);

            #if (Flywheel_DC_Motor_PWM_UseStatus)
                Flywheel_DC_Motor_PWM_STATUS_MASK = Flywheel_DC_Motor_PWM_backup.InterruptMaskValue;
            #endif /* (Flywheel_DC_Motor_PWM_UseStatus) */

            #if(Flywheel_DC_Motor_PWM_DeadBandMode == Flywheel_DC_Motor_PWM__B_PWM__DBM_256_CLOCKS || \
                Flywheel_DC_Motor_PWM_DeadBandMode == Flywheel_DC_Motor_PWM__B_PWM__DBM_2_4_CLOCKS)
                Flywheel_DC_Motor_PWM_WriteDeadTime(Flywheel_DC_Motor_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(Flywheel_DC_Motor_PWM_KillModeMinTime)
                Flywheel_DC_Motor_PWM_WriteKillTime(Flywheel_DC_Motor_PWM_backup.PWMKillCounterPeriod);
            #endif /* (Flywheel_DC_Motor_PWM_KillModeMinTime) */

            #if(Flywheel_DC_Motor_PWM_UseControl)
                Flywheel_DC_Motor_PWM_WriteControlRegister(Flywheel_DC_Motor_PWM_backup.PWMControlRegister);
            #endif /* (Flywheel_DC_Motor_PWM_UseControl) */
        #endif  /* (!Flywheel_DC_Motor_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: Flywheel_DC_Motor_PWM_Sleep
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
*  Flywheel_DC_Motor_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void Flywheel_DC_Motor_PWM_Sleep(void) 
{
    #if(Flywheel_DC_Motor_PWM_UseControl)
        if(Flywheel_DC_Motor_PWM_CTRL_ENABLE == (Flywheel_DC_Motor_PWM_CONTROL & Flywheel_DC_Motor_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            Flywheel_DC_Motor_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            Flywheel_DC_Motor_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (Flywheel_DC_Motor_PWM_UseControl) */

    /* Stop component */
    Flywheel_DC_Motor_PWM_Stop();

    /* Save registers configuration */
    Flywheel_DC_Motor_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: Flywheel_DC_Motor_PWM_Wakeup
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
*  Flywheel_DC_Motor_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void Flywheel_DC_Motor_PWM_Wakeup(void) 
{
     /* Restore registers values */
    Flywheel_DC_Motor_PWM_RestoreConfig();

    if(Flywheel_DC_Motor_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        Flywheel_DC_Motor_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
