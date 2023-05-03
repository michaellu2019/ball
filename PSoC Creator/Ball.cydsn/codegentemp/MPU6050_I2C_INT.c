/*******************************************************************************
* File Name: MPU6050_I2C_INT.c
* Version 3.50
*
* Description:
*  This file provides the source code of Interrupt Service Routine (ISR)
*  for the I2C component.
*
********************************************************************************
* Copyright 2008-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "MPU6050_I2C_PVT.h"
#include "cyapicallbacks.h"


/*******************************************************************************
*  Place your includes, defines and code here.
********************************************************************************/
/* `#START MPU6050_I2C_ISR_intc` */

/* `#END` */


/*******************************************************************************
* Function Name: MPU6050_I2C_ISR
********************************************************************************
*
* Summary:
*  The handler for the I2C interrupt. The slave and master operations are
*  handled here.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Reentrant:
*  No.
*
*******************************************************************************/
CY_ISR(MPU6050_I2C_ISR)
{
#if (MPU6050_I2C_MODE_SLAVE_ENABLED)
   uint8  tmp8;
#endif  /* (MPU6050_I2C_MODE_SLAVE_ENABLED) */

    uint8  tmpCsr;
    
#ifdef MPU6050_I2C_ISR_ENTRY_CALLBACK
    MPU6050_I2C_ISR_EntryCallback();
#endif /* MPU6050_I2C_ISR_ENTRY_CALLBACK */
    

#if(MPU6050_I2C_TIMEOUT_FF_ENABLED)
    if(0u != MPU6050_I2C_TimeoutGetStatus())
    {
        MPU6050_I2C_TimeoutReset();
        MPU6050_I2C_state = MPU6050_I2C_SM_EXIT_IDLE;
        /* MPU6050_I2C_CSR_REG should be cleared after reset */
    }
#endif /* (MPU6050_I2C_TIMEOUT_FF_ENABLED) */


    tmpCsr = MPU6050_I2C_CSR_REG;      /* Make copy as interrupts clear */

#if(MPU6050_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED)
    if(MPU6050_I2C_CHECK_START_GEN(MPU6050_I2C_MCSR_REG))
    {
        MPU6050_I2C_CLEAR_START_GEN;

        /* Set transfer complete and error flags */
        MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_ERR_XFER |
                                        MPU6050_I2C_GET_MSTAT_CMPLT);

        /* Slave was addressed */
        MPU6050_I2C_state = MPU6050_I2C_SM_SLAVE;
    }
#endif /* (MPU6050_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED) */


#if(MPU6050_I2C_MODE_MULTI_MASTER_ENABLED)
    if(MPU6050_I2C_CHECK_LOST_ARB(tmpCsr))
    {
        /* Set errors */
        MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_ERR_XFER     |
                                        MPU6050_I2C_MSTAT_ERR_ARB_LOST |
                                        MPU6050_I2C_GET_MSTAT_CMPLT);

        MPU6050_I2C_DISABLE_INT_ON_STOP; /* Interrupt on Stop is enabled by write */

        #if(MPU6050_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED)
            if(MPU6050_I2C_CHECK_ADDRESS_STS(tmpCsr))
            {
                /* Slave was addressed */
                MPU6050_I2C_state = MPU6050_I2C_SM_SLAVE;
            }
            else
            {
                MPU6050_I2C_BUS_RELEASE;

                MPU6050_I2C_state = MPU6050_I2C_SM_EXIT_IDLE;
            }
        #else
            MPU6050_I2C_BUS_RELEASE;

            MPU6050_I2C_state = MPU6050_I2C_SM_EXIT_IDLE;

        #endif /* (MPU6050_I2C_MODE_MULTI_MASTER_SLAVE_ENABLED) */
    }
#endif /* (MPU6050_I2C_MODE_MULTI_MASTER_ENABLED) */

    /* Check for master operation mode */
    if(MPU6050_I2C_CHECK_SM_MASTER)
    {
    #if(MPU6050_I2C_MODE_MASTER_ENABLED)
        if(MPU6050_I2C_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            switch (MPU6050_I2C_state)
            {
            case MPU6050_I2C_SM_MSTR_WR_ADDR:  /* After address is sent, write data */
            case MPU6050_I2C_SM_MSTR_RD_ADDR:  /* After address is sent, read data */

                tmpCsr &= ((uint8) ~MPU6050_I2C_CSR_STOP_STATUS); /* Clear Stop bit history on address phase */

                if(MPU6050_I2C_CHECK_ADDR_ACK(tmpCsr))
                {
                    /* Setup for transmit or receive of data */
                    if(MPU6050_I2C_state == MPU6050_I2C_SM_MSTR_WR_ADDR)   /* TRANSMIT data */
                    {
                        /* Check if at least one byte to transfer */
                        if(MPU6050_I2C_mstrWrBufSize > 0u)
                        {
                            /* Load the 1st data byte */
                            MPU6050_I2C_DATA_REG = MPU6050_I2C_mstrWrBufPtr[0u];
                            MPU6050_I2C_TRANSMIT_DATA;
                            MPU6050_I2C_mstrWrBufIndex = 1u;   /* Set index to 2nd element */

                            /* Set transmit state until done */
                            MPU6050_I2C_state = MPU6050_I2C_SM_MSTR_WR_DATA;
                        }
                        /* End of buffer: complete writing */
                        else if(MPU6050_I2C_CHECK_NO_STOP(MPU6050_I2C_mstrControl))
                        {
                            /* Set write complete and master halted */
                            MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_XFER_HALT |
                                                            MPU6050_I2C_MSTAT_WR_CMPLT);

                            MPU6050_I2C_state = MPU6050_I2C_SM_MSTR_HALT; /* Expect ReStart */
                            MPU6050_I2C_DisableInt();
                        }
                        else
                        {
                            MPU6050_I2C_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                            MPU6050_I2C_GENERATE_STOP;
                        }
                    }
                    else  /* Master receive data */
                    {
                        MPU6050_I2C_READY_TO_READ; /* Release bus to read data */

                        MPU6050_I2C_state  = MPU6050_I2C_SM_MSTR_RD_DATA;
                    }
                }
                /* Address is NACKed */
                else if(MPU6050_I2C_CHECK_ADDR_NAK(tmpCsr))
                {
                    /* Set Address NAK error */
                    MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_ERR_XFER |
                                                    MPU6050_I2C_MSTAT_ERR_ADDR_NAK);

                    if(MPU6050_I2C_CHECK_NO_STOP(MPU6050_I2C_mstrControl))
                    {
                        MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_XFER_HALT |
                                                        MPU6050_I2C_GET_MSTAT_CMPLT);

                        MPU6050_I2C_state = MPU6050_I2C_SM_MSTR_HALT; /* Expect RESTART */
                        MPU6050_I2C_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        MPU6050_I2C_ENABLE_INT_ON_STOP; /* Enable interrupt on Stop, to catch it */
                        MPU6050_I2C_GENERATE_STOP;
                    }
                }
                else
                {
                    /* Address phase is not set for some reason: error */
                    #if(MPU6050_I2C_TIMEOUT_ENABLED)
                        /* Exit interrupt to take chance for timeout timer to handle this case */
                        MPU6050_I2C_DisableInt();
                        MPU6050_I2C_ClearPendingInt();
                    #else
                        /* Block execution flow: unexpected condition */
                        CYASSERT(0u != 0u);
                    #endif /* (MPU6050_I2C_TIMEOUT_ENABLED) */
                }
                break;

            case MPU6050_I2C_SM_MSTR_WR_DATA:

                if(MPU6050_I2C_CHECK_DATA_ACK(tmpCsr))
                {
                    /* Check if end of buffer */
                    if(MPU6050_I2C_mstrWrBufIndex  < MPU6050_I2C_mstrWrBufSize)
                    {
                        MPU6050_I2C_DATA_REG =
                                                 MPU6050_I2C_mstrWrBufPtr[MPU6050_I2C_mstrWrBufIndex];
                        MPU6050_I2C_TRANSMIT_DATA;
                        MPU6050_I2C_mstrWrBufIndex++;
                    }
                    /* End of buffer: complete writing */
                    else if(MPU6050_I2C_CHECK_NO_STOP(MPU6050_I2C_mstrControl))
                    {
                        /* Set write complete and master halted */
                        MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_XFER_HALT |
                                                        MPU6050_I2C_MSTAT_WR_CMPLT);

                        MPU6050_I2C_state = MPU6050_I2C_SM_MSTR_HALT;    /* Expect restart */
                        MPU6050_I2C_DisableInt();
                    }
                    else  /* Do normal Stop */
                    {
                        MPU6050_I2C_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                        MPU6050_I2C_GENERATE_STOP;
                    }
                }
                /* Last byte NAKed: end writing */
                else if(MPU6050_I2C_CHECK_NO_STOP(MPU6050_I2C_mstrControl))
                {
                    /* Set write complete, short transfer and master halted */
                    MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_ERR_XFER       |
                                                    MPU6050_I2C_MSTAT_ERR_SHORT_XFER |
                                                    MPU6050_I2C_MSTAT_XFER_HALT      |
                                                    MPU6050_I2C_MSTAT_WR_CMPLT);

                    MPU6050_I2C_state = MPU6050_I2C_SM_MSTR_HALT;    /* Expect ReStart */
                    MPU6050_I2C_DisableInt();
                }
                else  /* Do normal Stop */
                {
                    MPU6050_I2C_ENABLE_INT_ON_STOP;    /* Enable interrupt on Stop, to catch it */
                    MPU6050_I2C_GENERATE_STOP;

                    /* Set short transfer and error flag */
                    MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_ERR_SHORT_XFER |
                                                    MPU6050_I2C_MSTAT_ERR_XFER);
                }

                break;

            case MPU6050_I2C_SM_MSTR_RD_DATA:

                MPU6050_I2C_mstrRdBufPtr[MPU6050_I2C_mstrRdBufIndex] = MPU6050_I2C_DATA_REG;
                MPU6050_I2C_mstrRdBufIndex++;

                /* Check if end of buffer */
                if(MPU6050_I2C_mstrRdBufIndex < MPU6050_I2C_mstrRdBufSize)
                {
                    MPU6050_I2C_ACK_AND_RECEIVE;       /* ACK and receive byte */
                }
                /* End of buffer: complete reading */
                else if(MPU6050_I2C_CHECK_NO_STOP(MPU6050_I2C_mstrControl))
                {
                    /* Set read complete and master halted */
                    MPU6050_I2C_mstrStatus |= (MPU6050_I2C_MSTAT_XFER_HALT |
                                                    MPU6050_I2C_MSTAT_RD_CMPLT);

                    MPU6050_I2C_state = MPU6050_I2C_SM_MSTR_HALT;    /* Expect ReStart */
                    MPU6050_I2C_DisableInt();
                }
                else
                {
                    MPU6050_I2C_ENABLE_INT_ON_STOP;
                    MPU6050_I2C_NAK_AND_RECEIVE;       /* NACK and TRY to generate Stop */
                }
                break;

            default: /* This is an invalid state and should not occur */

            #if(MPU6050_I2C_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handles this case */
                MPU6050_I2C_DisableInt();
                MPU6050_I2C_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (MPU6050_I2C_TIMEOUT_ENABLED) */

                break;
            }
        }

        /* Catches Stop: end of transaction */
        if(MPU6050_I2C_CHECK_STOP_STS(tmpCsr))
        {
            MPU6050_I2C_mstrStatus |= MPU6050_I2C_GET_MSTAT_CMPLT;

            MPU6050_I2C_DISABLE_INT_ON_STOP;
            MPU6050_I2C_state = MPU6050_I2C_SM_IDLE;
        }
    #endif /* (MPU6050_I2C_MODE_MASTER_ENABLED) */
    }
    else if(MPU6050_I2C_CHECK_SM_SLAVE)
    {
    #if(MPU6050_I2C_MODE_SLAVE_ENABLED)

        if((MPU6050_I2C_CHECK_STOP_STS(tmpCsr)) || /* Stop || Restart */
           (MPU6050_I2C_CHECK_BYTE_COMPLETE(tmpCsr) && MPU6050_I2C_CHECK_ADDRESS_STS(tmpCsr)))
        {
            /* Catch end of master write transaction: use interrupt on Stop */
            /* The Stop bit history on address phase does not have correct state */
            if(MPU6050_I2C_SM_SL_WR_DATA == MPU6050_I2C_state)
            {
                MPU6050_I2C_DISABLE_INT_ON_STOP;

                MPU6050_I2C_slStatus &= ((uint8) ~MPU6050_I2C_SSTAT_WR_BUSY);
                MPU6050_I2C_slStatus |= ((uint8)  MPU6050_I2C_SSTAT_WR_CMPLT);

                MPU6050_I2C_state = MPU6050_I2C_SM_IDLE;
            }
        }

        if(MPU6050_I2C_CHECK_BYTE_COMPLETE(tmpCsr))
        {
            /* The address only issued after Start or ReStart: so check the address
               to catch these events:
                FF : sets an address phase with a byte_complete interrupt trigger.
                UDB: sets an address phase immediately after Start or ReStart. */
            if(MPU6050_I2C_CHECK_ADDRESS_STS(tmpCsr))
            {
            /* Check for software address detection */
            #if(MPU6050_I2C_SW_ADRR_DECODE)
                tmp8 = MPU6050_I2C_GET_SLAVE_ADDR(MPU6050_I2C_DATA_REG);

                if(tmp8 == MPU6050_I2C_slAddress)   /* Check for address match */
                {
                    if(0u != (MPU6050_I2C_DATA_REG & MPU6050_I2C_READ_FLAG))
                    {
                        /* Place code to prepare read buffer here                  */
                        /* `#START MPU6050_I2C_SW_PREPARE_READ_BUF_interrupt` */

                        /* `#END` */

                    #ifdef MPU6050_I2C_SW_PREPARE_READ_BUF_CALLBACK
                        MPU6050_I2C_SwPrepareReadBuf_Callback();
                    #endif /* MPU6050_I2C_SW_PREPARE_READ_BUF_CALLBACK */
                        
                        /* Prepare next operation to read, get data and place in data register */
                        if(MPU6050_I2C_slRdBufIndex < MPU6050_I2C_slRdBufSize)
                        {
                            /* Load first data byte from array */
                            MPU6050_I2C_DATA_REG = MPU6050_I2C_slRdBufPtr[MPU6050_I2C_slRdBufIndex];
                            MPU6050_I2C_ACK_AND_TRANSMIT;
                            MPU6050_I2C_slRdBufIndex++;

                            MPU6050_I2C_slStatus |= MPU6050_I2C_SSTAT_RD_BUSY;
                        }
                        else    /* Overflow: provide 0xFF on bus */
                        {
                            MPU6050_I2C_DATA_REG = MPU6050_I2C_OVERFLOW_RETURN;
                            MPU6050_I2C_ACK_AND_TRANSMIT;

                            MPU6050_I2C_slStatus  |= (MPU6050_I2C_SSTAT_RD_BUSY |
                                                           MPU6050_I2C_SSTAT_RD_ERR_OVFL);
                        }

                        MPU6050_I2C_state = MPU6050_I2C_SM_SL_RD_DATA;
                    }
                    else  /* Write transaction: receive 1st byte */
                    {
                        MPU6050_I2C_ACK_AND_RECEIVE;
                        MPU6050_I2C_state = MPU6050_I2C_SM_SL_WR_DATA;

                        MPU6050_I2C_slStatus |= MPU6050_I2C_SSTAT_WR_BUSY;
                        MPU6050_I2C_ENABLE_INT_ON_STOP;
                    }
                }
                else
                {
                    /*     Place code to compare for additional address here    */
                    /* `#START MPU6050_I2C_SW_ADDR_COMPARE_interruptStart` */

                    /* `#END` */

                #ifdef MPU6050_I2C_SW_ADDR_COMPARE_ENTRY_CALLBACK
                    MPU6050_I2C_SwAddrCompare_EntryCallback();
                #endif /* MPU6050_I2C_SW_ADDR_COMPARE_ENTRY_CALLBACK */
                    
                    MPU6050_I2C_NAK_AND_RECEIVE;   /* NACK address */

                    /* Place code to end of condition for NACK generation here */
                    /* `#START MPU6050_I2C_SW_ADDR_COMPARE_interruptEnd`  */

                    /* `#END` */

                #ifdef MPU6050_I2C_SW_ADDR_COMPARE_EXIT_CALLBACK
                    MPU6050_I2C_SwAddrCompare_ExitCallback();
                #endif /* MPU6050_I2C_SW_ADDR_COMPARE_EXIT_CALLBACK */
                }

            #else /* (MPU6050_I2C_HW_ADRR_DECODE) */

                if(0u != (MPU6050_I2C_DATA_REG & MPU6050_I2C_READ_FLAG))
                {
                    /* Place code to prepare read buffer here                  */
                    /* `#START MPU6050_I2C_HW_PREPARE_READ_BUF_interrupt` */

                    /* `#END` */
                    
                #ifdef MPU6050_I2C_HW_PREPARE_READ_BUF_CALLBACK
                    MPU6050_I2C_HwPrepareReadBuf_Callback();
                #endif /* MPU6050_I2C_HW_PREPARE_READ_BUF_CALLBACK */

                    /* Prepare next operation to read, get data and place in data register */
                    if(MPU6050_I2C_slRdBufIndex < MPU6050_I2C_slRdBufSize)
                    {
                        /* Load first data byte from array */
                        MPU6050_I2C_DATA_REG = MPU6050_I2C_slRdBufPtr[MPU6050_I2C_slRdBufIndex];
                        MPU6050_I2C_ACK_AND_TRANSMIT;
                        MPU6050_I2C_slRdBufIndex++;

                        MPU6050_I2C_slStatus |= MPU6050_I2C_SSTAT_RD_BUSY;
                    }
                    else    /* Overflow: provide 0xFF on bus */
                    {
                        MPU6050_I2C_DATA_REG = MPU6050_I2C_OVERFLOW_RETURN;
                        MPU6050_I2C_ACK_AND_TRANSMIT;

                        MPU6050_I2C_slStatus  |= (MPU6050_I2C_SSTAT_RD_BUSY |
                                                       MPU6050_I2C_SSTAT_RD_ERR_OVFL);
                    }

                    MPU6050_I2C_state = MPU6050_I2C_SM_SL_RD_DATA;
                }
                else  /* Write transaction: receive 1st byte */
                {
                    MPU6050_I2C_ACK_AND_RECEIVE;
                    MPU6050_I2C_state = MPU6050_I2C_SM_SL_WR_DATA;

                    MPU6050_I2C_slStatus |= MPU6050_I2C_SSTAT_WR_BUSY;
                    MPU6050_I2C_ENABLE_INT_ON_STOP;
                }

            #endif /* (MPU6050_I2C_SW_ADRR_DECODE) */
            }
            /* Data states */
            /* Data master writes into slave */
            else if(MPU6050_I2C_state == MPU6050_I2C_SM_SL_WR_DATA)
            {
                if(MPU6050_I2C_slWrBufIndex < MPU6050_I2C_slWrBufSize)
                {
                    tmp8 = MPU6050_I2C_DATA_REG;
                    MPU6050_I2C_ACK_AND_RECEIVE;
                    MPU6050_I2C_slWrBufPtr[MPU6050_I2C_slWrBufIndex] = tmp8;
                    MPU6050_I2C_slWrBufIndex++;
                }
                else  /* of array: complete write, send NACK */
                {
                    MPU6050_I2C_NAK_AND_RECEIVE;

                    MPU6050_I2C_slStatus |= MPU6050_I2C_SSTAT_WR_ERR_OVFL;
                }
            }
            /* Data master reads from slave */
            else if(MPU6050_I2C_state == MPU6050_I2C_SM_SL_RD_DATA)
            {
                if(MPU6050_I2C_CHECK_DATA_ACK(tmpCsr))
                {
                    if(MPU6050_I2C_slRdBufIndex < MPU6050_I2C_slRdBufSize)
                    {
                         /* Get data from array */
                        MPU6050_I2C_DATA_REG = MPU6050_I2C_slRdBufPtr[MPU6050_I2C_slRdBufIndex];
                        MPU6050_I2C_TRANSMIT_DATA;
                        MPU6050_I2C_slRdBufIndex++;
                    }
                    else   /* Overflow: provide 0xFF on bus */
                    {
                        MPU6050_I2C_DATA_REG = MPU6050_I2C_OVERFLOW_RETURN;
                        MPU6050_I2C_TRANSMIT_DATA;

                        MPU6050_I2C_slStatus |= MPU6050_I2C_SSTAT_RD_ERR_OVFL;
                    }
                }
                else  /* Last byte was NACKed: read complete */
                {
                    /* Only NACK appears on bus */
                    MPU6050_I2C_DATA_REG = MPU6050_I2C_OVERFLOW_RETURN;
                    MPU6050_I2C_NAK_AND_TRANSMIT;

                    MPU6050_I2C_slStatus &= ((uint8) ~MPU6050_I2C_SSTAT_RD_BUSY);
                    MPU6050_I2C_slStatus |= ((uint8)  MPU6050_I2C_SSTAT_RD_CMPLT);

                    MPU6050_I2C_state = MPU6050_I2C_SM_IDLE;
                }
            }
            else
            {
            #if(MPU6050_I2C_TIMEOUT_ENABLED)
                /* Exit interrupt to take chance for timeout timer to handle this case */
                MPU6050_I2C_DisableInt();
                MPU6050_I2C_ClearPendingInt();
            #else
                /* Block execution flow: unexpected condition */
                CYASSERT(0u != 0u);
            #endif /* (MPU6050_I2C_TIMEOUT_ENABLED) */
            }
        }
    #endif /* (MPU6050_I2C_MODE_SLAVE_ENABLED) */
    }
    else
    {
        /* The FSM skips master and slave processing: return to IDLE */
        MPU6050_I2C_state = MPU6050_I2C_SM_IDLE;
    }

#ifdef MPU6050_I2C_ISR_EXIT_CALLBACK
    MPU6050_I2C_ISR_ExitCallback();
#endif /* MPU6050_I2C_ISR_EXIT_CALLBACK */    
}


#if ((MPU6050_I2C_FF_IMPLEMENTED) && (MPU6050_I2C_WAKEUP_ENABLED))
    /*******************************************************************************
    * Function Name: MPU6050_I2C_WAKEUP_ISR
    ********************************************************************************
    *
    * Summary:
    *  The interrupt handler to trigger after a wakeup.
    *
    * Parameters:
    *  None.
    *
    * Return:
    *  None.
    *
    *******************************************************************************/
    CY_ISR(MPU6050_I2C_WAKEUP_ISR)
    {
    #ifdef MPU6050_I2C_WAKEUP_ISR_ENTRY_CALLBACK
        MPU6050_I2C_WAKEUP_ISR_EntryCallback();
    #endif /* MPU6050_I2C_WAKEUP_ISR_ENTRY_CALLBACK */
         
        /* Set flag to notify that matched address is received */
        MPU6050_I2C_wakeupSource = 1u;

        /* SCL is stretched until the I2C_Wake() is called */

    #ifdef MPU6050_I2C_WAKEUP_ISR_EXIT_CALLBACK
        MPU6050_I2C_WAKEUP_ISR_ExitCallback();
    #endif /* MPU6050_I2C_WAKEUP_ISR_EXIT_CALLBACK */
    }
#endif /* ((MPU6050_I2C_FF_IMPLEMENTED) && (MPU6050_I2C_WAKEUP_ENABLED)) */


/* [] END OF FILE */
