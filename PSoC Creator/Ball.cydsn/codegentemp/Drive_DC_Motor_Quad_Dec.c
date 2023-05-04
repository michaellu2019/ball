/*******************************************************************************
* File Name: Drive_DC_Motor_Quad_Dec.c  
* Version 3.0
*
* Description:
*  This file provides the source code to the API for the Quadrature Decoder
*  component.
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

#if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
    #include "Drive_DC_Motor_Quad_Dec_PVT.h"
#endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */

uint8 Drive_DC_Motor_Quad_Dec_initVar = 0u;


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Init
********************************************************************************
*
* Summary:
*  Inits/Restores default QuadDec configuration provided with customizer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Init(void) 
{
    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
        /* Disable Interrupt. */
        CyIntDisable(Drive_DC_Motor_Quad_Dec_ISR_NUMBER);
        /* Set the ISR to point to the Drive_DC_Motor_Quad_Dec_isr Interrupt. */
        (void) CyIntSetVector(Drive_DC_Motor_Quad_Dec_ISR_NUMBER, & Drive_DC_Motor_Quad_Dec_ISR);
        /* Set the priority. */
        CyIntSetPriority(Drive_DC_Motor_Quad_Dec_ISR_NUMBER, Drive_DC_Motor_Quad_Dec_ISR_PRIORITY);
    #endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Enable
********************************************************************************
*
* Summary:
*  This function enable interrupts from Component and also enable Component's
*  ISR in case of 32-bit counter.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Enable(void) 
{
    uint8 enableInterrupts;

    Drive_DC_Motor_Quad_Dec_SetInterruptMask(Drive_DC_Motor_Quad_Dec_INIT_INT_MASK);

    /* Clear pending interrupts. */
    (void) Drive_DC_Motor_Quad_Dec_GetEvents();
    
    enableInterrupts = CyEnterCriticalSection();

    /* Enable interrupts from Statusi register */
    Drive_DC_Motor_Quad_Dec_SR_AUX_CONTROL |= Drive_DC_Motor_Quad_Dec_INTERRUPTS_ENABLE;

    CyExitCriticalSection(enableInterrupts);        

    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
        /* Enable Component interrupts */
        CyIntEnable(Drive_DC_Motor_Quad_Dec_ISR_NUMBER);
    #endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Start
********************************************************************************
*
* Summary:
*  Initializes UDBs and other relevant hardware.
*  Resets counter, enables or disables all relevant interrupts.
*  Starts monitoring the inputs and counting.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  Drive_DC_Motor_Quad_Dec_initVar - used to check initial configuration, modified on
*  first function call.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Start(void) 
{
    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
        Drive_DC_Motor_Quad_Dec_Cnt8_Start();
        Drive_DC_Motor_Quad_Dec_Cnt8_WriteCounter(Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE);
    #else
        /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) || 
        *  (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT) 
        */
        Drive_DC_Motor_Quad_Dec_Cnt16_Start();
        Drive_DC_Motor_Quad_Dec_Cnt16_WriteCounter(Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE);
    #endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT */
    
    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)        
       Drive_DC_Motor_Quad_Dec_count32SoftPart = 0;
    #endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */

    if (Drive_DC_Motor_Quad_Dec_initVar == 0u)
    {
        Drive_DC_Motor_Quad_Dec_Init();
        Drive_DC_Motor_Quad_Dec_initVar = 1u;
    }

    Drive_DC_Motor_Quad_Dec_Enable();
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_Stop
********************************************************************************
*
* Summary:
*  Turns off UDBs and other relevant hardware.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_Stop(void) 
{
    uint8 enableInterrupts;

    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
        Drive_DC_Motor_Quad_Dec_Cnt8_Stop();
    #else 
        /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) ||
        *  (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
        */
        Drive_DC_Motor_Quad_Dec_Cnt16_Stop();    /* counter disable */
    #endif /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT) */
 
    enableInterrupts = CyEnterCriticalSection();

    /* Disable interrupts interrupts from Statusi register */
    Drive_DC_Motor_Quad_Dec_SR_AUX_CONTROL &= (uint8) (~Drive_DC_Motor_Quad_Dec_INTERRUPTS_ENABLE);

    CyExitCriticalSection(enableInterrupts);

    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
        CyIntDisable(Drive_DC_Motor_Quad_Dec_ISR_NUMBER);    /* interrupt disable */
    #endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_GetCounter
********************************************************************************
*
* Summary:
*  Reports the current value of the counter.
*
* Parameters:
*  None.
*
* Return:
*  The counter value. Return type is signed and per the counter size setting.
*  A positive value indicates clockwise movement (B before A).
*
* Global variables:
*  Drive_DC_Motor_Quad_Dec_count32SoftPart - used to get hi 16 bit for current value
*  of the 32-bit counter, when Counter size equal 32-bit.
*
*******************************************************************************/
int16 Drive_DC_Motor_Quad_Dec_GetCounter(void) 
{
    int16 count;
    uint16 tmpCnt;

    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
        int16 hwCount;

        CyIntDisable(Drive_DC_Motor_Quad_Dec_ISR_NUMBER);

        tmpCnt = Drive_DC_Motor_Quad_Dec_Cnt16_ReadCounter();
        hwCount = (int16) ((int32) tmpCnt - (int32) Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE);
        count = Drive_DC_Motor_Quad_Dec_count32SoftPart + hwCount;

        CyIntEnable(Drive_DC_Motor_Quad_Dec_ISR_NUMBER);
    #else 
        /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT) || 
        *  (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT)
        */
        #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
            tmpCnt = Drive_DC_Motor_Quad_Dec_Cnt8_ReadCounter();
        #else /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) */
            tmpCnt = Drive_DC_Motor_Quad_Dec_Cnt16_ReadCounter();
        #endif  /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT */

        count = (int16) ((int32) tmpCnt -
                (int32) Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE);

    #endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */ 

    return (count);
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_SetCounter
********************************************************************************
*
* Summary:
*  Sets the current value of the counter.
*
* Parameters:
*  value:  The new value. Parameter type is signed and per the counter size
*  setting.
*
* Return:
*  None.
*
* Global variables:
*  Drive_DC_Motor_Quad_Dec_count32SoftPart - modified to set hi 16 bit for current
*  value of the 32-bit counter, when Counter size equal 32-bit.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_SetCounter(int16 value) 
{
    #if ((Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT) || \
         (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT))
        uint16 count;
        
        if (value >= 0)
        {
            count = (uint16) value + Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE;
        }
        else
        {
            count = Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE - (uint16)(-value);
        }
        #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
            Drive_DC_Motor_Quad_Dec_Cnt8_WriteCounter(count);
        #else /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) */
            Drive_DC_Motor_Quad_Dec_Cnt16_WriteCounter(count);
        #endif  /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT */
    #else /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT) */
        CyIntDisable(Drive_DC_Motor_Quad_Dec_ISR_NUMBER);

        Drive_DC_Motor_Quad_Dec_Cnt16_WriteCounter(Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE);
        Drive_DC_Motor_Quad_Dec_count32SoftPart = value;

        CyIntEnable(Drive_DC_Motor_Quad_Dec_ISR_NUMBER);
    #endif  /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT) ||
             * (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT)
             */
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_GetEvents
********************************************************************************
* 
* Summary:
*   Reports the current status of events. This function clears the bits of the 
*   status register.
*
* Parameters:
*  None.
*
* Return:
*  The events, as bits in an unsigned 8-bit value:
*    Bit      Description
*     0        Counter overflow.
*     1        Counter underflow.
*     2        Counter reset due to index, if index input is used.
*     3        Invalid A, B inputs state transition.
*
*******************************************************************************/
uint8 Drive_DC_Motor_Quad_Dec_GetEvents(void) 
{
    return (Drive_DC_Motor_Quad_Dec_STATUS_REG & Drive_DC_Motor_Quad_Dec_INIT_INT_MASK);
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_SetInterruptMask
********************************************************************************
*
* Summary:
*  Enables / disables interrupts due to the events.
*  For the 32-bit counter, the overflow, underflow and reset interrupts cannot
*  be disabled, these bits are ignored.
*
* Parameters:
*  mask: Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*
* Return:
*  None.
*
*******************************************************************************/
void Drive_DC_Motor_Quad_Dec_SetInterruptMask(uint8 mask) 
{
    #if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
        /* Underflow, Overflow and Reset interrupts for 32-bit Counter are always enable */
        mask |= (Drive_DC_Motor_Quad_Dec_COUNTER_OVERFLOW | Drive_DC_Motor_Quad_Dec_COUNTER_UNDERFLOW |
                 Drive_DC_Motor_Quad_Dec_COUNTER_RESET);
    #endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */

    Drive_DC_Motor_Quad_Dec_STATUS_MASK = mask;
}


/*******************************************************************************
* Function Name: Drive_DC_Motor_Quad_Dec_GetInterruptMask
********************************************************************************
*
* Summary:
*  Reports the current interrupt mask settings.
*
* Parameters:
*  None.
*
* Return:
*  Enable / disable bits in an 8-bit value, where 1 enables the interrupt.
*  For the 32-bit counter, the overflow, underflow and reset enable bits are
*  always set.
*
*******************************************************************************/
uint8 Drive_DC_Motor_Quad_Dec_GetInterruptMask(void) 
{
    return (Drive_DC_Motor_Quad_Dec_STATUS_MASK & Drive_DC_Motor_Quad_Dec_INIT_INT_MASK);
}


/* [] END OF FILE */
