/** 
 * Two Wire Interface / I2C Communication - example
 *
 * Copyright (C) 2018 Marian Hrinko.
 * Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       12.12.2018
 * @file        main.c
 * @tested      AVR Atmega16 / AVR Atmega8
 * @inspiration
 * ---------------------------------------------------
 */
 
// include libraries
#include <stdio.h>
#include <avr/io.h>
#include "twi.h"

#define SLAVE_ADDRESS_24C16 0x01

unsigned char array[10] = "test";

/**
 * @desc    Write
 *
 * @param   void
 * @return  unsigned char
 */
unsigned char write()
{
  unsigned char i;
  
  // Init TWI
  TWI_Init();
  // -------------------------------------------------------
  // Start in master transmitter mode - send slave address
  if (SUCCESS != TWI_Start(eSTART_SLAW, SLAVE_ADDRESS_24C16)) {
    // go to error
    error();
  }
  // -------------------------------------------------------  
  // Send address memory location
  if (SUCCESS !=TWI_MT_send_byte(0x00)) {
    // go to error
    error();
  }  
  // -------------------------------------------------------
  // send content of array into memory
  for (i = 0; i < strlen(array); i++) {
    // send data byte
    if (SUCCESS !=TWI_MT_send_byte(array[i])) {
      // go to error
      error(); 
    }
  }
  // -------------------------------------------------------   
  // TWI - end communication with slave
  TWI_Stop();
  
  // return value
  return 0;
}

/**
 * @desc    Read
 *
 * @param   void
 * @return  unsigned char
 */
unsigned char read()
{
  unsigned char i;
  
  // Init TWI
  TWI_Init();
  // -------------------------------------------------------
  // Start in master transmitter mode - send slave address
  if (SUCCESS != TWI_Start(eSTART_SLAW, SLAVE_ADDRESS_24C16)) {
    // go to error
    error();
  }
  // -------------------------------------------------------  
  // Send address memory location
  if (SUCCESS !=TWI_MT_send_byte(0x00)) {
    // go to error
    error();
  }
    // -------------------------------------------------------  
  // Send address memory location
  if (SUCCESS !=TWI_Start(eREPEATED_SLAR, SLAVE_ADDRESS_24C16)) {
    // go to error
    error();
  }
  // -------------------------------------------------------
  // read content of array into memory
  for (i = 0; i < strlen(array); i++) {
    // read data byte
    content[i] = TWI_MT_read_ack();
  }
  // -------------------------------------------------------  
  // read last data byte
  TWI_MT_read_nack();
  // -------------------------------------------------------   
  // TWI - end communication with slave
  TWI_Stop();
  
  // return value
  return 0;
}

/**
 * @desc    Error
 *
 * @param   void
 * @return  unsigned char
 */
unsigned char error()
{
  // TWI - end communication with slave
  TWI_Stop();
  // return
  return 0;
}