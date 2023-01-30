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
* File Name    : (oled.h)
* Version      : (v1.0)
* Device(s)    : (SAMC21)
* OS           : (none)
* H/W Platform : (OLED1-XPRO)
* Description  : (Declaration header file for OLED1 display)
***********************************************************************************************************************/



#ifndef OLED_H
#define OLED_H

#include <oled/ssd1306.h>

#define OLED_WIDTH              128
#define OLED_HEIGHT             32
#define OLED_PIXELS_PER_BYTE    8
#define OLED_PAGES              (OLED_HEIGHT / OLED_PIXELS_PER_BYTE)

void oled_init(void);
void oled_clear(void);
void oled_char(char c, uint8_t x, uint8_t y);
void oled_pixel(char px[], uint8_t x);
void oled_string(char *str, uint8_t x, uint8_t y);

#endif
