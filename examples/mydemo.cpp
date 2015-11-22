#include "ArduiPi_SSD1306.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

Adafruit_SSD1306 display;

// Adafruit SPI Display		Pi P1 Header Pin	Raspberry Pi Pin
// ---------------------------------------------------------------------
// GND				6			0V
// VIN				2			5V
// DATA				19			MOSI (SPI)
// CLK				23			SCLK (SPI)
// D/C				18			IO24 (GPIO)
// RST(Reset)			22			IO25 (GPIO)
// CS (Chip Select)		24			CE0 (SPI)

#define PIN_DC		RPI_V2_GPIO_P1_18 /* Data/Command Pin Hでデータ、Lでコマンド */
#define PIN_RESET 	RPI_V2_GPIO_P1_22 /* Lでリセット */
#define PIN_CS		BCM2835_SPI_CS1   /* Default Chip Select */

int main()
{
	int i, j;
	int black = 1;
	display.init(PIN_DC, PIN_RESET, PIN_CS, OLED_ADAFRUIT_SPI_128x32);
	display.begin();

	/* 左上 */
	display.drawPixel(0, 0, WHITE);
	display.display();
	sleep(2);

	/* 左下 */
	display.drawPixel(0, 31, WHITE);
	display.display();
	sleep(2);

	/* 右上 */	
	display.drawPixel(127, 0, WHITE);
	display.display();
	sleep(2);

	/* 右下 */
        display.drawPixel(127, 31, WHITE);
        display.display();
        sleep(2);

	for (i = 0; i < 32; i++) {
		for (j = 0; j < 128; j++) {
			//display.drawPixel(j, i, (black % 2) ? BLACK : WHITE);
			display.drawPixel(j, i, WHITE);
			display.display();
			black++;
		}
		//black = i;
		//display.display();
	}

	display.clearDisplay();
	display.close();
	return 0;
}

