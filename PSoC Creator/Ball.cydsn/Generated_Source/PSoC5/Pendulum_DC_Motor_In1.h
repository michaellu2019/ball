/*******************************************************************************
* File Name: Pendulum_DC_Motor_In1.h  
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

#if !defined(CY_PINS_Pendulum_DC_Motor_In1_H) /* Pins Pendulum_DC_Motor_In1_H */
#define CY_PINS_Pendulum_DC_Motor_In1_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pendulum_DC_Motor_In1_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pendulum_DC_Motor_In1__PORT == 15 && ((Pendulum_DC_Motor_In1__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Pendulum_DC_Motor_In1_Write(uint8 value);
void    Pendulum_DC_Motor_In1_SetDriveMode(uint8 mode);
uint8   Pendulum_DC_Motor_In1_ReadDataReg(void);
uint8   Pendulum_DC_Motor_In1_Read(void);
void    Pendulum_DC_Motor_In1_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pendulum_DC_Motor_In1_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pendulum_DC_Motor_In1_SetDriveMode() function.
     *  @{
     */
        #define Pendulum_DC_Motor_In1_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pendulum_DC_Motor_In1_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pendulum_DC_Motor_In1_DM_RES_UP          PIN_DM_RES_UP
        #define Pendulum_DC_Motor_In1_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pendulum_DC_Motor_In1_DM_OD_LO           PIN_DM_OD_LO
        #define Pendulum_DC_Motor_In1_DM_OD_HI           PIN_DM_OD_HI
        #define Pendulum_DC_Motor_In1_DM_STRONG          PIN_DM_STRONG
        #define Pendulum_DC_Motor_In1_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Pendulum_DC_Motor_In1_MASK               Pendulum_DC_Motor_In1__MASK
#define Pendulum_DC_Motor_In1_SHIFT              Pendulum_DC_Motor_In1__SHIFT
#define Pendulum_DC_Motor_In1_WIDTH              1u

/* Interrupt constants */
#if defined(Pendulum_DC_Motor_In1__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pendulum_DC_Motor_In1_SetInterruptMode() function.
     *  @{
     */
        #define Pendulum_DC_Motor_In1_INTR_NONE      (uint16)(0x0000u)
        #define Pendulum_DC_Motor_In1_INTR_RISING    (uint16)(0x0001u)
        #define Pendulum_DC_Motor_In1_INTR_FALLING   (uint16)(0x0002u)
        #define Pendulum_DC_Motor_In1_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Pendulum_DC_Motor_In1_INTR_MASK      (0x01u) 
#endif /* (Pendulum_DC_Motor_In1__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pendulum_DC_Motor_In1_PS                     (* (reg8 *) Pendulum_DC_Motor_In1__PS)
/* Data Register */
#define Pendulum_DC_Motor_In1_DR                     (* (reg8 *) Pendulum_DC_Motor_In1__DR)
/* Port Number */
#define Pendulum_DC_Motor_In1_PRT_NUM                (* (reg8 *) Pendulum_DC_Motor_In1__PRT) 
/* Connect to Analog Globals */                                                  
#define Pendulum_DC_Motor_In1_AG                     (* (reg8 *) Pendulum_DC_Motor_In1__AG)                       
/* Analog MUX bux enable */
#define Pendulum_DC_Motor_In1_AMUX                   (* (reg8 *) Pendulum_DC_Motor_In1__AMUX) 
/* Bidirectional Enable */                                                        
#define Pendulum_DC_Motor_In1_BIE                    (* (reg8 *) Pendulum_DC_Motor_In1__BIE)
/* Bit-mask for Aliased Register Access */
#define Pendulum_DC_Motor_In1_BIT_MASK               (* (reg8 *) Pendulum_DC_Motor_In1__BIT_MASK)
/* Bypass Enable */
#define Pendulum_DC_Motor_In1_BYP                    (* (reg8 *) Pendulum_DC_Motor_In1__BYP)
/* Port wide control signals */                                                   
#define Pendulum_DC_Motor_In1_CTL                    (* (reg8 *) Pendulum_DC_Motor_In1__CTL)
/* Drive Modes */
#define Pendulum_DC_Motor_In1_DM0                    (* (reg8 *) Pendulum_DC_Motor_In1__DM0) 
#define Pendulum_DC_Motor_In1_DM1                    (* (reg8 *) Pendulum_DC_Motor_In1__DM1)
#define Pendulum_DC_Motor_In1_DM2                    (* (reg8 *) Pendulum_DC_Motor_In1__DM2) 
/* Input Buffer Disable Override */
#define Pendulum_DC_Motor_In1_INP_DIS                (* (reg8 *) Pendulum_DC_Motor_In1__INP_DIS)
/* LCD Common or Segment Drive */
#define Pendulum_DC_Motor_In1_LCD_COM_SEG            (* (reg8 *) Pendulum_DC_Motor_In1__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pendulum_DC_Motor_In1_LCD_EN                 (* (reg8 *) Pendulum_DC_Motor_In1__LCD_EN)
/* Slew Rate Control */
#define Pendulum_DC_Motor_In1_SLW                    (* (reg8 *) Pendulum_DC_Motor_In1__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pendulum_DC_Motor_In1_PRTDSI__CAPS_SEL       (* (reg8 *) Pendulum_DC_Motor_In1__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pendulum_DC_Motor_In1_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pendulum_DC_Motor_In1__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pendulum_DC_Motor_In1_PRTDSI__OE_SEL0        (* (reg8 *) Pendulum_DC_Motor_In1__PRTDSI__OE_SEL0) 
#define Pendulum_DC_Motor_In1_PRTDSI__OE_SEL1        (* (reg8 *) Pendulum_DC_Motor_In1__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pendulum_DC_Motor_In1_PRTDSI__OUT_SEL0       (* (reg8 *) Pendulum_DC_Motor_In1__PRTDSI__OUT_SEL0) 
#define Pendulum_DC_Motor_In1_PRTDSI__OUT_SEL1       (* (reg8 *) Pendulum_DC_Motor_In1__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pendulum_DC_Motor_In1_PRTDSI__SYNC_OUT       (* (reg8 *) Pendulum_DC_Motor_In1__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Pendulum_DC_Motor_In1__SIO_CFG)
    #define Pendulum_DC_Motor_In1_SIO_HYST_EN        (* (reg8 *) Pendulum_DC_Motor_In1__SIO_HYST_EN)
    #define Pendulum_DC_Motor_In1_SIO_REG_HIFREQ     (* (reg8 *) Pendulum_DC_Motor_In1__SIO_REG_HIFREQ)
    #define Pendulum_DC_Motor_In1_SIO_CFG            (* (reg8 *) Pendulum_DC_Motor_In1__SIO_CFG)
    #define Pendulum_DC_Motor_In1_SIO_DIFF           (* (reg8 *) Pendulum_DC_Motor_In1__SIO_DIFF)
#endif /* (Pendulum_DC_Motor_In1__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pendulum_DC_Motor_In1__INTSTAT)
    #define Pendulum_DC_Motor_In1_INTSTAT            (* (reg8 *) Pendulum_DC_Motor_In1__INTSTAT)
    #define Pendulum_DC_Motor_In1_SNAP               (* (reg8 *) Pendulum_DC_Motor_In1__SNAP)
    
	#define Pendulum_DC_Motor_In1_0_INTTYPE_REG 		(* (reg8 *) Pendulum_DC_Motor_In1__0__INTTYPE)
#endif /* (Pendulum_DC_Motor_In1__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pendulum_DC_Motor_In1_H */


/* [] END OF FILE */
