/** 
 * -------------------------------------------------------------+ 
 * @desc        Simple scanner I2C
 * -------------------------------------------------------------+ 
 *              Copyright (C) 2020 Marian Hrinko.
 *              Written by Marian Hrinko (mato.hrinko@gmail.com)
 *
 * @author      Marian Hrinko
 * @datum       06.09.2020
 * @file        main.c
 * @tested      AVR Atmega16 
 * -------------------------------------------------------------+ 
 */
 
// include libraries
#include "lib/st7735.h"
#include "lib/twi.h"

/**
 * @desc    Main
 *
 * @param   void
 *
 * @return  unsigned char
 */
int main(void)
{
  unsigned char found;
  char msg[20];

  // DISPLAY ST7735
  // -------------------------------------------------   
  St7735Init();
  // clear screen
  ClearScreen(BLACK);

  // Init TWI
  // -------------------------------------------------------
  TWI_Init();

  // set position x, y
  SetPosition(25, 5);
  // draw string
  DrawString("TWI / I2C SCANNER", WHITE, X1);
  // find device address
  found = TWI_MT_FindDevice();
  // set position x, y
  SetPosition(18, 20);
  // draw string
  DrawString("Device found at [", RED, X1);
  // to string
  sprintf(msg, "%x", found);
  // draw string
  DrawString(msg, WHITE, X1);
  // draw string
  DrawString("]", RED, X1);
  // update screen
  UpdateScreen();

  // return value
  return 0;
}
