/*********************************************************************
This is an example for our Monochrome OLEDs based on SSD1306 drivers

  Pick one up today in the adafruit shop!
  ------> http://www.adafruit.com/category/63_98

This example is for a 128x32|64 size display using SPI or I2C to communicate
4 or 5 pins are required to interface

Adafruit invests time and resources providing this open source code, 
please support Adafruit and open-source hardware by purchasing 
products from Adafruit!

Written by Limor Fried/Ladyada  for Adafruit Industries.  
BSD license, check license.txt for more information
All text above, and the splash screen must be included in any redistribution

02/18/2013 	Charles-Henri Hallard (http://hallard.me)
						Modified for compiling and use on Raspberry ArduiPi Board
						LCD size and connection are now passed as arguments on 
						the command line (no more #define on compilation needed)
						ArduiPi project documentation http://hallard.me/arduipi

						
*********************************************************************/

#include "ArduiPi_SSD1306.h"
#include "Adafruit_GFX.h"
#include "Adafruit_SSD1306.h"

#include <getopt.h>

#define PRG_NAME        "ssd1306_demo"
#define PRG_VERSION     "1.1"

// Instantiate the display
Adafruit_SSD1306 display;

// Config Option
struct s_opts
{
	int oled;
	int verbose;
};

// default options values
s_opts opts = {
	OLED_ADAFRUIT_SPI_128x32,	// Default oled
	false				// Not verbose
};

#define NUMFLAKES 10
#define XPOS 0
#define YPOS 1
#define DELTAY 2

#define LOGO16_GLCD_HEIGHT 16
#define LOGO16_GLCD_WIDTH  16

static unsigned char logo16_glcd_bmp[] =
{
	0b00000000, 0b11000000,
	0b00000001, 0b11000000,
	0b00000001, 0b11000000,
	0b00000011, 0b11100000,
	0b11110011, 0b11100000,
	0b11111110, 0b11111000,
	0b01111110, 0b11111111,
	0b00110011, 0b10011111,
	0b00011111, 0b11111100,
	0b00001101, 0b01110000,
	0b00011011, 0b10100000,
	0b00111111, 0b11100000,
	0b00111111, 0b11110000,
	0b01111100, 0b11110000,
	0b01110000, 0b01110000,
	0b00000000, 0b00110000
};


void testdrawbitmap(const uint8_t *bitmap, uint8_t w, uint8_t h)
{
	uint8_t icons[NUMFLAKES][3];
	srandom(666);     // whatever seed
 
	// initialize
	for (uint8_t f = 0; f < NUMFLAKES; f++) {
		icons[f][XPOS] = random() % display.width();
		icons[f][YPOS] = 0;
		icons[f][DELTAY] = random() % 5 + 1;
    
		printf("x: %d", icons[f][XPOS]);
		printf("y: %d", icons[f][YPOS]);
		printf("dy: %d\n", icons[f][DELTAY]);
	}

	for (int32_t i = 0; i < 100; i++) {
//	while (1) {
		// draw each icon
		for (uint8_t f = 0; f < NUMFLAKES; f++) {
			display.drawBitmap(icons[f][XPOS], icons[f][YPOS], logo16_glcd_bmp, w, h, WHITE);
		}
		display.display();
		usleep(100000);
    
		// then erase it + move it
		for (uint8_t f = 0; f < NUMFLAKES; f++) {
			display.drawBitmap(icons[f][XPOS], icons[f][YPOS],  logo16_glcd_bmp, w, h, BLACK);
			// move it
			icons[f][YPOS] += icons[f][DELTAY];
			// if its gone, reinit
			if (icons[f][YPOS] > display.height()) {
				icons[f][XPOS] = random() % display.width();
				icons[f][YPOS] = 0;
				icons[f][DELTAY] = random() % 5 + 1;
			}
		}
	}
}


void testdrawchar(void)
{
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);

	for (uint8_t i = 0; i < 168; i++) {
		if (i == '\n')
			continue;
		display.write(i);
		if ((i > 0) && (i % 21 == 0))
			display.print("\n");
	}    
	display.display();
}

void testdrawcircle(void)
{
	for (int16_t i = 0; i < display.height(); i += 2) {
		display.drawCircle(display.width()/2, display.height()/2, i, WHITE);
		display.display();
	}
}

void testfillrect(void)
{
	uint8_t color = 1;

	for (int16_t i = 0; i < display.height() / 2; i += 3) {
		// alternate colors
		display.fillRect(i, i, display.width()-i*2, display.height()-i*2, color%2);
		display.display();
		color++;
	}
}

void testdrawtriangle(void)
{
	for (int16_t i = 0; i < min(display.width(), display.height()) / 2; i += 5) {
		display.drawTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
		display.display();
	}
}

void testfilltriangle(void)
{
	uint8_t color = WHITE;

	for (int16_t i = min(display.width(), display.height()) / 2; i > 0; i-=5) {
		display.fillTriangle(display.width()/2, display.height()/2-i,
                     display.width()/2-i, display.height()/2+i,
                     display.width()/2+i, display.height()/2+i, WHITE);
		if (color == WHITE)
			color = BLACK;
		else
			color = WHITE;
		display.display();
	}
}

void testdrawroundrect(void)
{
	for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
		display.drawRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, WHITE);
		display.display();
	}
}

void testfillroundrect(void)
{
	uint8_t color = WHITE;

	for (int16_t i = 0; i < display.height() / 2 - 2; i += 2) {
		display.fillRoundRect(i, i, display.width()-2*i, display.height()-2*i, display.height()/4, color);
		if (color == WHITE)
			color = BLACK;
		else color = WHITE;
			display.display();
	}
}
   
void testdrawrect(void)
{
	for (int16_t i = 0; i < display.height() / 2; i += 2) {
		display.drawRect(i, i, display.width()-2*i, display.height()-2*i, WHITE);
		display.display();
	}
}

void testdrawline()
{  
	for (int16_t i = 0; i < display.width(); i += 4) {
		display.drawLine(0, 0, i, display.height()-1, WHITE);
		display.display();
	}
	for (int16_t i = 0; i < display.height(); i += 4) {
		display.drawLine(0, 0, display.width()-1, i, WHITE);
		display.display();
	}
	usleep(250000);
  
	display.clearDisplay();
	for (int16_t i = 0; i < display.width(); i += 4) {
		display.drawLine(0, display.height()-1, i, 0, WHITE);
		display.display();
	}
	for (int16_t i = display.height() - 1; i >= 0; i -= 4) {
		display.drawLine(0, display.height()-1, display.width()-1, i, WHITE);
		display.display();
	}
	usleep(250000);
  
	display.clearDisplay();
	for (int16_t i=display.width()-1; i>=0; i-=4) {
		display.drawLine(display.width()-1, display.height()-1, i, 0, WHITE);
		display.display();
	}
	for (int16_t i=display.height()-1; i>=0; i-=4) {
		display.drawLine(display.width()-1, display.height()-1, 0, i, WHITE);
		display.display();
	}
	usleep(250000);

	display.clearDisplay();
	for (int16_t i=0; i<display.height(); i+=4) {
		display.drawLine(display.width()-1, 0, 0, i, WHITE);
		display.display();
	}
	for (int16_t i=0; i<display.width(); i+=4) {
		display.drawLine(display.width()-1, 0, i, display.height()-1, WHITE); 
		display.display();
	}
	usleep(250000);
}

void testscrolltext(void)
{
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.setCursor(10,0);
	display.clearDisplay();
	display.print("scroll");
	display.display();
 
	display.startscrollright(0x00, 0x0F);
	sleep(2);
	display.stopscroll();
	sleep(1);
	display.startscrollleft(0x00, 0x0F);
	sleep(2);
	display.stopscroll();
	sleep(1);    
	display.startscrolldiagright(0x00, 0x07);
	sleep(2);
	display.startscrolldiagleft(0x00, 0x07);
	sleep(2);
	display.stopscroll();
}


/* ======================================================================
Function: usage
Purpose : display usage
Input 	: program name
Output	: -
Comments: 
====================================================================== */
void usage( char * name)
{
	printf("%s\n", name );
	printf("Usage is: %s --oled type [options]\n", name);
	printf("  --<o>led type\nOLED type are:\n");
	for (int i=0; i<OLED_LAST_OLED;i++)
		printf("  %1d %s\n", i, oled_type_str[i]);
	
	printf("Options are:\n");
	printf("  --<v>erbose  : speak more to user\n");
	printf("  --<h>elp\n");
	printf("<?> indicates the equivalent short option.\n");
	printf("Short options are prefixed by \"-\" instead of by \"--\".\n");
	printf("Example :\n");
	printf( "%s -o 1 use a %s OLED\n\n", name, oled_type_str[1]);
	printf( "%s -o 4 -v use a %s OLED being verbose\n", name, oled_type_str[4]);
}


/* ======================================================================
Function: parse_args
Purpose : parse argument passed to the program
Input 	: -
Output	: -
Comments: 
====================================================================== */
void parse_args(int argc, char *argv[])
{
	static struct option longOptions[] =
	{
		{"oled"	  , required_argument,0, 'o'},
		{"verbose", no_argument,	  	0, 'v'},
		{"help"		, no_argument, 			0, 'h'},
		{0, 0, 0, 0}
	};

	int optionIndex = 0;
	int c;

	while (1) 
	{
		/* no default error messages printed. */
		opterr = 0;

		c = getopt_long(argc, argv, "vho:", longOptions, &optionIndex);

		if (c < 0)
			break;

		switch (c) 
		{
			case 'v': opts.verbose = true;
				break;

			case 'o':
				opts.oled = (int) atoi(optarg);
				
				if (opts.oled < 0 || opts.oled >= OLED_LAST_OLED )
				{
					fprintf(stderr, "--oled %d ignored must be 0 to %d.\n", opts.oled, OLED_LAST_OLED-1);
					fprintf(stderr, "--oled set to 0 now\n");
					opts.oled = 0;
				}
				break;

			case 'h':
				usage(argv[0]);
				exit(EXIT_SUCCESS);
				break;
			
			case '?':
			default:
				fprintf(stderr, "Unrecognized option.\n");
				fprintf(stderr, "Run with '--help'.\n");
				exit(EXIT_FAILURE);
		}
	} /* while */

	if (opts.verbose)
	{
		printf("%s v%s\n", PRG_NAME, PRG_VERSION);
		printf("-- OLED params -- \n");
		printf("Oled is    : %s\n", oled_type_str[opts.oled]);
		printf("-- Other Stuff -- \n");
		printf("verbose is : %s\n", opts.verbose? "yes" : "no");
		printf("\n");
	}	
}


/* ======================================================================
Function: main
Purpose : Main entry Point
Input 	: -
Output	: -
Comments: 
====================================================================== */
int main(int argc, char **argv)
{
	int i;
	
	// Oled supported display in ArduiPi_SSD1306.h
	// Get OLED type
	parse_args(argc, argv);

	// SPI
	if (display.oled_is_spi_proto(opts.oled) != true) {
		printf("%s(%d) oled_is_spi_proto() ERROR!\n", __func__, __LINE__);
		exit(EXIT_FAILURE);
	}

	// SPI change parameters to fit to your LCD
	if (display.init(OLED_SPI_DC,OLED_SPI_RESET,OLED_SPI_CS, opts.oled) != true) {
		printf("%s(%d) init() ERROR!\n", __func__, __LINE__);
		exit(EXIT_FAILURE);
	}
printf("%s:%s(%d) display.begin()\n", __FILE__, __func__, __LINE__);
	display.begin();
printf("%s:%s(%d) display.clearDisplay()\n", __FILE__, __func__, __LINE__);	
	// init done
	display.clearDisplay();   // clears the screen and buffer

	// draw a single pixel
printf("%s:%s(%d) draw a single pixel\n", __FILE__, __func__, __LINE__);
	display.drawPixel(10, 10, WHITE);
	display.display();
	sleep(2);
	display.clearDisplay();

	// draw many lines
printf("%s:%s(%d) draw many lines\n", __FILE__, __func__, __LINE__);	
	testdrawline();
	display.display();
	sleep(2);
	display.clearDisplay();

	// draw rectangles
printf("%s:%s(%d) draw rectangles\n", __FILE__, __func__, __LINE__);
	testdrawrect();
	display.display();
	sleep(2);
	display.clearDisplay();

	// draw multiple rectangles
printf("%s:%s(%d) draw multiple rectangles\n", __FILE__, __func__, __LINE__);
	testfillrect();
	display.display();
	sleep(2);
	display.clearDisplay();

	// draw mulitple circles
printf("%s:%s(%d) draw multiple circles\n", __FILE__, __func__, __LINE__);
	testdrawcircle();
	display.display();
	sleep(2);
	display.clearDisplay();

	// draw a white circle, 10 pixel radius
printf("%s:%s(%d) draw a white circle, 10 pixel radius\n", __FILE__, __func__, __LINE__);
	display.fillCircle(display.width()/2, display.height()/2, 10, WHITE);
	display.display();
	sleep(2);
	display.clearDisplay();

printf("%s:%s(%d) draw round rect\n", __FILE__, __func__, __LINE__);
	testdrawroundrect();
	sleep(2);
	display.clearDisplay();

printf("%s:%s(%d) fill round rect\n", __FILE__, __func__, __LINE__);
	testfillroundrect();
	sleep(2);
	display.clearDisplay();

printf("%s:%s(%d) draw triangle\n", __FILE__, __func__, __LINE__);
	testdrawtriangle();
	sleep(2);
	display.clearDisplay();

printf("%s:%s(%d) fill triangle\n", __FILE__, __func__, __LINE__); 
	testfilltriangle();
	sleep(2);
	display.clearDisplay();

	// draw the first ~12 characters in the font
printf("%s:%s(%d) draw the first 12 characters in the font \n", __FILE__, __func__, __LINE__);
	testdrawchar();
	display.display();
	sleep(2);
	display.clearDisplay();

	// text display tests
printf("%s:%s(%d) text display\n", __FILE__, __func__, __LINE__);
	display.setTextSize(1);
	display.setTextColor(WHITE);
	display.setCursor(0,0);
	display.print("Hello, world!\n");
	display.setTextColor(BLACK, WHITE); // 'inverted' text
	display.printf("%f\n", 3.141592);
	display.setTextSize(2);
	display.setTextColor(WHITE);
	display.printf("0x%8X\n", 0xDEADBEEF);
	display.display();
	sleep(2);
 
	// horizontal bargraph tests
printf("%s:%s(%d) horizontal bargraph\n", __FILE__, __func__, __LINE__);
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	for ( i =0 ; i<=100 ; i++)
	{
		display.clearDisplay();
		display.setCursor(0,0);
		display.print("Gauge Graph!\n");
		display.printf("  %03d %%", i);
		display.drawHorizontalBargraph(0,16,128,16,1, i);
		display.display();
		usleep(25000);
	}

	// vertical bargraph tests
printf("%s:%s(%d) vertical bargraph\n", __FILE__, __func__, __LINE__);	
	display.clearDisplay();
	display.setTextSize(1);
	display.setTextColor(WHITE);
	for ( i =0 ; i<=100 ; i++)
	{
		display.clearDisplay();
		display.setCursor(0,0);
		display.print("Gauge !\n");
		display.printf("%03d %%", i);
		display.drawVerticalBargraph(114,0,8,32,1, i);
		display.display();
		usleep(25000);
	}

	// draw scrolling text
printf("%s:%s(%d) draw scrolling text\n", __FILE__, __func__, __LINE__);
	testscrolltext();
	sleep(2);
	display.clearDisplay();

	// miniature bitmap display
printf("%s:%s(%d) miniature bitmap\n", __FILE__, __func__, __LINE__);
	display.clearDisplay();
	display.drawBitmap(30, 16,  logo16_glcd_bmp, 16, 16, 1);
	display.display();

	// invert the display
printf("%s:%s(%d) invert the display\n", __FILE__, __func__, __LINE__);
	display.invertDisplay(true);
	sleep(1); 
	display.invertDisplay(false);
	sleep(1); 

	// draw a bitmap icon and 'animate' movement
printf("%s:%s(%d) draw a bitmap icon and 'animate' movement\n", __FILE__, __func__, __LINE__);
	testdrawbitmap(logo16_glcd_bmp, LOGO16_GLCD_HEIGHT, LOGO16_GLCD_WIDTH);
	display.clearDisplay();
	sleep(1);
	// Free PI GPIO ports
	display.close();
}


