/***********************************************************************************************************************
* Copyright (c) 2022 Microchip Corporation. All rights reserved.
*
* Redistribution and use in source and binary forms, with or without modification, are permitted provided that the
* following conditions are met:
*
* 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following
*    disclaimer.
*
* 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the
*    following disclaimer in the documentation and/or other materials provided with the distribution.
*
* 3. The name of Microchip may not be used to endorse or promote products derived from this software without specific
*    prior written permission.
*
* 4. This software may only be redistributed and used in connection with an Microchip microcontroller product.
*
* THIS SOFTWARE IS PROVIDED BY Microchip "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE EXPRESSLY AND
* SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY,
* OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
* DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
* STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
* EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : (oled.c)
* Version      : (v1.0)
* Device(s)    : (SAMC21)
* OS           : (none)
* H/W Platform : (OLED1-XPRO)
* Description  : (Low Level drivers for the OLED1 display)
***********************************************************************************************************************/


#include "oled.h"
#include "sysfont.h"
#include "ssd1306.h"

/**
 * \brief Initialize SSD1306 controller and LCD display.
 * It will also write the graphic controller RAM to all zeroes.
 *
 * \note This function will clear the contents of the display.
 */
void oled_init(void)
{
    OLED_CS_Set();
	/* Initialize the low-level display controller. */
	ssd1306_init();
	/* Clear the contents of the display. */
	oled_clear();
}

/**
 * \brief Clear the LCD display by writing the RAM to all zeroes.
 *
 * \note This function will clear the contents of the display.
 */
void oled_clear(void)
{
	uint8_t page;
	uint8_t column;

	/* Set display to output data from line 0 */
	ssd1306_set_display_start_line_address(0);

	/* Clear the contents of the display.
	 * If using a framebuffer (SPI interface) it will both clear the
	 * controller memory and the framebuffer.
	 */
	for (page = 0; page < OLED_PAGES; page++) {
		for (column = 0; column < OLED_WIDTH; column++) {
			ssd1306_put_byte(page, column, 0x00);
		}
	}
}

/**
 * \internal
 * \brief Helper function that draws a character from a font to the display
 *
 * This function will first calculate the start offset in the font character
 * data before iterating over the specific character data.
 *
 * Only pixels in the character that should be enabled are done so, the caller
 * is required to prepare the drawing area before printing a character to it.
 * This is done by the gfx_mono_draw_string() and
 * gfx_mono_draw_progmem_string() functions.
 *
 * \param[in] ch       Character to be drawn
 * \param[in] x        X coordinate on screen.
 * \param[in] y        Y coordinate on screen.
 */
void oled_char(char ch, uint8_t x, uint8_t y)
{
	uint8_t *glyph_data;
	uint16_t glyph_data_offset;
	uint8_t col_left;
	int8_t i;

	uint8_t inc_col = x;						// display column (0...127)
	uint8_t inc_page = y/OLED_PIXELS_PER_BYTE;	// display page (0...3)
	uint8_t glyph_byte = 0;
	uint8_t col_mask = 0x80;

	glyph_data_offset = sysfont.height * ((uint8_t)ch - sysfont.first_char);
	col_left = sysfont.width;

	// Font data have to be aligned from 1 byte per row to 1 byte per column
	// Display is setup as 128 columns with 32 rows.
	// 8 rows build 1 page, i.e. a total of 4 pages are available
	col_mask = 0x80;
	do {
		// read font data per column
		glyph_byte = 0x00;
		glyph_data = (uint8_t *) sysfont.data + glyph_data_offset;
		for(i=0; i<sysfont.height; i++)
		{
			if(((*(glyph_data++)) & col_mask) != 0)
			{
				glyph_byte |= 0x80;
			}
			glyph_byte >>= 1;
		}
		// write column to display
		ssd1306_put_byte(inc_page, inc_col, glyph_byte);
		inc_col += 1;
		col_mask >>= 1;
		col_left--;
	} while (col_left > 0);
	// write empty column to display
//	ssd1306_put_byte(inc_page, inc_col, 0x00);
}


/**
 * \internal
 * \brief Helper function that draws pixel's in a display column
 *
 * \param[in] px[3]    4 bytes for one column with 32 pixels
 * \param[in] x        X coordinate on screen.
 */
void oled_pixel(char px[], uint8_t x)
{
	ssd1306_put_byte(0, x, px[0]);
	ssd1306_put_byte(1, x, px[1]);
	ssd1306_put_byte(2, x, px[2]);
	ssd1306_put_byte(3, x, px[3]);
}


/**
 * \brief Draws a string to the display
 *
 * This function will draw a string located in memory to the display.
 *
 * \param[in] str       Pointer to string
 * \param[in] x         X coordinate on screen.
 * \param[in] y         Y coordinate on screen.
 */
void oled_string(char *str, uint8_t x, uint8_t y)
{
	/* Save X in order to know where to return to on CR. */
	const uint8_t start_of_string_position_x = x;

	/* Draw characters until trailing null byte */
	do {
		/* Handle '\n' as newline, draw normal characters. */
		if (*str == '\n') {
			x = start_of_string_position_x;
			y += sysfont.height + 1;
		} else if (*str == '\r') {
			/* Skip '\r' characters. */
		} else {
			oled_char(*str, x, y);
			if(*str != ' ')
			{
				x += sysfont.width;
			}
			else
			{
				x += (sysfont.width - 2);
			}
		}
	} while (*(++str));
}

