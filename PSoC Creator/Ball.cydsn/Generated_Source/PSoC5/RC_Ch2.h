/*******************************************************************************
* File Name: RC_Ch2.h  
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

#if !defined(CY_PINS_RC_Ch2_H) /* Pins RC_Ch2_H */
#define CY_PINS_RC_Ch2_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "RC_Ch2_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 RC_Ch2__PORT == 15 && ((RC_Ch2__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    RC_Ch2_Write(uint8 value);
void    RC_Ch2_SetDriveMode(uint8 mode);
uint8   RC_Ch2_ReadDataReg(void);
uint8   RC_Ch2_Read(void);
void    RC_Ch2_SetInterruptMode(uint16 position, uint16 mode);
uint8   RC_Ch2_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the RC_Ch2_SetDriveMode() function.
     *  @{
     */
        #define RC_Ch2_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define RC_Ch2_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define RC_Ch2_DM_RES_UP          PIN_DM_RES_UP
        #define RC_Ch2_DM_RES_DWN         PIN_DM_RES_DWN
        #define RC_Ch2_DM_OD_LO           PIN_DM_OD_LO
        #define RC_Ch2_DM_OD_HI           PIN_DM_OD_HI
        #define RC_Ch2_DM_STRONG          PIN_DM_STRONG
        #define RC_Ch2_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define RC_Ch2_MASK               RC_Ch2__MASK
#define RC_Ch2_SHIFT              RC_Ch2__SHIFT
#define RC_Ch2_WIDTH              1u

/* Interrupt constants */
#if defined(RC_Ch2__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in RC_Ch2_SetInterruptMode() function.
     *  @{
     */
        #define RC_Ch2_INTR_NONE      (uint16)(0x0000u)
        #define RC_Ch2_INTR_RISING    (uint16)(0x0001u)
        #define RC_Ch2_INTR_FALLING   (uint16)(0x0002u)
        #define RC_Ch2_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define RC_Ch2_INTR_MASK      (0x01u) 
#endif /* (RC_Ch2__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define RC_Ch2_PS                     (* (reg8 *) RC_Ch2__PS)
/* Data Register */
#define RC_Ch2_DR                     (* (reg8 *) RC_Ch2__DR)
/* Port Number */
#define RC_Ch2_PRT_NUM                (* (reg8 *) RC_Ch2__PRT) 
/* Connect to Analog Globals */                                                  
#define RC_Ch2_AG                     (* (reg8 *) RC_Ch2__AG)                       
/* Analog MUX bux enable */
#define RC_Ch2_AMUX                   (* (reg8 *) RC_Ch2__AMUX) 
/* Bidirectional Enable */                                                        
#define RC_Ch2_BIE                    (* (reg8 *) RC_Ch2__BIE)
/* Bit-mask for Aliased Register Access */
#define RC_Ch2_BIT_MASK               (* (reg8 *) RC_Ch2__BIT_MASK)
/* Bypass Enable */
#define RC_Ch2_BYP                    (* (reg8 *) RC_Ch2__BYP)
/* Port wide control signals */                                                   
#define RC_Ch2_CTL                    (* (reg8 *) RC_Ch2__CTL)
/* Drive Modes */
#define RC_Ch2_DM0                    (* (reg8 *) RC_Ch2__DM0) 
#define RC_Ch2_DM1                    (* (reg8 *) RC_Ch2__DM1)
#define RC_Ch2_DM2                    (* (reg8 *) RC_Ch2__DM2) 
/* Input Buffer Disable Override */
#define RC_Ch2_INP_DIS                (* (reg8 *) RC_Ch2__INP_DIS)
/* LCD Common or Segment Drive */
#define RC_Ch2_LCD_COM_SEG            (* (reg8 *) RC_Ch2__LCD_COM_SEG)
/* Enable Segment LCD */
#define RC_Ch2_LCD_EN                 (* (reg8 *) RC_Ch2__LCD_EN)
/* Slew Rate Control */
#define RC_Ch2_SLW                    (* (reg8 *) RC_Ch2__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define RC_Ch2_PRTDSI__CAPS_SEL       (* (reg8 *) RC_Ch2__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define RC_Ch2_PRTDSI__DBL_SYNC_IN    (* (reg8 *) RC_Ch2__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define RC_Ch2_PRTDSI__OE_SEL0        (* (reg8 *) RC_Ch2__PRTDSI__OE_SEL0) 
#define RC_Ch2_PRTDSI__OE_SEL1        (* (reg8 *) RC_Ch2__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define RC_Ch2_PRTDSI__OUT_SEL0       (* (reg8 *) RC_Ch2__PRTDSI__OUT_SEL0) 
#define RC_Ch2_PRTDSI__OUT_SEL1       (* (reg8 *) RC_Ch2__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define RC_Ch2_PRTDSI__SYNC_OUT       (* (reg8 *) RC_Ch2__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(RC_Ch2__SIO_CFG)
    #define RC_Ch2_SIO_HYST_EN        (* (reg8 *) RC_Ch2__SIO_HYST_EN)
    #define RC_Ch2_SIO_REG_HIFREQ     (* (reg8 *) RC_Ch2__SIO_REG_HIFREQ)
    #define RC_Ch2_SIO_CFG            (* (reg8 *) RC_Ch2__SIO_CFG)
    #define RC_Ch2_SIO_DIFF           (* (reg8 *) RC_Ch2__SIO_DIFF)
#endif /* (RC_Ch2__SIO_CFG) */

/* Interrupt Registers */
#if defined(RC_Ch2__INTSTAT)
    #define RC_Ch2_INTSTAT            (* (reg8 *) RC_Ch2__INTSTAT)
    #define RC_Ch2_SNAP               (* (reg8 *) RC_Ch2__SNAP)
    
	#define RC_Ch2_0_INTTYPE_REG 		(* (reg8 *) RC_Ch2__0__INTTYPE)
#endif /* (RC_Ch2__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_RC_Ch2_H */


/* [] END OF FILE */
