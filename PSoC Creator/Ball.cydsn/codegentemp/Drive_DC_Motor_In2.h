/*******************************************************************************
* File Name: Drive_DC_Motor_In2.h  
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

#if !defined(CY_PINS_Drive_DC_Motor_In2_H) /* Pins Drive_DC_Motor_In2_H */
#define CY_PINS_Drive_DC_Motor_In2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Drive_DC_Motor_In2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Drive_DC_Motor_In2__PORT == 15 && ((Drive_DC_Motor_In2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Drive_DC_Motor_In2_Write(uint8 value);
void    Drive_DC_Motor_In2_SetDriveMode(uint8 mode);
uint8   Drive_DC_Motor_In2_ReadDataReg(void);
uint8   Drive_DC_Motor_In2_Read(void);
void    Drive_DC_Motor_In2_SetInterruptMode(uint16 position, uint16 mode);
uint8   Drive_DC_Motor_In2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Drive_DC_Motor_In2_SetDriveMode() function.
     *  @{
     */
        #define Drive_DC_Motor_In2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Drive_DC_Motor_In2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Drive_DC_Motor_In2_DM_RES_UP          PIN_DM_RES_UP
        #define Drive_DC_Motor_In2_DM_RES_DWN         PIN_DM_RES_DWN
        #define Drive_DC_Motor_In2_DM_OD_LO           PIN_DM_OD_LO
        #define Drive_DC_Motor_In2_DM_OD_HI           PIN_DM_OD_HI
        #define Drive_DC_Motor_In2_DM_STRONG          PIN_DM_STRONG
        #define Drive_DC_Motor_In2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Drive_DC_Motor_In2_MASK               Drive_DC_Motor_In2__MASK
#define Drive_DC_Motor_In2_SHIFT              Drive_DC_Motor_In2__SHIFT
#define Drive_DC_Motor_In2_WIDTH              1u

/* Interrupt constants */
#if defined(Drive_DC_Motor_In2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Drive_DC_Motor_In2_SetInterruptMode() function.
     *  @{
     */
        #define Drive_DC_Motor_In2_INTR_NONE      (uint16)(0x0000u)
        #define Drive_DC_Motor_In2_INTR_RISING    (uint16)(0x0001u)
        #define Drive_DC_Motor_In2_INTR_FALLING   (uint16)(0x0002u)
        #define Drive_DC_Motor_In2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Drive_DC_Motor_In2_INTR_MASK      (0x01u) 
#endif /* (Drive_DC_Motor_In2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Drive_DC_Motor_In2_PS                     (* (reg8 *) Drive_DC_Motor_In2__PS)
/* Data Register */
#define Drive_DC_Motor_In2_DR                     (* (reg8 *) Drive_DC_Motor_In2__DR)
/* Port Number */
#define Drive_DC_Motor_In2_PRT_NUM                (* (reg8 *) Drive_DC_Motor_In2__PRT) 
/* Connect to Analog Globals */                                                  
#define Drive_DC_Motor_In2_AG                     (* (reg8 *) Drive_DC_Motor_In2__AG)                       
/* Analog MUX bux enable */
#define Drive_DC_Motor_In2_AMUX                   (* (reg8 *) Drive_DC_Motor_In2__AMUX) 
/* Bidirectional Enable */                                                        
#define Drive_DC_Motor_In2_BIE                    (* (reg8 *) Drive_DC_Motor_In2__BIE)
/* Bit-mask for Aliased Register Access */
#define Drive_DC_Motor_In2_BIT_MASK               (* (reg8 *) Drive_DC_Motor_In2__BIT_MASK)
/* Bypass Enable */
#define Drive_DC_Motor_In2_BYP                    (* (reg8 *) Drive_DC_Motor_In2__BYP)
/* Port wide control signals */                                                   
#define Drive_DC_Motor_In2_CTL                    (* (reg8 *) Drive_DC_Motor_In2__CTL)
/* Drive Modes */
#define Drive_DC_Motor_In2_DM0                    (* (reg8 *) Drive_DC_Motor_In2__DM0) 
#define Drive_DC_Motor_In2_DM1                    (* (reg8 *) Drive_DC_Motor_In2__DM1)
#define Drive_DC_Motor_In2_DM2                    (* (reg8 *) Drive_DC_Motor_In2__DM2) 
/* Input Buffer Disable Override */
#define Drive_DC_Motor_In2_INP_DIS                (* (reg8 *) Drive_DC_Motor_In2__INP_DIS)
/* LCD Common or Segment Drive */
#define Drive_DC_Motor_In2_LCD_COM_SEG            (* (reg8 *) Drive_DC_Motor_In2__LCD_COM_SEG)
/* Enable Segment LCD */
#define Drive_DC_Motor_In2_LCD_EN                 (* (reg8 *) Drive_DC_Motor_In2__LCD_EN)
/* Slew Rate Control */
#define Drive_DC_Motor_In2_SLW                    (* (reg8 *) Drive_DC_Motor_In2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Drive_DC_Motor_In2_PRTDSI__CAPS_SEL       (* (reg8 *) Drive_DC_Motor_In2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Drive_DC_Motor_In2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Drive_DC_Motor_In2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Drive_DC_Motor_In2_PRTDSI__OE_SEL0        (* (reg8 *) Drive_DC_Motor_In2__PRTDSI__OE_SEL0) 
#define Drive_DC_Motor_In2_PRTDSI__OE_SEL1        (* (reg8 *) Drive_DC_Motor_In2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Drive_DC_Motor_In2_PRTDSI__OUT_SEL0       (* (reg8 *) Drive_DC_Motor_In2__PRTDSI__OUT_SEL0) 
#define Drive_DC_Motor_In2_PRTDSI__OUT_SEL1       (* (reg8 *) Drive_DC_Motor_In2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Drive_DC_Motor_In2_PRTDSI__SYNC_OUT       (* (reg8 *) Drive_DC_Motor_In2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Drive_DC_Motor_In2__SIO_CFG)
    #define Drive_DC_Motor_In2_SIO_HYST_EN        (* (reg8 *) Drive_DC_Motor_In2__SIO_HYST_EN)
    #define Drive_DC_Motor_In2_SIO_REG_HIFREQ     (* (reg8 *) Drive_DC_Motor_In2__SIO_REG_HIFREQ)
    #define Drive_DC_Motor_In2_SIO_CFG            (* (reg8 *) Drive_DC_Motor_In2__SIO_CFG)
    #define Drive_DC_Motor_In2_SIO_DIFF           (* (reg8 *) Drive_DC_Motor_In2__SIO_DIFF)
#endif /* (Drive_DC_Motor_In2__SIO_CFG) */

/* Interrupt Registers */
#if defined(Drive_DC_Motor_In2__INTSTAT)
    #define Drive_DC_Motor_In2_INTSTAT            (* (reg8 *) Drive_DC_Motor_In2__INTSTAT)
    #define Drive_DC_Motor_In2_SNAP               (* (reg8 *) Drive_DC_Motor_In2__SNAP)
    
	#define Drive_DC_Motor_In2_0_INTTYPE_REG 		(* (reg8 *) Drive_DC_Motor_In2__0__INTTYPE)
#endif /* (Drive_DC_Motor_In2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Drive_DC_Motor_In2_H */


/* [] END OF FILE */
