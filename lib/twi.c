/** 
 * Two Wire Interface / I2C Communication
 *
 * Copyright (C) 2018 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       12.12.2018
 * @file        twi.c
 * @tested      AVR Atmega16 / AVR Atmega8
 * @inspiration
 * ---------------------------------------------------
 */
 
// include libraries
#include <stdio.h>
#include <avr/io.h>
#include "twi.h"

/**
 * @desc    TWI init - initialize frequency
 *
 * @param   void
 * @return  void
 */
void TWI_Init()
{
  // +++++++++++++++++++++++++++++++++++++++++++++
  // Calculation fclk:
  //
  // fclk = (fcpu)/(16+2*TWBR*4^Prescaler)
  // --------------------------------------------- 
  // Calculation TWBR:
  // 
  // TWBR = {(fcpu/fclk) - 16 } / (2*4^Prescaler)
  // +++++++++++++++++++++++++++++++++++++++++++++
  // @param1 value of TWBR, 
  //    fclk = 400 kHz; TWBR = 3
  //    fclk = 100 kHz; TWBR = 20
  // @param2 value of Prescaler = 1
  TWI_FREQ(3,1);
}

/**
 * @desc    TWI start
 *
 * @param   unsigned char
 * @return  unsigned char 
 */
unsigned char TWI_Transmit(EType type, unsigned char slave_address)
{
  // sitch type of transmission
  switch (type) {
    // Start & Write
    case eSTART_SLAW:
      return TWI_Start(slave_address, (eWRITE & 0xFF));
    // Start & Read
    case eSTART_SLAR:
      return TWI_Start(slave_address, (eREAD & 0xFF));
    // Repeated start & Write 
    case eREPEATED_SLAW:
      return TWI_Repeated_start(slave_address, (eWRITE & 0xFF));
    // Repeated start & Read
    case eREPEATED_SLAR:
      return TWI_Repeated_start(slave_address, (eREAD & 0xFF));
    // not switched
    default:
      // unsuccess
      return UNSUCCESS;
  }
  // success
  return SUCCESS;
}

/**
 * @desc    TWI start
 *
 * @param   unsigned char
 * @return  unsigned char 
 */
unsigned char TWI_Start_condition(unsigned char status_code)
{
  // Start TWI
  // -------------------------------------------------
  // send start sequence
  TWI_START();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // start condition has not been transmitted
  if (TWI_STATUS_CODE != status_code) {
   // error
   return TWI_STATUS_CODE;
  }
  // success
  return SUCCESS;
}

/**
 * @desc    TWI start - Master Mode
 *
 * @param   unsigned char - slave address
 * @param   EOperation - eWrite -> write
 *                       eRead -> read
 * @return  unsigned char 
 */
unsigned char TWI_Start(unsigned char address, EOperation operation)
{
  // start TWI communication
  // @param Successful status code for start 0x08
  unsigned char status = TWI_Start_condition(TWI_START);
  // fill TWDR register with low byte of enum Operation
  //  read  = 0
  //  write = 1
  TWI_SLA_RW(address, (operation & 0xFF));
  // send address
  TWI_ALLOW_NACK();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // check high byte of enum Operation if not ACK has been received
  //  write TWI_MT_SLAW_ACK
  //  read  TWI_MR_SLAR_ACK
  if (TWI_STATUS_CODE != ((operation >> 8) & 0xFF)) {
   // error
   return TWI_STATUS_CODE;
  }
  // success
  return status;
}

/**
 * @desc    TWI Repeated start - Master Transmitter Mode
 *
 * @param   EOperation - eWrite -> write
 *                       eRead -> read  
 * @return  unsigned char 
 */
unsigned char TWI_Repeated_start(unsigned char address, EOperation operation)
{
  // start TWI communication
  // @param Successful status code for repeated start 0x10
  unsigned char status = TWI_Start_condition(TWI_START_REPEAT);
  // fill TWDR register with low byte of enum Operation
  //  read  = 0
  //  write = 1
  TWI_SLA_RW(address, (operation & 0xFF));
  // send address
  TWI_ALLOW_NACK();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // check high byte of enum Operation if not ACK has been received
  //  read  - TWI_MR_SLAR_ACK 0x40
  //  write - TWI_MT_SLAW_ACK 0x18
  if (TWI_STATUS_CODE != ((operation >> 8) & 0xFF)) {
   // error
   return TWI_STATUS_CODE;
  }
  // success
  return status;
}

/**
 * @desc    TWI stop
 *
 * @param   void
 * @return  void
 */
void TWI_Stop(void)
{
  // End TWI
  // -------------------------------------------------
  // send stop sequence
  TWI_STOP();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
}

/**
 * @desc    TWI send one byte - Master Transmitter Mode
 *
 * @param   unsigned char
 * @return  unsigned char 
 */
unsigned char TWI_MT_send_byte(unsigned char data)
{
  // Send DATA
  // -------------------------------------------------
  // fill TWDR register with data
  TWI_TWDR = data;
  // send address
  TWI_ALLOW_NACK();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // check if not ACK has been received
  if (TWI_STATUS_CODE != TWI_MT_DATA_ACK) {
   // error
   return TWI_error(TWI_STATUS_CODE);
  }
  // -------------------------------------------------
  // success
  return SUCCESS;
}

/**
 * @desc    TWI receive one byte with acknowledgement
 *
 * @param   void
 * @return  unsigned char 
 */
unsigned char TWI_MT_read_ack(void)
{
  // read byte with acknowledgement
  TWI_ALLOW_ACK();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // retrun recieved byte
  return TWDR;
}

/**
 * @desc    TWI receive one byte with NOT acknowledgement
 *
 * @param   void
 * @return  unsigned char 
 */
unsigned char TWI_MT_read_nack(void)
{
  // read byte with NOT acknowledgement
  TWI_ALLOW_NACK();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // retrun recieved byte
  return TWDR;
}

/**
 * @desc    TWI error
 *
 * @param   unsigned char -  status code
 * @return  unsigned char 
 */
unsigned char TWI_error(unsigned char status)
{
  // if no ACK received after address transmitted in MT mode
  if (status == TWI_MT_SLAW_NACK) {
    // send STOP
    TWI_Stop();
  }
  // return
  return SUCCESS;
}