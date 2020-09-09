/**
 * \file
 *
 * \brief ILI9341 Display Controller Component Driver
 *
 * Copyright (c) 2012-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */
#ifndef ILI9341_H_INCLUDED
#define ILI9341_H_INCLUDED

#include <ioport.h>
#include <compiler.h>
#include "spi_class.h"

/* Controller and interface configuration file */
#include "conf_ili9341.h"

#define CONF_ILI9341_CLOCK_SPEED		8000000UL
#define CONF_ILI9341_SPI			SPI0
#define CONF_ILI9341_CS_PIN			PIO_PA28_IDX
#define CONF_ILI9341_DC_PIN			PIO_PB3_IDX
#define CONF_ILI9341_BACKLIGHT_PIN	PIO_PA21_IDX
#define CONF_ILI9341_RESET_PIN		PIO_PA29_IDX

/** This macro generates a 16-bit native color for the display from a
 *  24-bit RGB value.
 */
#define ILI9341_COLOR(r, g, b)\
	Swap16((((uint16_t)b) >> 3) |\
	       ((((uint16_t)g) << 3) & 0x07E0) |\
	       ((((uint16_t)r) << 8) & 0xf800))

/** Type define for an integer type large enough to store a pixel color. */
typedef uint16_t ili9341_color_t;

/** Type define for an integer type large enough to store a pixel coordinate.
 */
typedef int16_t ili9341_coord_t;

/**
 * \name Display orientation flags
 * @{
 */

/** Bit mask for flipping X for ili9341_set_orientation() */
#define ILI9341_FLIP_X 1
/** Bit mask for flipping Y for ili9341_set_orientation() */
#define ILI9341_FLIP_Y 2
/** Bit mask for swapping X and Y for ili9341_set_orientation() */
#define ILI9341_SWITCH_XY 4

/** @} */

/** Height of display using default orientation */
#define ILI9341_DEFAULT_HEIGHT   240

/** Width of display using default orientation */
#define ILI9341_DEFAULT_WIDTH    320

/** Height of display using swapped X/Y orientation */
#define ILI9341_SWITCH_XY_HEIGHT 320

/** Width of display using swapped X/Y orientation */
#define ILI9341_SWITCH_XY_WIDTH  240

/**
 * \name Controller primitive graphical functions
 * @{
 */
class Touch
{
public:
		Touch(){
			hwspi spi_Touch(1, uint8_t 0x02);
		}
	
}

class ili9341_Display 
{
public:
	
		hwspi spi_Display(0,  uint8_t 0x02);		// chipselect = 0, pin 10, PA28
		ili9341_color_t ili9341_read_gram(void);
		static ili9341_coord_t limit_start_x, limit_start_y;
		static ili9341_coord_t limit_end_x, limit_end_y;

		static void ili9341_send_draw_limits(const bool send_end_limits);
		
		void ili9341_write_gram(ili9341_color_t color);

		void ili9341_set_top_left_limit(ili9341_coord_t x, ili9341_coord_t y);

		void ili9341_set_bottom_right_limit(ili9341_coord_t x, ili9341_coord_t y);

		void ili9341_set_limits(ili9341_coord_t start_x, ili9341_coord_t start_y,
		ili9341_coord_t end_x, ili9341_coord_t end_y);

		void ili9341_set_orientation(uint8_t flags);

		void ili9341_copy_pixels_to_screen(const ili9341_color_t *pixels,
		uint32_t count);

		void ili9341_copy_pixels_from_screen(ili9341_color_t *pixels, uint32_t count);

		void ili9341_duplicate_pixel(const ili9341_color_t color, uint32_t count);

		inline void backlight_on(void);

		inline void backlight_off(void);


}


#endif /* ILI9341_H_INCLUDED */
