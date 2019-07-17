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
  //  @param TWBR
  //  @param Prescaler
  //    TWPS1 TWPS0  - PRESCALER
  //      0     0    -     1
  //      0     1    -     4
  //      1     0    -    16
  //      1     1    -    64
  #define TWI_FREQ(BIT_RATE, PRESCALER) { TWI_TWBR = BIT_RATE; TWI_TWSR |= (TWI_TWSR & 0x03) | PRESCALER; }
  // TWI test if TWINT Flag is set
  #define TWI_WAIT_TILL_TWINT_IS_SET() { while (!(TWI_TWCR & (1 << TWINT))); }
  // TWI start condition
  // (1 <<  TWEN) - TWI Enable
  // (1 << TWINT) - TWI Interrupt Flag - must be cleared by set
  // (1 << TWSTA) - TWI Start
  #define TWI_START() { TWI_TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTA); }
  // TWI stop condition
  // (1 <<  TWEN) - TWI Enable
  // (1 << TWINT) - TWI Interrupt Flag - must be cleared by set
  // (1 << TWSTO) - TWI Stop
  #define TWI_STOP() { TWI_TWCR = (1 << TWEN) | (1 << TWINT) | (1 << TWSTO); }
  // TWI allow - for sending / reading address or data with no acknowledgement
  // (1 <<  TWEN) - TWI Enable
  // (1 << TWINT) - TWI Interrupt Flag - must be cleared by set
  #define TWI_ALLOW_NACK() { TWI_TWCR = (1 << TWEN) | (1 << TWINT); }
  // TWI allow - for reading data with acknowledgement
  // (1 <<  TWEN) - TWI Enable
  // (1 << TWINT) - TWI Interrupt Flag - must be cleared by set
  // (1 <<  TWEA) - Set enable acknowledge for transmitter
  #define TWI_ALLOW_ACK() { TWI_TWCR = (1 << TWEN) | (1 << TWINT) | (1 <<  TWEA); }
  // Send addres with read / write operation
  //  @param SLave Address
  //  @param Operation - Read / Write
  #define TWI_SLA_RW (SLAVE_ADDRESS, OPERATION) { TWI_TWDR = (SLAVE_ADDRESS << 1); TWI_TWDR |= OPERATION; }
  // TWI mask status rgister
  #define TWI_STATUS_CODE (TWI_TWSR & 0xF8)
  
  // success return value
  #define SUCCESS = 0
  // success return value
  #define UNSUCCESS = 0xff  

  // ++++++++++++++++++++++++++++++++++++++++++
  //
  //        M A S T E R   M O D E
  //
  // ++++++++++++++++++++++++++++++++++++++++++  
  // Master Mode - Transmitter / Receiver
  #define TWI_START           = 0x08  // A START condition has been transmitted
  #define TWI_START_REPEAT    = 0x10  // A repeated START condition has been transmitted
  #define TWI_ARBIT_LOST      = 0x38  // Arbitration lost in SLA+W or NOT ACK bit
  // Master Transmitter Mode
  #define TWI_MT_SLAW_ACK     = 0x18  // SLA+W has been transmitted; ACK has been received
  #define TWI_MT_SLAW_NACK    = 0x20  // SLA+W has been transmitted; NOT ACK has been received
  #define TWI_MT_DATA_ACK     = 0x28  // Data byte has been transmitted; ACK has been received
  #define TWI_MT_DATA_NACK    = 0x30  // Data byte has been transmitted; NOT ACK has been received  
  // Master Receiver Mode
  #define TWI_MR_SLAR_ACK     = 0x40  // SLA+R has been transmitted; ACK has been received
  #define TWI_MR_SLAR_NACK    = 0x48  // SLA+R has been transmitted; NOT ACK has been received
  #define TWI_MR_DATA_ACK     = 0x50  // Data byte has been received; ACK has been received
  #define TWI_MR_DATA_NACK    = 0x58  // Data byte has been received; NOT ACK has been received
  
  // ++++++++++++++++++++++++++++++++++++++++++
  //
  //         S L A V E   M O D E
  //
  // ++++++++++++++++++++++++++++++++++++++++++
  // Slave Receiver Mode
  #define TWI_SR_SLAW_ACK     = 0x60  // Own Slave address has been received; ACK returned
  #define TWI_SR_ALMOA_ACK    = 0x68  // Arbitration Lost in SLA+R/W as Master; Own Slave address has been received; ACK returned
  #define TWI_SR_GCALL_ACK    = 0x70  // General call address has been received; ACK returned
  #define TWI_SR_ALMGA_ACK    = 0x78  // Arbitration lost in SLA+R/W as Master; General call address has been received; ACK returned  
  #define TWI_SR_OA_DATA_ACK  = 0x80  // Previously addressed with own SLA+W; data has been received; ACK returned
  #define TWI_SR_OA_DATA_NACK = 0x88  // Previously addressed with own SLA+W; data has been received; NOT ACK returned
  #define TWI_SR_GC_DATA_ACK  = 0x90  // Previously addressed with general call; data has been received; ACK returned
  #define TWI_SR_GC_DATA_NACK = 0x98  // Previously addressed with general call; data has been received; NOT ACK returned
  #define TWI_SR_STOP_RSTART  = 0xA0  // A STOP condition or repeated START condition has been received while still addressed as Slave
  // Slave Transmitter Mode
  #define TWI_ST_OA_ACK       = 0xA8  // Own SLA+R has been received; ACK has been returned
  #define TWI_ST_ALMOA_ACK    = 0xB0  // Arbitration lost in SLA+R/W as Master; own SLA+R has been received; ACK has been received
  #define TWI_ST_DATA_ACK     = 0xB8  // Data byte in TWDR has been transmitted; ACK has been received
  #define TWI_ST_DATA_NACK    = 0xC0  // Data byte in TWDR has been transmitted; NOT ACK has been received
  #define TWI_ST_DATA_NACK    = 0xC8  // Last data byte in TWDR has been transmitted (TWEA = '0'); ACK has been received

  /** @enum Type of operation read / write */
  typedef enum {
    eREAD  = 0x0001 | (TWI_MR_SLAR_ACK << 8);  // 0x40 - status code for success
    eWRITE = 0x0000 | (TWI_MT_SLAW_ACK << 8)   // 0x18 - status code for success
  } EOperation;
  
  /** @enum Type of transmission operation */
  typedef enum {
    eSTART_SLAW     = 0x00;  // 0x40 - status code for success
    eSTART_SLAR     = 0x01;  // 0x18 - status code for success
    eREP_START_SLAW = 0x02;
    eREP_START_SLAR = 0x03;
    eDATA_WRITE     = 0x04;
    eDATA_READ      = 0x05
  } EType;
  
  
  /**
   * @desc    TWI init - initialise communication
   *
   * @param   void
   * @return  void
   */
  void TWI_Init();
  
  /**
   * @desc    TWI start
   *
   * @param   unsigned char - status code for success
   * @return  unsigned char 
   */
  unsigned char TWI_Start_condition(unsigned char status_code)

  /**
   * @desc    TWI start - Master Mode
   *
   * @param   unsigned char - slave address
   * @param   EOperation - eWrite -> write
   *                       eRead -> read 
   * @return  unsigned char 
   */
  unsigned char TWI_Start(unsigned char address, EOperation operation);

  /**
   * @desc    TWI Repeated start - Master Transmitter Mode
   *
   * @param   EOperation - eWrite -> write
   *                       eRead -> read 
   * @return  unsigned char 
   */
  unsigned char TWI_Repeated_start(unsigned char address, EOperation operation);

  /**
   * @desc    TWI send one byte - Master Transmitter Mode
   *
   * @param   unsigned char
   * @return  unsigned char 
   */
  unsigned char TWI_MT_send_byte(unsigned char data);

  /**
   * @desc    TWI receive one byte with acknowledgement
   *
   * @param   void
   * @return  unsigned char 
   */
  unsigned char TWI_MT_read_ack(void);

  /**
   * @desc    TWI receive one byte with NOT acknowledgement
   *
   * @param   void
   * @return  unsigned char 
   */
  unsigned char TWI_MT_read_nack(void);
  
  /**
   * @desc    TWI error
   *
   * @param   unsigned char - status code
   * @return  unsigned char
   */
  unsigned char TWI_error(unsigned char status);
  
  /**
   * @desc    TWI stop
   *
   * @param   void
   * @return  void
   */
  void TWI_Stop(void);

#endif
