# TWI / I2C Scanner
Example looks up for device address connected on I2C bus. Found device is printed on LCD 1.8 display.
## Limitation
Display depicts first found device either more devices connected on the same bus. Scanner can find only one device address.
## Tested
Program was tested with Atmega16A, 1.8" LCD connected through SPI and 0.96" OLED connected through I2C.
## Prerequisite
- [Library st7735](https://github.com/Matiasus/ST7735)
