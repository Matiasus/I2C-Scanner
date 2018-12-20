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
 * @description TWI start
 *
 * @param  void
 * @return void
 */
void TWI_start()
{
  // set TWI start conditions
  TWI_TWCR = (1 << TWEN)  |  // TWI enable
             (1 << TWINT) |  // TWI Interrupt Flag - must be cleared by set
             (1 << TWSTA);   // TWI Start
}

/**
 * @description TWI stop
 *
 * @param  void
 * @return void
 */
void TWI_stop()
{
  // set TWI start conditions
  TWI_TWCR = (1 << TWEN)  |  // TWI Enable
             (1 << TWIE) |   // TWI Interrupt Enabled  
             (1 << TWINT) |  // TWI Interrupt Flag - must be cleared by set
             (1 << TWSTA);   // TWI Start
}
