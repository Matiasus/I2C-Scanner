/** 
 * -------------------------------------------------------------+ 
 * @desc        Two Wire Interface / I2C Communication
 * -------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       06.09.2020
 * @file        twi.c
 * @tested      AVR Atmega16
 * -------------------------------------------------------------+ 
 */
 
// include libraries
#include "twi.h"

/**
 * @desc    TWI init - initialize frequency
 *
 * @param   void
 *
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
  TWI_FREQ(20,1);
}

/**
 * @desc    TWI MT Start
 *
 * @param   void
 *
 * @return  void
 */
unsigned char TWI_MT_Start(void)
{
  // null status flag
  TWI_TWSR &= ~0xA8;
  // START
  // ----------------------------------------------
  // request for bus
  TWI_START();
  // wait till flag set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // test if start acknowledged
  if (TWI_STATUS != TWI_START_ACK) {
    // return status
    return TWI_STATUS;
  }
  // success
  return SUCCESS;
}

/**
 * @desc    TWI Find address
 *
 * @param   void
 *
 * @return  unsigned char
 */
unsigned char TWI_MT_FindDevice(void)
{
  // declaration
  unsigned char status = 0x00;
  unsigned char found = 0x00;
  unsigned char max_addr = 127;

  for (int i=0; i<max_addr; i++) {
    // start
    status = TWI_MT_Start();
    // start
    if ((status != SUCCESS) && (status != TWI_REP_START_ACK)) {
      // return 
      return 0;
    }
    // SLA+W
    // ----------------------------------------------
    TWI_TWDR = (i << 1);
    // enable
    TWI_ENABLE();
    // wait till flag set
    TWI_WAIT_TILL_TWINT_IS_SET();

    // find
    if (TWI_STATUS == TWI_MT_SLAW_ACK) {
      // found
      found = i;
      // cancel looop
      break;
      // STOP
      // ----------------------------------------------
      // stop TWI
      TWI_Stop();
    }
  }
  // return found device address
  return found;
}

/**
 * @desc    TWI stop
 *
 * @param   void
 *
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
