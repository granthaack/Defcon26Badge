/*
  Super Simple Neopixel Driver based on BigJosh's code
  #defines are specifically for our badge
  More info at http://wp.josh.com/2014/05/11/ws2812-neopixels-made-easy/
  Modified by Grant Haack, July 22nd 2018
  Released under GPL v.3
    Join us now and share the Software
    You'll be free, Hackers, you'll be free
*/
#ifndef SIMPIX_H_
#define SIMPIX_H_
#include "Arduino.h"

#define PIXELS 50  // Number of pixels in the string
#define BRIGHTNESS 42 //The max brightness we want
#define PIXEL_PORT  PORTD  // Port of the pin the pixels are connected to
#define PIXEL_DDR   DDRD   // Port of the pin the pixels are connected to
#define PIXEL_BIT   6      // Bit of the pin the pixels are connected to
#define T1H  900    // Width of a 1 bit in ns
#define T1L  600    // Width of a 1 bit in ns
#define T0H  400    // Width of a 0 bit in ns
#define T0L  900    // Width of a 0 bit in ns
#define RES 6000    // Width of the low gap between bits to cause a frame to latch
// Here are some convience defines for using nanoseconds specs to generate actual CPU delays
#define NS_PER_SEC (1000000000L)          // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives
#define CYCLES_PER_SEC (F_CPU)
#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )
#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )

//The function that does all the actual heavy lifting and sends the bits
//to the neopixel array
inline void send_bit(bool bit_val);

//Send a byte to the neopixel array
inline void send_byte(uint8_t s_bit);

//Set up the neopixels
void led_setup();

//Send a the data for a single pixel
void send_pixel(uint8_t r, uint8_t g, uint8_t b);

//Latch the pixels and display the frame
void show_pixels();

#endif
