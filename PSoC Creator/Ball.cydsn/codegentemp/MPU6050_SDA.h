/*******************************************************************************
* File Name: MPU6050_SDA.h  
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

#if !defined(CY_PINS_MPU6050_SDA_H) /* Pins MPU6050_SDA_H */
#define CY_PINS_MPU6050_SDA_H

#include "cytypes.h"
#include "cyfitter.h"
#include "cypins.h"
#include "MPU6050_SDA_aliases.h"

/* APIs are not generated for P15[7:6] */
#if !(CY_PSOC5A &&\
	 MPU6050_SDA__PORT == 15 && ((MPU6050_SDA__MASK & 0xC0) != 0))


/***************************************
*        Function Prototypes             
***************************************/    

/**
* \addtogroup group_general
* @{
*/
void    MPU6050_SDA_Write(uint8 value);
void    MPU6050_SDA_SetDriveMode(uint8 mode);
uint8   MPU6050_SDA_ReadDataReg(void);
uint8   MPU6050_SDA_Read(void);
void    MPU6050_SDA_SetInterruptMode(uint16 position, uint16 mode);
uint8   MPU6050_SDA_ClearInterrupt(void);
/** @} general */

/***************************************
*           API Constants        
***************************************/
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup driveMode Drive mode constants
     * \brief Constants to be passed as "mode" parameter in the MPU6050_SDA_SetDriveMode() function.
     *  @{
     */
        #define MPU6050_SDA_DM_ALG_HIZ         PIN_DM_ALG_HIZ
        #define MPU6050_SDA_DM_DIG_HIZ         PIN_DM_DIG_HIZ
        #define MPU6050_SDA_DM_RES_UP          PIN_DM_RES_UP
        #define MPU6050_SDA_DM_RES_DWN         PIN_DM_RES_DWN
        #define MPU6050_SDA_DM_OD_LO           PIN_DM_OD_LO
        #define MPU6050_SDA_DM_OD_HI           PIN_DM_OD_HI
        #define MPU6050_SDA_DM_STRONG          PIN_DM_STRONG
        #define MPU6050_SDA_DM_RES_UPDWN       PIN_DM_RES_UPDWN
    /** @} driveMode */
/** @} group_constants */
    
/* Digital Port Constants */
#define MPU6050_SDA_MASK               MPU6050_SDA__MASK
#define MPU6050_SDA_SHIFT              MPU6050_SDA__SHIFT
#define MPU6050_SDA_WIDTH              1u

/* Interrupt constants */
#if defined(MPU6050_SDA__INTSTAT)
/**
* \addtogroup group_constants
* @{
*/
    /** \addtogroup intrMode Interrupt constants
     * \brief Constants to be passed as "mode" parameter in MPU6050_SDA_SetInterruptMode() function.
     *  @{
     */
        #define MPU6050_SDA_INTR_NONE      (uint16)(0x0000u)
        #define MPU6050_SDA_INTR_RISING    (uint16)(0x0001u)
        #define MPU6050_SDA_INTR_FALLING   (uint16)(0x0002u)
        #define MPU6050_SDA_INTR_BOTH      (uint16)(0x0003u) 
    /** @} intrMode */
/** @} group_constants */

    #define MPU6050_SDA_INTR_MASK      (0x01u) 
#endif /* (MPU6050_SDA__INTSTAT) */


/***************************************
*             Registers        
***************************************/

/* Main Port Registers */
/* Pin State */
#define MPU6050_SDA_PS                     (* (reg8 *) MPU6050_SDA__PS)
/* Data Register */
#define MPU6050_SDA_DR                     (* (reg8 *) MPU6050_SDA__DR)
/* Port Number */
#define MPU6050_SDA_PRT_NUM                (* (reg8 *) MPU6050_SDA__PRT) 
/* Connect to Analog Globals */                                                  
#define MPU6050_SDA_AG                     (* (reg8 *) MPU6050_SDA__AG)                       
/* Analog MUX bux enable */
#define MPU6050_SDA_AMUX                   (* (reg8 *) MPU6050_SDA__AMUX) 
/* Bidirectional Enable */                                                        
#define MPU6050_SDA_BIE                    (* (reg8 *) MPU6050_SDA__BIE)
/* Bit-mask for Aliased Register Access */
#define MPU6050_SDA_BIT_MASK               (* (reg8 *) MPU6050_SDA__BIT_MASK)
/* Bypass Enable */
#define MPU6050_SDA_BYP                    (* (reg8 *) MPU6050_SDA__BYP)
/* Port wide control signals */                                                   
#define MPU6050_SDA_CTL                    (* (reg8 *) MPU6050_SDA__CTL)
/* Drive Modes */
#define MPU6050_SDA_DM0                    (* (reg8 *) MPU6050_SDA__DM0) 
#define MPU6050_SDA_DM1                    (* (reg8 *) MPU6050_SDA__DM1)
#define MPU6050_SDA_DM2                    (* (reg8 *) MPU6050_SDA__DM2) 
/* Input Buffer Disable Override */
#define MPU6050_SDA_INP_DIS                (* (reg8 *) MPU6050_SDA__INP_DIS)
/* LCD Common or Segment Drive */
#define MPU6050_SDA_LCD_COM_SEG            (* (reg8 *) MPU6050_SDA__LCD_COM_SEG)
/* Enable Segment LCD */
#define MPU6050_SDA_LCD_EN                 (* (reg8 *) MPU6050_SDA__LCD_EN)
/* Slew Rate Control */
#define MPU6050_SDA_SLW                    (* (reg8 *) MPU6050_SDA__SLW)

/* DSI Port Registers */
/* Global DSI Select Register */
#define MPU6050_SDA_PRTDSI__CAPS_SEL       (* (reg8 *) MPU6050_SDA__PRTDSI__CAPS_SEL) 
/* Double Sync Enable */
#define MPU6050_SDA_PRTDSI__DBL_SYNC_IN    (* (reg8 *) MPU6050_SDA__PRTDSI__DBL_SYNC_IN) 
/* Output Enable Select Drive Strength */
#define MPU6050_SDA_PRTDSI__OE_SEL0        (* (reg8 *) MPU6050_SDA__PRTDSI__OE_SEL0) 
#define MPU6050_SDA_PRTDSI__OE_SEL1        (* (reg8 *) MPU6050_SDA__PRTDSI__OE_SEL1) 
/* Port Pin Output Select Registers */
#define MPU6050_SDA_PRTDSI__OUT_SEL0       (* (reg8 *) MPU6050_SDA__PRTDSI__OUT_SEL0) 
#define MPU6050_SDA_PRTDSI__OUT_SEL1       (* (reg8 *) MPU6050_SDA__PRTDSI__OUT_SEL1) 
/* Sync Output Enable Registers */
#define MPU6050_SDA_PRTDSI__SYNC_OUT       (* (reg8 *) MPU6050_SDA__PRTDSI__SYNC_OUT) 

/* SIO registers */
#if defined(MPU6050_SDA__SIO_CFG)
    #define MPU6050_SDA_SIO_HYST_EN        (* (reg8 *) MPU6050_SDA__SIO_HYST_EN)
    #define MPU6050_SDA_SIO_REG_HIFREQ     (* (reg8 *) MPU6050_SDA__SIO_REG_HIFREQ)
    #define MPU6050_SDA_SIO_CFG            (* (reg8 *) MPU6050_SDA__SIO_CFG)
    #define MPU6050_SDA_SIO_DIFF           (* (reg8 *) MPU6050_SDA__SIO_DIFF)
#endif /* (MPU6050_SDA__SIO_CFG) */

/* Interrupt Registers */
#if defined(MPU6050_SDA__INTSTAT)
    #define MPU6050_SDA_INTSTAT            (* (reg8 *) MPU6050_SDA__INTSTAT)
    #define MPU6050_SDA_SNAP               (* (reg8 *) MPU6050_SDA__SNAP)
    
	#define MPU6050_SDA_0_INTTYPE_REG 		(* (reg8 *) MPU6050_SDA__0__INTTYPE)
#endif /* (MPU6050_SDA__INTSTAT) */

#endif /* CY_PSOC5A... */

#endif /*  CY_PINS_MPU6050_SDA_H */


/* [] END OF FILE */
