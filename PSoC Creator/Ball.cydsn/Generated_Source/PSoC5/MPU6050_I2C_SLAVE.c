/*******************************************************************************
* File Name: MPU6050_I2C_SLAVE.c
* Version 3.50
*
* Description:
*  This file provides the source code of APIs for the I2C component Slave mode.
*
* Note:
*
*******************************************************************************
* Copyright 2012-2015, Cypress Semiconductor Corporation. All rights reserved.
* You may use this file only in accordance with the license, terms, conditions,
* disclaimers, and limitations in the end user license agreement accompanying
* the software package with which this file was provided.
*******************************************************************************/

#include "MPU6050_I2C_PVT.h"

#if (MPU6050_I2C_MODE_SLAVE_ENABLED)

/**********************************
*      System variables
**********************************/

volatile uint8 MPU6050_I2C_slStatus;   /* Slave Status  */

/* Transmit buffer variables */
volatile uint8 * MPU6050_I2C_slRdBufPtr;   /* Pointer to Transmit buffer  */
volatile uint8   MPU6050_I2C_slRdBufSize;  /* Slave Transmit buffer size  */
volatile uint8   MPU6050_I2C_slRdBufIndex; /* Slave Transmit buffer Index */

/* Receive buffer variables */
volatile uint8 * MPU6050_I2C_slWrBufPtr;   /* Pointer to Receive buffer  */
volatile uint8   MPU6050_I2C_slWrBufSize;  /* Slave Receive buffer size  */
volatile uint8   MPU6050_I2C_slWrBufIndex; /* Slave Receive buffer Index */

#if (MPU6050_I2C_SW_ADRR_DECODE)
    volatile uint8 MPU6050_I2C_slAddress;  /* Software address variable */
#endif /* (MPU6050_I2C_SW_ADRR_DECODE) */


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveStatus
********************************************************************************
*
* Summary:
*  Returns I2C slave's communication status.
*
* Parameters:
*  None.
*
* Return:
*  Current status of I2C slave.
*
* Global variables:
*  MPU6050_I2C_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
*******************************************************************************/
uint8 MPU6050_I2C_SlaveStatus(void) 
{
    return (MPU6050_I2C_slStatus);
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveClearReadStatus
********************************************************************************
*
* Summary:
*  Clears the read status flags and returns they values.
*  The MPU6050_I2C_SSTAT_RD_BUSY flag is not effected by clear.
*
* Parameters:
*  None.
*
* Return:
*  Current read status of I2C slave.
*
* Global variables:
*  MPU6050_I2C_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 MPU6050_I2C_SlaveClearReadStatus(void) 
{
    uint8 status;

    MPU6050_I2C_DisableInt(); /* Lock from interrupt */

    /* Mask of transfer complete flag and Error status */
    status = (MPU6050_I2C_slStatus & MPU6050_I2C_SSTAT_RD_MASK);
    MPU6050_I2C_slStatus &= (uint8) ~MPU6050_I2C_SSTAT_RD_CLEAR;

    MPU6050_I2C_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveClearWriteStatus
********************************************************************************
*
* Summary:
*  Clears the write status flags and returns they values.
*  The MPU6050_I2C_SSTAT_WR_BUSY flag is not effected by clear.
*
* Parameters:
*  None.
*
* Return:
*  Current write status of I2C slave.
*
* Global variables:
*  MPU6050_I2C_slStatus - The global variable used to store a current
*                              status of the I2C slave.
*
* Reentrant:
*  No.
*
*******************************************************************************/
uint8 MPU6050_I2C_SlaveClearWriteStatus(void) 
{
    uint8 status;

    MPU6050_I2C_DisableInt(); /* Lock from interrupt */

    /* Mask of transfer complete flag and Error status */
    status = (MPU6050_I2C_slStatus & MPU6050_I2C_SSTAT_WR_MASK);
    MPU6050_I2C_slStatus &= (uint8) ~MPU6050_I2C_SSTAT_WR_CLEAR;

    MPU6050_I2C_EnableInt(); /* Release lock */

    return (status);
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveSetAddress
********************************************************************************
*
* Summary:
*  Sets the I2C slave address.
*
* Parameters:
*  address: I2C slave address for the primary device. This value may be any
*  address between 0 and 127.
*
* Return:
*  None.
*
* Global variables:
*  MPU6050_I2C_Address  - The global variable used to store an I2C slave
*                              address for the primary device when the software
*                              address detect feature is used.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MPU6050_I2C_SlaveSetAddress(uint8 address) 
{
#if (MPU6050_I2C_SW_ADRR_DECODE)
    MPU6050_I2C_slAddress = (address & MPU6050_I2C_SLAVE_ADDR_MASK);
#else
    MPU6050_I2C_ADDR_REG  = (address & MPU6050_I2C_SLAVE_ADDR_MASK);
#endif /* (MPU6050_I2C_SW_ADRR_DECODE) */
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveInitReadBuf
********************************************************************************
*
* Summary:
*  Sets the buffer pointer and size of the read buffer. This function also
*  resets the transfer count returned with the I2C_SlaveGetReadBufSize function.
*
* Parameters:
*  readBuf:  Pointer to the data buffer to be read by the master.
*  bufSize:  Size of the read buffer exposed to the I2C master.
*
* Return:
*  None.
*
* Global variables:
*  MPU6050_I2C_slRdBufPtr   - The global variable used to store a pointer
*                                  to the slave read buffer.
*  MPU6050_I2C_slRdBufSize  - The global variable used to store a slave
*                                  read buffer size.
*  MPU6050_I2C_slRdBufIndex - The global variable used to store a current
*                                  index within the slave read buffer.
*
* Side Effects:
*  If this function is called during a bus transaction, data from the previous
*  buffer location and the beginning of the current buffer may be transmitted.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MPU6050_I2C_SlaveInitReadBuf(uint8 * rdBuf, uint8 bufSize)
     
{
    if (NULL != rdBuf)
    {
        MPU6050_I2C_DisableInt(); /* Lock from interrupt */

        /* Set buffer pointer */
        MPU6050_I2C_slRdBufPtr   = (volatile uint8 *) rdBuf;
        MPU6050_I2C_slRdBufSize  = bufSize;    /* Set buffer size     */
        MPU6050_I2C_slRdBufIndex = 0u;         /* Clears buffer index */

        MPU6050_I2C_EnableInt(); /* Release lock */
    }
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveInitWriteBuf
********************************************************************************
*
* Summary:
*  Sets the buffer pointer and size of the read buffer. This function also
*  resets the transfer count returned with the I2C_SlaveGetReadBufSize function.
*
* Parameters:
*  writeBuf:  Pointer to the data buffer to be read by the master.
*  bufSize:  Size of the buffer exposed to the I2C master.
*
* Return:
*  None.
*
* Global variables:
*  MPU6050_I2C_slWrBufPtr   - The global variable used to store a pointer
*                                  to the slave write buffer.
*  MPU6050_I2C_slWrBufSize  - The global variable used to store a slave
*                                  write buffer size.
*  MPU6050_I2C_slWrBufIndex - The global variable used to store a current
*                                  index within the slave write buffer.
*
* Side Effects:
*  If this function is called during a bus transaction, data from the previous
*  buffer location and the beginning of the current buffer may be transmitted.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MPU6050_I2C_SlaveInitWriteBuf(uint8 * wrBuf, uint8 bufSize)
     
{
    if (NULL != wrBuf)
    {
        MPU6050_I2C_DisableInt(); /* Lock from interrupt */

         /* Set buffer pointer */
        MPU6050_I2C_slWrBufPtr   = (volatile uint8 *) wrBuf;
        MPU6050_I2C_slWrBufSize  = bufSize;    /* Set buffer size     */
        MPU6050_I2C_slWrBufIndex = 0u;         /* Clears buffer index */

        MPU6050_I2C_EnableInt(); /* Release lock */
    }
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveGetReadBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes read by the I2C master since an
*  I2C_SlaveInitReadBuf or I2C_SlaveClearReadBuf function was executed.
*  The maximum return value will be the size of the read buffer.
*
* Parameters:
*  None.
*
* Return:
*  Bytes read by master.
*
* Global variables:
*  MPU6050_I2C_slRdBufIndex - The global variable used to store a current
*                                  index within the slave read buffer.
*
*******************************************************************************/
uint8 MPU6050_I2C_SlaveGetReadBufSize(void) 
{
    return (MPU6050_I2C_slRdBufIndex);
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveGetWriteBufSize
********************************************************************************
*
* Summary:
*  Returns the number of bytes written by the I2C master since an
*  I2C_SlaveInitWriteBuf or I2C_SlaveClearWriteBuf function was executed.
*  The maximum return value will be the size of the write buffer.
*
* Parameters:
*  None.
*
* Return:
*  Bytes written by master.
*
* Global variables:
*  MPU6050_I2C_slWrBufIndex - The global variable used to store a current
*                                  index within the slave write buffer.
*
*******************************************************************************/
uint8 MPU6050_I2C_SlaveGetWriteBufSize(void) 
{
    return (MPU6050_I2C_slWrBufIndex);
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveClearReadBuf
********************************************************************************
*
* Summary:
*  Resets the read pointer to the first byte in the read buffer. The next byte
*  read by the master will be the first byte in the read buffer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  MPU6050_I2C_slRdBufIndex - The global variable used to store a current
*                                  index the within slave read buffer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MPU6050_I2C_SlaveClearReadBuf(void) 
{
    MPU6050_I2C_slRdBufIndex = 0u;
}


/*******************************************************************************
* Function Name: MPU6050_I2C_SlaveClearRxBuf
********************************************************************************
*
* Summary:
*  Resets the write pointer to the first byte in the write buffer. The next byte
*  written by the master will be the first byte in the write buffer.
*
* Parameters:
*  None.
*
* Return:
*  None.
*
* Global variables:
*  MPU6050_I2C_slWrBufIndex - The global variable used to store a current
*                                  index within the slave write buffer.
*
* Reentrant:
*  No.
*
*******************************************************************************/
void MPU6050_I2C_SlaveClearWriteBuf(void) 
{
    MPU6050_I2C_slWrBufIndex = 0u;
}

#endif /* (MPU6050_I2C_MODE_SLAVE_ENABLED) */


/* [] END OF FILE */
