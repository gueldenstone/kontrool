
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
const uint8_t cvOut = 10;
const uint8_t pulsePin = A1;
const uint8_t motorPin = 3;

PulseSensor *PulseSensor::OurThis;

/* ---------------- global variables & object instantiations ---------------- */
UI ui = UI();
PulseSensor pulseSensor = PulseSensor(pulsePin);
const int numReadings = 31;
int readings[numReadings];

/* ---------------------------------- setup --------------------------------- */
void setup()
{
	Serial.begin(baudRate);
	if (!ui.start())
	{
		error("could not setup oled");
	}
	ui.displayLogo();
	delay(2000);
	ui.clearDisplay();
	ui.drawTemplate();

	pinMode(motorPin, OUTPUT);

	// configure CV Pin with higher PWM freq
	setHigherPWMFreq();

	// threshhold to avoid noise
	pulseSensor.setThreshold(480);

	// zero readings array
	for (int i = 0; i < numReadings; i++)
	{
		readings[i] = 0;
	}
}

/* ------------------------- variables in interrupt ------------------------- */
volatile uint16_t muscleSenseVal = 0;
volatile uint8_t cvOutVal = 0;

/* ------------------------ global variables in loop ------------------------ */
unsigned long pTime = 0;
unsigned long pTime2 = 0;
unsigned long interval;
uint8_t average = 0;
int sum;
uint8_t count = 0;
int smooth;
int total = 0;
int readIndex = 0;
bool motorOn = false;

/* ---------------------------------- loop ---------------------------------- */
void loop()
{
	unsigned long now = millis();

	// smoothing
	total -= readings[readIndex];
	int bpm = pulseSensor.getBPM();
	if (bpm < smooth + 5 || bpm > smooth - 5)
	{
		readings[readIndex] = bpm;
	}
	total += readings[readIndex];
	readIndex += 1;

	if (readIndex >= numReadings)
	{
		readIndex = 0;
	}

	// calculate smooth
	smooth = total / numReadings;

	/* -------------------------------- averaging ------------------------------- */
	if (now - pTime2 >= 500)
	{
		pTime2 = now;
		sum += smooth;
		count++;
		if (count >= 20)
		{
			average = sum / 20;
			sum = 0;
			count = 0;
		}
	}

	// interval for the vibration motor output
	interval = 30000 / smooth;

	//
	if ((now - pTime) >= interval)
	{
		pTime = now;
		motorOn ^= true;
		if (motorOn)
		{
			digitalWrite(motorPin, HIGH);
		}
		else
		{
			digitalWrite(motorPin, LOW);
		}
	}

	// read pulse sensor value and map for display
	int val = analogRead(A1);
	val = map(val, 0, 1023, 0, 25);
	// update ui
	ui.update(muscleSenseVal, val, average);
	analogWrite(cvOut, cvOutVal);
}

/* ------------------------ interrupt service routine ----------------------- */
// get data and process pulse samples
ISR(TIMER2_COMPA_vect)
{
	DISABLE_PULSE_SENSOR_INTERRUPTS; // disable interrupts while we do this
	PulseSensor::OurThis->onSampleTime();
	muscleSenseVal = analogRead(muscleSensor);
	cvOutVal = map(muscleSenseVal, 0, 1023, 0, 255);
	ENABLE_PULSE_SENSOR_INTERRUPTS; // enable interrupts when you're done
}