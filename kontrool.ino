/**
 * @file: kontrool.ino
 *
 * @date: 2021-06-02
 *
 * @author: Yazan Saleh & Lukas Güldenstein
 *
 * @brief: kontrool - controlling music with your body
 */

/* -------------------------------- includes -------------------------------- */
#include "ui.hpp"
#include "util.hpp"
// #include <avr/iomxxhva.h>

/* -------------------------- parameter definitons -------------------------- */
const unsigned long baudRate = 115200;

/* ----------------------------- pin definitions ---------------------------- */

const uint8_t muscleSensor = A0;
const uint8_t cvOut = 5;

/* ---------------- global variables & object instantiations ---------------- */
UI ui = UI();
void setup()
{
	Serial.begin(baudRate);
	if (!ui.start())
	{
		error("could not setup oled");
	}
	ui.displayLogo();
	delay(2000);
	pinMode(cvOut, OUTPUT);
	TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM
}

void loop()
{
	static uint16_t muscleSenseVal = 0;
	static uint8_t cvOutVal = 0;
	static long pTime = 0;
	static long now;
	now = millis();

	// do somthing every 20ms
	if (now - pTime > 20)
	{
		muscleSenseVal = analogRead(muscleSensor);
		cvOutVal = map(muscleSenseVal, 0, 1023, 0, 255);

		pTime = now;
	}
	analogWrite(cvOut, cvOutVal);
	ui.update(muscleSenseVal, 55, 60);
}