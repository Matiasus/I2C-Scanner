/** 
 * Two Wire Interface / I2C Communication
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
    #define TWI_TWAR TWAR
    #define TWI_TWBR TWBR
    #define TWI_TWCR TWCR
    #define TWI_TWSR TWSR
  #endif

  

  // TWI CLK frequency
  #define TWI_FREQ(BIT_RATE, PRESCALER) ((_FCPU)/(16UL+2*BIT_RATE*4^PRESCALER)-1)

  // Slave address
  #define TWI_SLA = 0x01
  // SLave Address & Write
  #define TWI_SLA_W = (TWI_SLA << 1)
  // SLave Address & Read
  #define TWI_SLA_R = (TWI_SLA << 1) | 0x01

  // ************************
  // Master Transmitter Mode
  // ************************
  #define TWI_MT_START = 0x08
  #define TWI_MT_START_REPEAT = 0x10
  #define TWI_MT_START_ACK = 0x18
  #define TWI_MT_START_NACK = 0x20
  #define TWI_MT_DATA_ACK = 0x28
  #define TWI_MT_DATA_NACK = 0x30
  #define TWI_MT_ARB_LOST = 0x38

  /**
   * @description TWI init - 
   *
   * @param  void
   * @return void
   */
  void TWI_init();

  /**
   * @description TWI start
   *
   * @param  void
   * @return void
   */
  void TWI_start();

#endif
