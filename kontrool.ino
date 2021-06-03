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

/* -------------------------- parameter definitons -------------------------- */
const long baudRate = 115200;

/* ----------------------------- pin definitions ---------------------------- */

const int muscleSensor = A0;
const int cvOutMuscleSensor = 3;

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
}

void loop()
{
	static int counter = 0;
	static int pTime = 0;
	static int now;
	now = millis();

	// do somthing every 100ms
	if (now - pTime > 500)
	{
		counter += 20;
		counter = counter & 0x3ff;
		pTime = now;
	}
	ui.update(counter, 55, 60);
	delay(100);
}