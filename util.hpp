/**
 * @file: util.hpp
 *
 * @date: 2021-06-02
 *
 * @author: Lukas Güldenstein 
 */

#ifndef _UTIL_HPP
#define _UTIL_HPP

/* -------------------------------- includes -------------------------------- */

/**
 * flash LED and print message to signal error 
 */
void error(String msg)
{
	pinMode(LED_BUILTIN, OUTPUT);
	Serial.println(msg);
	while (true)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);
		digitalWrite(LED_BUILTIN, LOW);
		delay(100);
	}
}

/**
 * flash LED to signal error 
 */
void error()
{
	pinMode(LED_BUILTIN, OUTPUT);
	while (true)
	{
		digitalWrite(LED_BUILTIN, HIGH);
		delay(100);
		digitalWrite(LED_BUILTIN, LOW);
		delay(100);
	}
}

void setHigherPWMFreq()
{
	TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM 62kHz
}

#endif /* _UTIL_HPP */