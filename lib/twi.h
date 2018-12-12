/** 
 * Two Wire Interface / i2C Communication
 *
 * Copyright (C) 2018 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       12.12.2018
 * @file        twi.h
 * @tested      AVR Atmega16 / AVR Atmega8
 * @inspiration
 * ---------------------------------------------------
 */
#ifndef __TWI_H__
#define __TWI_H__

  // define clock
  #if defined(__AVR_ATMEGA8__)
    #define _FCPU 8000000
  #elif defined(__AVR_ATMEGA16__)
    #define _FCPU 16000000
  #endif

  // define register for TWI communication
  #if defined(__AVR_ATMEGA16__) 
    #define TWI
  #endif

#endif
