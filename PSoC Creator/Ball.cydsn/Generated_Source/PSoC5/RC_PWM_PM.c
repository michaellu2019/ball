/*******************************************************************************
* File Name: RC_PWM_PM.c
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

#include "RC_PWM.h"

static RC_PWM_backupStruct RC_PWM_backup;


/*******************************************************************************
* Function Name: RC_PWM_SaveConfig
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
*  RC_PWM_backup:  Variables of this global structure are modified to
*  store the values of non retention configuration registers when Sleep() API is
*  called.
*
*******************************************************************************/
void RC_PWM_SaveConfig(void) 
{

    #if(!RC_PWM_UsingFixedFunction)
        #if(!RC_PWM_PWMModeIsCenterAligned)
            RC_PWM_backup.PWMPeriod = RC_PWM_ReadPeriod();
        #endif /* (!RC_PWM_PWMModeIsCenterAligned) */
        RC_PWM_backup.PWMUdb = RC_PWM_ReadCounter();
        #if (RC_PWM_UseStatus)
            RC_PWM_backup.InterruptMaskValue = RC_PWM_STATUS_MASK;
        #endif /* (RC_PWM_UseStatus) */

        #if(RC_PWM_DeadBandMode == RC_PWM__B_PWM__DBM_256_CLOCKS || \
            RC_PWM_DeadBandMode == RC_PWM__B_PWM__DBM_2_4_CLOCKS)
            RC_PWM_backup.PWMdeadBandValue = RC_PWM_ReadDeadTime();
        #endif /*  deadband count is either 2-4 clocks or 256 clocks */

        #if(RC_PWM_KillModeMinTime)
             RC_PWM_backup.PWMKillCounterPeriod = RC_PWM_ReadKillTime();
        #endif /* (RC_PWM_KillModeMinTime) */

        #if(RC_PWM_UseControl)
            RC_PWM_backup.PWMControlRegister = RC_PWM_ReadControlRegister();
        #endif /* (RC_PWM_UseControl) */
    #endif  /* (!RC_PWM_UsingFixedFunction) */
}


/*******************************************************************************
* Function Name: RC_PWM_RestoreConfig
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
*  RC_PWM_backup:  Variables of this global structure are used to
*  restore the values of non retention registers on wakeup from sleep mode.
*
*******************************************************************************/
void RC_PWM_RestoreConfig(void) 
{
        #if(!RC_PWM_UsingFixedFunction)
            #if(!RC_PWM_PWMModeIsCenterAligned)
                RC_PWM_WritePeriod(RC_PWM_backup.PWMPeriod);
            #endif /* (!RC_PWM_PWMModeIsCenterAligned) */

            RC_PWM_WriteCounter(RC_PWM_backup.PWMUdb);

            #if (RC_PWM_UseStatus)
                RC_PWM_STATUS_MASK = RC_PWM_backup.InterruptMaskValue;
            #endif /* (RC_PWM_UseStatus) */

            #if(RC_PWM_DeadBandMode == RC_PWM__B_PWM__DBM_256_CLOCKS || \
                RC_PWM_DeadBandMode == RC_PWM__B_PWM__DBM_2_4_CLOCKS)
                RC_PWM_WriteDeadTime(RC_PWM_backup.PWMdeadBandValue);
            #endif /* deadband count is either 2-4 clocks or 256 clocks */

            #if(RC_PWM_KillModeMinTime)
                RC_PWM_WriteKillTime(RC_PWM_backup.PWMKillCounterPeriod);
            #endif /* (RC_PWM_KillModeMinTime) */

            #if(RC_PWM_UseControl)
                RC_PWM_WriteControlRegister(RC_PWM_backup.PWMControlRegister);
            #endif /* (RC_PWM_UseControl) */
        #endif  /* (!RC_PWM_UsingFixedFunction) */
    }


/*******************************************************************************
* Function Name: RC_PWM_Sleep
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
*  RC_PWM_backup.PWMEnableState:  Is modified depending on the enable
*  state of the block before entering sleep mode.
*
*******************************************************************************/
void RC_PWM_Sleep(void) 
{
    #if(RC_PWM_UseControl)
        if(RC_PWM_CTRL_ENABLE == (RC_PWM_CONTROL & RC_PWM_CTRL_ENABLE))
        {
            /*Component is enabled */
            RC_PWM_backup.PWMEnableState = 1u;
        }
        else
        {
            /* Component is disabled */
            RC_PWM_backup.PWMEnableState = 0u;
        }
    #endif /* (RC_PWM_UseControl) */

    /* Stop component */
    RC_PWM_Stop();

    /* Save registers configuration */
    RC_PWM_SaveConfig();
}


/*******************************************************************************
* Function Name: RC_PWM_Wakeup
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
*  RC_PWM_backup.pwmEnable:  Is used to restore the enable state of
*  block on wakeup from sleep mode.
*
*******************************************************************************/
void RC_PWM_Wakeup(void) 
{
     /* Restore registers values */
    RC_PWM_RestoreConfig();

    if(RC_PWM_backup.PWMEnableState != 0u)
    {
        /* Enable component's operation */
        RC_PWM_Enable();
    } /* Do nothing if component's block was disabled before */

}


/* [] END OF FILE */
