/*******************************************************************************
* File Name: Head_Servo_Motor.h  
* Version 2.20
*
* Description:
*  This file contains Pin function prototypes and register defines
*
* Note:
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation.  All rights reserved.
* You may use this file only in accordance with the license, terms, conditions, 
* disclaimers, and limitations in the end user license agreement accompanying 
* the software package with which this file was provided.
*******************************************************************************/

#if !defined(CY_PINS_Head_Servo_Motor_H) /* Pins Head_Servo_Motor_H */
#define CY_PINS_Head_Servo_Motor_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Head_Servo_Motor_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Head_Servo_Motor__PORT == 15 && ((Head_Servo_Motor__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Head_Servo_Motor_Write(uint8 value);
void    Head_Servo_Motor_SetDriveMode(uint8 mode);
uint8   Head_Servo_Motor_ReadDataReg(void);
uint8   Head_Servo_Motor_Read(void);
void    Head_Servo_Motor_SetInterruptMode(uint16 position, uint16 mode);
uint8   Head_Servo_Motor_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Head_Servo_Motor_SetDriveMode() function.
     *  @{
     */
        #define Head_Servo_Motor_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Head_Servo_Motor_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Head_Servo_Motor_DM_RES_UP          PIN_DM_RES_UP
        #define Head_Servo_Motor_DM_RES_DWN         PIN_DM_RES_DWN
        #define Head_Servo_Motor_DM_OD_LO           PIN_DM_OD_LO
        #define Head_Servo_Motor_DM_OD_HI           PIN_DM_OD_HI
        #define Head_Servo_Motor_DM_STRONG          PIN_DM_STRONG
        #define Head_Servo_Motor_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Head_Servo_Motor_MASK               Head_Servo_Motor__MASK
#define Head_Servo_Motor_SHIFT              Head_Servo_Motor__SHIFT
#define Head_Servo_Motor_WIDTH              1u

/* Interrupt constants */
#if defined(Head_Servo_Motor__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Head_Servo_Motor_SetInterruptMode() function.
     *  @{
     */
        #define Head_Servo_Motor_INTR_NONE      (uint16)(0x0000u)
        #define Head_Servo_Motor_INTR_RISING    (uint16)(0x0001u)
        #define Head_Servo_Motor_INTR_FALLING   (uint16)(0x0002u)
        #define Head_Servo_Motor_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Head_Servo_Motor_INTR_MASK      (0x01u) 
#endif /* (Head_Servo_Motor__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Head_Servo_Motor_PS                     (* (reg8 *) Head_Servo_Motor__PS)
/* Data Register */
#define Head_Servo_Motor_DR                     (* (reg8 *) Head_Servo_Motor__DR)
/* Port Number */
#define Head_Servo_Motor_PRT_NUM                (* (reg8 *) Head_Servo_Motor__PRT) 
/* Connect to Analog Globals */                                                  
#define Head_Servo_Motor_AG                     (* (reg8 *) Head_Servo_Motor__AG)                       
/* Analog MUX bux enable */
#define Head_Servo_Motor_AMUX                   (* (reg8 *) Head_Servo_Motor__AMUX) 
/* Bidirectional Enable */                                                        
#define Head_Servo_Motor_BIE                    (* (reg8 *) Head_Servo_Motor__BIE)
/* Bit-mask for Aliased Register Access */
#define Head_Servo_Motor_BIT_MASK               (* (reg8 *) Head_Servo_Motor__BIT_MASK)
/* Bypass Enable */
#define Head_Servo_Motor_BYP                    (* (reg8 *) Head_Servo_Motor__BYP)
/* Port wide control signals */                                                   
#define Head_Servo_Motor_CTL                    (* (reg8 *) Head_Servo_Motor__CTL)
/* Drive Modes */
#define Head_Servo_Motor_DM0                    (* (reg8 *) Head_Servo_Motor__DM0) 
#define Head_Servo_Motor_DM1                    (* (reg8 *) Head_Servo_Motor__DM1)
#define Head_Servo_Motor_DM2                    (* (reg8 *) Head_Servo_Motor__DM2) 
/* Input Buffer Disable Override */
#define Head_Servo_Motor_INP_DIS                (* (reg8 *) Head_Servo_Motor__INP_DIS)
/* LCD Common or Segment Drive */
#define Head_Servo_Motor_LCD_COM_SEG            (* (reg8 *) Head_Servo_Motor__LCD_COM_SEG)
/* Enable Segment LCD */
#define Head_Servo_Motor_LCD_EN                 (* (reg8 *) Head_Servo_Motor__LCD_EN)
/* Slew Rate Control */
#define Head_Servo_Motor_SLW                    (* (reg8 *) Head_Servo_Motor__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Head_Servo_Motor_PRTDSI__CAPS_SEL       (* (reg8 *) Head_Servo_Motor__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Head_Servo_Motor_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Head_Servo_Motor__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Head_Servo_Motor_PRTDSI__OE_SEL0        (* (reg8 *) Head_Servo_Motor__PRTDSI__OE_SEL0) 
#define Head_Servo_Motor_PRTDSI__OE_SEL1        (* (reg8 *) Head_Servo_Motor__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Head_Servo_Motor_PRTDSI__OUT_SEL0       (* (reg8 *) Head_Servo_Motor__PRTDSI__OUT_SEL0) 
#define Head_Servo_Motor_PRTDSI__OUT_SEL1       (* (reg8 *) Head_Servo_Motor__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Head_Servo_Motor_PRTDSI__SYNC_OUT       (* (reg8 *) Head_Servo_Motor__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Head_Servo_Motor__SIO_CFG)
    #define Head_Servo_Motor_SIO_HYST_EN        (* (reg8 *) Head_Servo_Motor__SIO_HYST_EN)
    #define Head_Servo_Motor_SIO_REG_HIFREQ     (* (reg8 *) Head_Servo_Motor__SIO_REG_HIFREQ)
    #define Head_Servo_Motor_SIO_CFG            (* (reg8 *) Head_Servo_Motor__SIO_CFG)
    #define Head_Servo_Motor_SIO_DIFF           (* (reg8 *) Head_Servo_Motor__SIO_DIFF)
#endif /* (Head_Servo_Motor__SIO_CFG) */

/* Interrupt Registers */
#if defined(Head_Servo_Motor__INTSTAT)
    #define Head_Servo_Motor_INTSTAT            (* (reg8 *) Head_Servo_Motor__INTSTAT)
    #define Head_Servo_Motor_SNAP               (* (reg8 *) Head_Servo_Motor__SNAP)
    
	#define Head_Servo_Motor_0_INTTYPE_REG 		(* (reg8 *) Head_Servo_Motor__0__INTTYPE)
#endif /* (Head_Servo_Motor__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Head_Servo_Motor_H */


/* [] END OF FILE */
