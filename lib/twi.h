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
    #define TWI_TWAR TWAR // TWI (Slave) Address Register
    #define TWI_TWBR TWBR // TWI Bit Rate Register
    #define TWI_TWDR TWDR // TWI Data Register
    #define TWI_TWCR TWCR // TWI Control Register
    #define TWI_TWSR TWSR // TWI Status Register
  #endif

  // TWI CLK frequency
  // TWPS1 TWPS0  - PRESCALER
  //   0     0    -     1
  //   0     1    -     4
  //   1     0    -    16
  //   1     1    -    64
  #define TWI_FREQ(BIT_RATE, PRESCALER) { TWI_TWBR = BIT_RATE; TWI_TWSR |= (TWI_TWSR & 0x03) | PRESCALER; }

  // TWI test if TWINT Flag is ste
  #define TWI_TWINT_SET() { while (!(TWI_TWCR & (1 << TWINT))); }

  // TWI mask status rgister
  #define TWI_STAT() { TWI_TWSR & 0xF8; }

  // SLave Address & Write
  #define TWI_SLA_W (SLAVE_ADDRESS) (TWI_TWDR = (SLAVE_ADDRESS << 1))
  // SLave Address & Read
  #define TWI_SLA_R (SLAVE_ADDRESS) (TWI_TWDR = (SLAVE_ADDRESS << 1) |= 0x01;)

  // ************************
  // Master Transmitter Mode
  // ************************
  #define TWI_MT_START        = 0x08
  #define TWI_MT_START_REPEAT = 0x10
  #define TWI_MT_START_ACK    = 0x18
  #define TWI_MT_START_NACK   = 0x20
  #define TWI_MT_DATA_ACK     = 0x28
  #define TWI_MT_DATA_NACK    = 0x30
  #define TWI_MT_ARB_LOST     = 0x38

  /**
   * @description TWI init - initialise communication
   *
   * @param  void
   * @return void
   */
  void TWI_init();

  /**
   * @description TWI start - Master Transmitter Mode
   *
   * @param  void
   * @return void
   */
  void TWI_MT_start();

  /**
   * @description TWI repeated start - Master Transmitter Mode
   *
   * @param  void
   * @return void
   */
  void TWI_MT_repeat_start();

#endif
