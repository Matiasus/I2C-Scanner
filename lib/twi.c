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
 * @description TWI init - initialize frequency
 *
 * @param  void
 * @return void
 */
void TWI_init()
{
  // set TWI frequency to 400 kHz
  // fclk = (fcpu)/(16+2*TWBR*4*Prescaler)
  // @param1 value of TWBR
  // @param2 value of Prescaler
  TWI_FREQ(3,1);
}

/**
 * @description TWI send - Master Transmitter Mode
 *
 * @param  void
 * @return unsigned char 
 */
unsigned char TWI_MT_send_byte(unsigned char address, char data)
{
  // Start TWI
  // -------------------------------------------------
  // send start sequence
  TWI_START();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // start condition has not been transmitted
  if (TWI_STATUS_CODE != TWI_MT_START) {
   // error
   return 0;
  }
  // Send Address + Write flag
  // -------------------------------------------------
  // fill TWDR register with SLave Address + Write
  TWI_SLA_W(address);
  // send address
  TWI_ALLOW_SEND();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // check if not ACK has been received
  if (TWI_STATUS_CODE != TWI_MT_SLAW_ACK) {
   // error
   return 0;
  }
  // Send DATA
  // -------------------------------------------------
  // fill TWDR register with data
  TWI_SLA_W(data);
  // send address
  TWI_ALLOW_SEND();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET();
  // check if not ACK has been received
  if (TWI_STATUS_CODE != TWI_MT_DATA_ACK) {
   // error
   return 0;
  }  
  // Stop TWI
  // -------------------------------------------------
  // send stop sequence
  TWI_START();
  // wait for TWINT flag is set
  TWI_WAIT_TILL_TWINT_IS_SET(); 
  // END
  // -------------------------------------------------
  // success
  return 1;
}
