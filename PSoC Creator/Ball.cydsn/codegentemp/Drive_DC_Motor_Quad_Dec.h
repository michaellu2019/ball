/*******************************************************************************
* File Name: Drive_DC_Motor_Quad_Dec.h  
* Version 3.0
*
* Description:
*  This file provides constants and parameter values for the Quadrature
*  Decoder component.
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

#if !defined(CY_QUADRATURE_DECODER_Drive_DC_Motor_Quad_Dec_H)
#define CY_QUADRATURE_DECODER_Drive_DC_Motor_Quad_Dec_H

#include "cyfitter.h"
#include "CyLib.h"
#include "cytypes.h"

/* Check to see if required defines such as CY_PSOC5LP are available */
/* They are defined starting with cy_boot v3.0 */
#if !defined (CY_PSOC5LP)
    #error Component QuadDec_v3_0 requires cy_boot v3.0 or later
#endif /* (CY_PSOC5LP) */

#define Drive_DC_Motor_Quad_Dec_COUNTER_SIZE               (16u)
#define Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT         (8u)
#define Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT        (16u)
#define Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT        (32u)

#if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
    #include "Drive_DC_Motor_Quad_Dec_Cnt8.h"
#else 
    /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) || 
    *  (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT) 
    */
    #include "Drive_DC_Motor_Quad_Dec_Cnt16.h"
#endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT */

extern uint8 Drive_DC_Motor_Quad_Dec_initVar;


/***************************************
*   Conditional Compilation Parameters
***************************************/

#define Drive_DC_Motor_Quad_Dec_COUNTER_RESOLUTION         (1u)


/***************************************
*       Data Struct Definition
***************************************/

/* Sleep Mode API Support */
typedef struct
{
    uint8 enableState;
} Drive_DC_Motor_Quad_Dec_BACKUP_STRUCT;


/***************************************
*        Function Prototypes
***************************************/

void  Drive_DC_Motor_Quad_Dec_Init(void) ;
void  Drive_DC_Motor_Quad_Dec_Start(void) ;
void  Drive_DC_Motor_Quad_Dec_Stop(void) ;
void  Drive_DC_Motor_Quad_Dec_Enable(void) ;
uint8 Drive_DC_Motor_Quad_Dec_GetEvents(void) ;
void  Drive_DC_Motor_Quad_Dec_SetInterruptMask(uint8 mask) ;
uint8 Drive_DC_Motor_Quad_Dec_GetInterruptMask(void) ;
int16 Drive_DC_Motor_Quad_Dec_GetCounter(void) ;
void  Drive_DC_Motor_Quad_Dec_SetCounter(int16 value)
;
void  Drive_DC_Motor_Quad_Dec_Sleep(void) ;
void  Drive_DC_Motor_Quad_Dec_Wakeup(void) ;
void  Drive_DC_Motor_Quad_Dec_SaveConfig(void) ;
void  Drive_DC_Motor_Quad_Dec_RestoreConfig(void) ;

#if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
    CY_ISR_PROTO(Drive_DC_Motor_Quad_Dec_ISR);
#endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */


/***************************************
*           API Constants
***************************************/

#if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
    #define Drive_DC_Motor_Quad_Dec_ISR_NUMBER             ((uint8) Drive_DC_Motor_Quad_Dec_isr__INTC_NUMBER)
    #define Drive_DC_Motor_Quad_Dec_ISR_PRIORITY           ((uint8) Drive_DC_Motor_Quad_Dec_isr__INTC_PRIOR_NUM)
#endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT */


/***************************************
*    Enumerated Types and Parameters
***************************************/

#define Drive_DC_Motor_Quad_Dec_GLITCH_FILTERING           (1u)
#define Drive_DC_Motor_Quad_Dec_INDEX_INPUT                (0u)


/***************************************
*    Initial Parameter Constants
***************************************/

#if (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT)
    #define Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE    (0x80u)
#else 
    /* (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_16_BIT) ||
    *  (Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_32_BIT)
    */
    #define Drive_DC_Motor_Quad_Dec_COUNTER_INIT_VALUE    (0x8000u)
    #define Drive_DC_Motor_Quad_Dec_COUNTER_MAX_VALUE     (0x7FFFu)
#endif /* Drive_DC_Motor_Quad_Dec_COUNTER_SIZE == Drive_DC_Motor_Quad_Dec_COUNTER_SIZE_8_BIT */


/***************************************
*             Registers
***************************************/

#define Drive_DC_Motor_Quad_Dec_STATUS_REG                 (* (reg8 *) Drive_DC_Motor_Quad_Dec_bQuadDec_Stsreg__STATUS_REG)
#define Drive_DC_Motor_Quad_Dec_STATUS_PTR                 (  (reg8 *) Drive_DC_Motor_Quad_Dec_bQuadDec_Stsreg__STATUS_REG)
#define Drive_DC_Motor_Quad_Dec_STATUS_MASK                (* (reg8 *) Drive_DC_Motor_Quad_Dec_bQuadDec_Stsreg__MASK_REG)
#define Drive_DC_Motor_Quad_Dec_STATUS_MASK_PTR            (  (reg8 *) Drive_DC_Motor_Quad_Dec_bQuadDec_Stsreg__MASK_REG)
#define Drive_DC_Motor_Quad_Dec_SR_AUX_CONTROL             (* (reg8 *) Drive_DC_Motor_Quad_Dec_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)
#define Drive_DC_Motor_Quad_Dec_SR_AUX_CONTROL_PTR         (  (reg8 *) Drive_DC_Motor_Quad_Dec_bQuadDec_Stsreg__STATUS_AUX_CTL_REG)


/***************************************
*        Register Constants
***************************************/

#define Drive_DC_Motor_Quad_Dec_COUNTER_OVERFLOW_SHIFT     (0x00u)
#define Drive_DC_Motor_Quad_Dec_COUNTER_UNDERFLOW_SHIFT    (0x01u)
#define Drive_DC_Motor_Quad_Dec_COUNTER_RESET_SHIFT        (0x02u)
#define Drive_DC_Motor_Quad_Dec_INVALID_IN_SHIFT           (0x03u)
#define Drive_DC_Motor_Quad_Dec_COUNTER_OVERFLOW           ((uint8) (0x01u << Drive_DC_Motor_Quad_Dec_COUNTER_OVERFLOW_SHIFT))
#define Drive_DC_Motor_Quad_Dec_COUNTER_UNDERFLOW          ((uint8) (0x01u << Drive_DC_Motor_Quad_Dec_COUNTER_UNDERFLOW_SHIFT))
#define Drive_DC_Motor_Quad_Dec_COUNTER_RESET              ((uint8) (0x01u << Drive_DC_Motor_Quad_Dec_COUNTER_RESET_SHIFT))
#define Drive_DC_Motor_Quad_Dec_INVALID_IN                 ((uint8) (0x01u << Drive_DC_Motor_Quad_Dec_INVALID_IN_SHIFT))

#define Drive_DC_Motor_Quad_Dec_INTERRUPTS_ENABLE_SHIFT    (0x04u)
#define Drive_DC_Motor_Quad_Dec_INTERRUPTS_ENABLE          ((uint8)(0x01u << Drive_DC_Motor_Quad_Dec_INTERRUPTS_ENABLE_SHIFT))
#define Drive_DC_Motor_Quad_Dec_INIT_INT_MASK              (0x0Fu)


/******************************************************************************************
* Following code are OBSOLETE and must not be used starting from Quadrature Decoder 2.20
******************************************************************************************/
#define Drive_DC_Motor_Quad_Dec_DISABLE                    (0x00u)


#endif /* CY_QUADRATURE_DECODER_Drive_DC_Motor_Quad_Dec_H */


/* [] END OF FILE */
