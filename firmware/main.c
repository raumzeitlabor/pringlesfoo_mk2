/*-
 * Copyright (C) 2013  Tobias Rehbein
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

#define LED	PB5
#define SPEAKER	PB3
#define BUTTON	PD2

/*
 * The list of frequencies is taken from
 * https://gist.github.com/tagliati/1804108
 */
#define c 261
#define d 294
#define e 329
#define f 349
#define g 391
#define gS 415
#define a 440
#define aS 455
#define b 466
#define cH 523
#define cSH 554
#define dH 587
#define dSH 622
#define eH 659
#define fH 698
#define fSH 740
#define gH 784
#define gSH 830
#define aH 880

void beep(uint16_t hertz, int32_t ms);
void play_march(void);
void delay_us(int16_t us);

int
main(void)
{
	DDRB |= ((1<<LED) | (1<<SPEAKER));
	PORTD |= (1<<BUTTON);

	for (;;) {
		if (PIND & (1<<BUTTON))
			play_march();
	}
}

void
beep(uint16_t hertz, int32_t ms)
{
	/*
	 * This function is based on the beep() function found in
	 * https://gist.github.com/tagliati/1804108
	 */

	int16_t delay = 1000000/hertz;
	int16_t iterations = (ms * 1000) / (delay * 2);

	for (int16_t i=0; i < iterations; i++)
	{
		PORTB |= ((1<<SPEAKER) | (1<<LED));
		delay_us(delay);

		PORTB &= ~((1<<SPEAKER) | (1<<LED));
		delay_us(delay);
	}

	_delay_ms(20);
}

void
delay_us(int16_t us)
{
	/*
	 * This is a stripped down version of the delayMicroseconds() function
	 * found in the Arduino libraries. Comments and code covering
	 * non-16-MHz-clocks has been deleted.
	 */

        if (--us == 0)
                return;

        us <<= 2;
        us -= 2;

        __asm__ __volatile__ (
                "1: sbiw %0,1" "\n\t"
                "brne 1b" : "=w" (us) : "0" (us)
        );
}

void
play_march(void)
{
	/*
	 * The tune is taken from https://gist.github.com/tagliati/1804108
	 */

	beep(a,	500);
	beep(a, 500);
	beep(a, 500);
	beep(f, 350);
	beep(cH, 150);
	beep(a, 500);
	beep(f, 350);
	beep(cH, 150);
	beep(a, 1000);
	
	beep(eH, 500);
	beep(eH, 500);
	beep(eH, 500);
	beep(fH, 350);
	beep(cH, 150);
	beep(gS, 500);
	beep(f, 350);
	beep(cH, 150);
	beep(a, 1000);
}
