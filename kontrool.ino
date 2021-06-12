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
#include "pulseSensor.hpp"

// debuggin?
#define DEBUG false

/* -------------------------- parameter definitons -------------------------- */
const unsigned long baudRate = 115200;

/* ----------------------------- pin definitions ---------------------------- */

const uint8_t muscleSensor = A0;
const uint8_t cvOut = 5;
const uint8_t pulsePin = A1;
const uint8_t motorPin = 3;

PulseSensor *PulseSensor::OurThis;

/* ---------------- global variables & object instantiations ---------------- */
UI ui = UI();
PulseSensor pulseSensor = PulseSensor(pulsePin);
void setup()
{
	Serial.begin(baudRate);
	if (!ui.start())
	{
		error("could not setup oled");
	}
	ui.displayLogo();
	// delay(2000);
	pinMode(cvOut, OUTPUT);
	pinMode(motorPin, OUTPUT);
	TCCR0B = TCCR0B & B11111000 | B00000001; // for PWM 62kHz
	pulseSensor.setThreshold(480);
}

volatile uint16_t muscleSenseVal = 0;
volatile uint8_t cvOutVal = 0;

// loop
void loop()
{
	ui.update(muscleSenseVal, 80, pulseSensor.getBPM());
	if (pulseSensor.getPulse())
	{
		digitalWrite(motorPin, HIGH);
		analogWrite(cvOut, cvOutVal);
#if DEBUG
		Serial.print("BPM: ");
		Serial.println(pulseSensor.getBPM());
#endif
	}
	else
	{
		digitalWrite(motorPin, LOW);
	}
	delay(20);
}

// get data and process pulse samples
ISR(TIMER2_COMPA_vect)
{
	DISABLE_PULSE_SENSOR_INTERRUPTS; // disable interrupts while we do this
	muscleSenseVal = analogRead(muscleSensor);
	cvOutVal = map(muscleSenseVal, 0, 1023, 0, 255);
	PulseSensor::OurThis->onSampleTime();
	ENABLE_PULSE_SENSOR_INTERRUPTS; // enable interrupts when you're done
}