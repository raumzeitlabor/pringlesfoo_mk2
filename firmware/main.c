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

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <avr/sleep.h>
#include <util/delay.h>

#define LED	PB5
#define MOODY	PB4
#define SPEAKER	PC5
#define BUTTON	PD2

#define DEBOUNCE_TIME	250

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

#define BEEP_DELAY(hertz, ms)	do {					\
	beep((hertz), (ms));						\
	_delay_ms(20);							\
} while (0)

void seed_rng(void);
void beep(uint16_t hertz, int32_t ms);
void delay_us(int16_t us);

void play_random(void);
void play_march(void);
void play_r2d2(void);
void play_ohhh(void);
void play_uhoh(void);

volatile bool button_released = false;

int
main(void)
{
	DDRC |= (1<<SPEAKER);
	DDRB |= ((1<<MOODY) | (1<<LED));
	PORTD |= (1<<BUTTON);

	EICRA |= (1<<ISC01);	/* INT0 IRQ on falling edge */
	EIMSK |= (1<<INT0);

	seed_rng();
	sei();

	for (;;) {
		if (button_released) {
			button_released = false;

			_delay_ms(DEBOUNCE_TIME);
			if (!(PIND & (1<<BUTTON))) {
				PORTB |= (1<<MOODY);
				play_random();
				PORTB &= ~((1<<MOODY));
			}
		}

		set_sleep_mode(SLEEP_MODE_STANDBY);
		sleep_mode();
	}
}

void
seed_rng(void)
{
	ADMUX |= (1<<REFS0);
	ADCSRA |= (1<<ADEN);

	ADCSRA |= (1<<ADSC);
	while (ADCSRA & (1<<ADSC))
		/* no-op */;
	srand(ADCL);

	ADCSRA &= ~(1<<ADEN);
	ADMUX &= ~(1<<REFS0);
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
#ifdef SILENT
		PORTB |= (1<<LED);
#else
		PORTC |= (1<<SPEAKER);
#endif
		delay_us(delay);

#ifdef SILENT
		PORTB &= ~((1<<LED));
#else
		PORTC &= ~((1<<SPEAKER));
#endif
		delay_us(delay);
	}
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
play_random(void)
{
	switch (rand() % 4) {
	case 0:
		play_march();
		break;
	case 1:
		play_r2d2();
		break;
	case 2:
		play_uhoh();
		break;
	case 3:
		play_ohhh();
		break;
	}
}

void
play_march(void)
{
	/*
	 * The tune is taken from https://gist.github.com/tagliati/1804108
	 */

	BEEP_DELAY(a, 500);
	BEEP_DELAY(a, 500);
	BEEP_DELAY(a, 500);
	BEEP_DELAY(f, 350);
	BEEP_DELAY(cH, 150);
	BEEP_DELAY(a, 500);
	BEEP_DELAY(f, 350);
	BEEP_DELAY(cH, 150);
	BEEP_DELAY(a, 1000);
	
	BEEP_DELAY(eH, 500);
	BEEP_DELAY(eH, 500);
	BEEP_DELAY(eH, 500);
	BEEP_DELAY(fH, 350);
	BEEP_DELAY(cH, 150);
	BEEP_DELAY(gS, 500);
	BEEP_DELAY(f, 350);
	BEEP_DELAY(cH, 150);
	BEEP_DELAY(a, 1000);
}

void
play_r2d2(void)
{
	/*
	 * Based on a function, found on
	 * http://www.mycontraption.com/sound-effects-with-and-arduino/
	 */

	BEEP_DELAY(a, 100);
	BEEP_DELAY(g, 100);
	BEEP_DELAY(e, 100);
	BEEP_DELAY(c, 100);
	BEEP_DELAY(d, 100);
	BEEP_DELAY(b, 100);
	BEEP_DELAY(f, 100);
	BEEP_DELAY(cH, 100);
	BEEP_DELAY(a, 100);
	BEEP_DELAY(g, 100);
	BEEP_DELAY(e, 100);
	BEEP_DELAY(c, 100);
	BEEP_DELAY(d, 100);
	BEEP_DELAY(b, 100);
	BEEP_DELAY(f, 100);
	BEEP_DELAY(cH, 100);
}

void
play_ohhh(void)
{
	/*
	 * Based on a function, found on
	 * http://www.mycontraption.com/sound-effects-with-and-arduino/
	 */

	for (int16_t i = 1000; i < 2000; i = i * 1.02) {
		beep(i, 10);
	}

	for (int16_t i = 2000; i > 1000; i = i * 0.98) {
		beep(i, 10);
	}
}

void
play_uhoh(void)
{
	/*
	 * Based on a function, found on
	 * http://www.mycontraption.com/sound-effects-with-and-arduino/
	 */

	for (int16_t i = 1000; i < 1244; i = i * 1.01) {
		beep(i, 30);
	}

	_delay_ms(200);

	for (int16_t i = 1244; i > 1108; i = i * 0.99) {
		beep(i, 30);
	}
}

ISR(INT0_vect)
{
	button_released = true;
}
