/*******************************************************************************
* File Name: Pendulum_DC_Motor_Enc_B.h  
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

#if !defined(CY_PINS_Pendulum_DC_Motor_Enc_B_H) /* Pins Pendulum_DC_Motor_Enc_B_H */
#define CY_PINS_Pendulum_DC_Motor_Enc_B_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "Pendulum_DC_Motor_Enc_B_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 Pendulum_DC_Motor_Enc_B__PORT == 15 && ((Pendulum_DC_Motor_Enc_B__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    Pendulum_DC_Motor_Enc_B_Write(uint8 value);
void    Pendulum_DC_Motor_Enc_B_SetDriveMode(uint8 mode);
uint8   Pendulum_DC_Motor_Enc_B_ReadDataReg(void);
uint8   Pendulum_DC_Motor_Enc_B_Read(void);
void    Pendulum_DC_Motor_Enc_B_SetInterruptMode(uint16 position, uint16 mode);
uint8   Pendulum_DC_Motor_Enc_B_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the Pendulum_DC_Motor_Enc_B_SetDriveMode() function.
     *  @{
     */
        #define Pendulum_DC_Motor_Enc_B_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define Pendulum_DC_Motor_Enc_B_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define Pendulum_DC_Motor_Enc_B_DM_RES_UP          PIN_DM_RES_UP
        #define Pendulum_DC_Motor_Enc_B_DM_RES_DWN         PIN_DM_RES_DWN
        #define Pendulum_DC_Motor_Enc_B_DM_OD_LO           PIN_DM_OD_LO
        #define Pendulum_DC_Motor_Enc_B_DM_OD_HI           PIN_DM_OD_HI
        #define Pendulum_DC_Motor_Enc_B_DM_STRONG          PIN_DM_STRONG
        #define Pendulum_DC_Motor_Enc_B_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define Pendulum_DC_Motor_Enc_B_MASK               Pendulum_DC_Motor_Enc_B__MASK
#define Pendulum_DC_Motor_Enc_B_SHIFT              Pendulum_DC_Motor_Enc_B__SHIFT
#define Pendulum_DC_Motor_Enc_B_WIDTH              1u

/* Interrupt constants */
#if defined(Pendulum_DC_Motor_Enc_B__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in Pendulum_DC_Motor_Enc_B_SetInterruptMode() function.
     *  @{
     */
        #define Pendulum_DC_Motor_Enc_B_INTR_NONE      (uint16)(0x0000u)
        #define Pendulum_DC_Motor_Enc_B_INTR_RISING    (uint16)(0x0001u)
        #define Pendulum_DC_Motor_Enc_B_INTR_FALLING   (uint16)(0x0002u)
        #define Pendulum_DC_Motor_Enc_B_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define Pendulum_DC_Motor_Enc_B_INTR_MASK      (0x01u) 
#endif /* (Pendulum_DC_Motor_Enc_B__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define Pendulum_DC_Motor_Enc_B_PS                     (* (reg8 *) Pendulum_DC_Motor_Enc_B__PS)
/* Data Register */
#define Pendulum_DC_Motor_Enc_B_DR                     (* (reg8 *) Pendulum_DC_Motor_Enc_B__DR)
/* Port Number */
#define Pendulum_DC_Motor_Enc_B_PRT_NUM                (* (reg8 *) Pendulum_DC_Motor_Enc_B__PRT) 
/* Connect to Analog Globals */                                                  
#define Pendulum_DC_Motor_Enc_B_AG                     (* (reg8 *) Pendulum_DC_Motor_Enc_B__AG)                       
/* Analog MUX bux enable */
#define Pendulum_DC_Motor_Enc_B_AMUX                   (* (reg8 *) Pendulum_DC_Motor_Enc_B__AMUX) 
/* Bidirectional Enable */                                                        
#define Pendulum_DC_Motor_Enc_B_BIE                    (* (reg8 *) Pendulum_DC_Motor_Enc_B__BIE)
/* Bit-mask for Aliased Register Access */
#define Pendulum_DC_Motor_Enc_B_BIT_MASK               (* (reg8 *) Pendulum_DC_Motor_Enc_B__BIT_MASK)
/* Bypass Enable */
#define Pendulum_DC_Motor_Enc_B_BYP                    (* (reg8 *) Pendulum_DC_Motor_Enc_B__BYP)
/* Port wide control signals */                                                   
#define Pendulum_DC_Motor_Enc_B_CTL                    (* (reg8 *) Pendulum_DC_Motor_Enc_B__CTL)
/* Drive Modes */
#define Pendulum_DC_Motor_Enc_B_DM0                    (* (reg8 *) Pendulum_DC_Motor_Enc_B__DM0) 
#define Pendulum_DC_Motor_Enc_B_DM1                    (* (reg8 *) Pendulum_DC_Motor_Enc_B__DM1)
#define Pendulum_DC_Motor_Enc_B_DM2                    (* (reg8 *) Pendulum_DC_Motor_Enc_B__DM2) 
/* Input Buffer Disable Override */
#define Pendulum_DC_Motor_Enc_B_INP_DIS                (* (reg8 *) Pendulum_DC_Motor_Enc_B__INP_DIS)
/* LCD Common or Segment Drive */
#define Pendulum_DC_Motor_Enc_B_LCD_COM_SEG            (* (reg8 *) Pendulum_DC_Motor_Enc_B__LCD_COM_SEG)
/* Enable Segment LCD */
#define Pendulum_DC_Motor_Enc_B_LCD_EN                 (* (reg8 *) Pendulum_DC_Motor_Enc_B__LCD_EN)
/* Slew Rate Control */
#define Pendulum_DC_Motor_Enc_B_SLW                    (* (reg8 *) Pendulum_DC_Motor_Enc_B__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define Pendulum_DC_Motor_Enc_B_PRTDSI__CAPS_SEL       (* (reg8 *) Pendulum_DC_Motor_Enc_B__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define Pendulum_DC_Motor_Enc_B_PRTDSI__DBL_SYNC_IN    (* (reg8 *) Pendulum_DC_Motor_Enc_B__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define Pendulum_DC_Motor_Enc_B_PRTDSI__OE_SEL0        (* (reg8 *) Pendulum_DC_Motor_Enc_B__PRTDSI__OE_SEL0) 
#define Pendulum_DC_Motor_Enc_B_PRTDSI__OE_SEL1        (* (reg8 *) Pendulum_DC_Motor_Enc_B__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define Pendulum_DC_Motor_Enc_B_PRTDSI__OUT_SEL0       (* (reg8 *) Pendulum_DC_Motor_Enc_B__PRTDSI__OUT_SEL0) 
#define Pendulum_DC_Motor_Enc_B_PRTDSI__OUT_SEL1       (* (reg8 *) Pendulum_DC_Motor_Enc_B__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define Pendulum_DC_Motor_Enc_B_PRTDSI__SYNC_OUT       (* (reg8 *) Pendulum_DC_Motor_Enc_B__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(Pendulum_DC_Motor_Enc_B__SIO_CFG)
    #define Pendulum_DC_Motor_Enc_B_SIO_HYST_EN        (* (reg8 *) Pendulum_DC_Motor_Enc_B__SIO_HYST_EN)
    #define Pendulum_DC_Motor_Enc_B_SIO_REG_HIFREQ     (* (reg8 *) Pendulum_DC_Motor_Enc_B__SIO_REG_HIFREQ)
    #define Pendulum_DC_Motor_Enc_B_SIO_CFG            (* (reg8 *) Pendulum_DC_Motor_Enc_B__SIO_CFG)
    #define Pendulum_DC_Motor_Enc_B_SIO_DIFF           (* (reg8 *) Pendulum_DC_Motor_Enc_B__SIO_DIFF)
#endif /* (Pendulum_DC_Motor_Enc_B__SIO_CFG) */

/* Interrupt Registers */
#if defined(Pendulum_DC_Motor_Enc_B__INTSTAT)
    #define Pendulum_DC_Motor_Enc_B_INTSTAT            (* (reg8 *) Pendulum_DC_Motor_Enc_B__INTSTAT)
    #define Pendulum_DC_Motor_Enc_B_SNAP               (* (reg8 *) Pendulum_DC_Motor_Enc_B__SNAP)
    
	#define Pendulum_DC_Motor_Enc_B_0_INTTYPE_REG 		(* (reg8 *) Pendulum_DC_Motor_Enc_B__0__INTTYPE)
#endif /* (Pendulum_DC_Motor_Enc_B__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_Pendulum_DC_Motor_Enc_B_H */


/* [] END OF FILE */
