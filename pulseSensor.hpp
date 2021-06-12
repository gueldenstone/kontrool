/**
 * @file: pulseSensor.hpp
 *
 * @date: 2021-06-12
 *
 * @author: Yazan Saleh & Lukas Güldenstein 
 */

#ifndef _PULSESENSOR_HPP
#define _PULSESENSOR_HPP

#define FADE_SCALE 10
#define FADE_LEVEL_PER_SAMPLE 12
#define MAX_FADE_LEVEL (255 * FADE_SCALE)
#define DISABLE_PULSE_SENSOR_INTERRUPTS cli()
#define ENABLE_PULSE_SENSOR_INTERRUPTS sei()

class PulseSensor
{
public:
	PulseSensor(const int &inputPin)
	{
		OurThis = this;
		InputPin = inputPin;
		// Initialize (seed) the pulse detector
		sampleIntervalMs = 1;
		resetVariables();
		initInterrupts();
	}

private:
	// Configuration
	int InputPin; // Analog input pin for PulseSensor.

	// Pulse detection output variables.
	// Volatile because our pulse detection code could be called from an Interrupt
	volatile int BPM;										 // int that holds raw Analog in 0. updated every call to readSensor()
	int Signal;													 // holds the latest incoming raw data (0..1023)
	volatile int IBI;										 // int that holds the time interval (ms) between beats! Must be seeded!
	volatile bool Pulse;								 // "True" when User's live heartbeat is detected. "False" when not a "live beat".
	volatile bool QS;										 // The start of beat has been detected and not read by the Sketch.
	volatile int FadeLevel;							 // brightness of the FadePin, in scaled PWM units. See FADE_SCALE
	volatile int threshSetting;					 // used to seed and reset the thresh variable
	volatile int amp;										 // used to hold amplitude of pulse waveform, seeded (sample value)
	volatile unsigned long lastBeatTime; // used to find IBI. Time (sampleCounter) of the previous detected beat start.

	// Variables internal to the pulse detection algorithm.
	// Not volatile because we use them only internally to the pulse detection.
	unsigned long sampleIntervalMs; // expected time between calls to readSensor(), in milliseconds.
	int rate[10];										// array to hold last ten IBI values (ms)
	unsigned long sampleCounter;		// used to determine pulse timing. Milliseconds since we started.
	int P;													// used to find peak in pulse wave, seeded (sample value)
	int T;													// used to find trough in pulse wave, seeded (sample value)
	int thresh;											// used to find instant moment of heart beat, seeded (sample value)
	boolean firstBeat;							// used to seed rate array so we startup with reasonable BPM
	boolean secondBeat;							// used to seed rate array so we startup with reasonable BPM
public:
	bool getPulse()
	{
		return Pulse;
	}
	int getFadeLevel()
	{
		return FadeLevel;
	}
	int getBPM()
	{
		return BPM;
	}
	int getInterval()
	{
		return IBI;
	}

	void setThreshold(int threshold)
	{
		DISABLE_PULSE_SENSOR_INTERRUPTS;
		threshSetting = threshold;
		thresh = threshold;
		ENABLE_PULSE_SENSOR_INTERRUPTS;
	}

	void resetVariables()
	{
		for (int i = 0; i < 10; ++i)
		{
			rate[i] = 0;
		}
		QS = false;
		BPM = 0;
		IBI = 750; // 750ms per beat = 80 Beats Per Minute (BPM)
		Pulse = false;
		sampleCounter = 0;
		lastBeatTime = 0;
		P = 512;						 // peak at 1/2 the input range of 0..1023
		T = 512;						 // trough at 1/2 the input range.
		threshSetting = 550; // used to seed and reset the thresh variable
		thresh = 550;				 // threshold a little above the trough
		amp = 100;					 // beat amplitude 1/10 of input range.
		firstBeat = true;		 // looking for the first beat
		secondBeat = false;	 // not yet looking for the second beat in a row
		FadeLevel = 0;			 // LED is dark.
	}

	int readSample()
	{
		Signal = analogRead(InputPin);
	}

	void processLatestSample()
	{
		// Serial.println(threshSetting);
		// Serial.print('\t');
		// Serial.println(thresh);
		sampleCounter += sampleIntervalMs;		// keep track of the time in mS with this variable
		int N = sampleCounter - lastBeatTime; // monitor the time since the last beat to avoid noise

		// Fade the Fading LED
		FadeLevel = FadeLevel - FADE_LEVEL_PER_SAMPLE;
		FadeLevel = constrain(FadeLevel, 0, MAX_FADE_LEVEL);

		//  find the peak and trough of the pulse wave
		if (Signal < thresh && N > (IBI / 5) * 3)
		{ // avoid dichrotic noise by waiting 3/5 of last IBI
			if (Signal < T)
			{							// T is the trough
				T = Signal; // keep track of lowest point in pulse wave
			}
		}

		if (Signal > thresh && Signal > P)
		{							// thresh condition helps avoid noise
			P = Signal; // P is the peak
		}							// keep track of highest point in pulse wave

		//  NOW IT'S TIME TO LOOK FOR THE HEART BEAT
		// signal surges up in value every time there is a pulse
		if (N > 250)
		{ // avoid high frequency noise
			if ((Signal > thresh) && (Pulse == false) && (N > (IBI / 5) * 3))
			{
				Pulse = true;												// set the Pulse flag when we think there is a pulse
				IBI = sampleCounter - lastBeatTime; // measure time between beats in mS
				lastBeatTime = sampleCounter;				// keep track of time for next pulse

				if (secondBeat)
				{											// if this is the second beat, if secondBeat == TRUE
					secondBeat = false; // clear secondBeat flag
					for (int i = 0; i <= 9; i++)
					{ // seed the running total to get a realisitic BPM at startup
						rate[i] = IBI;
					}
				}

				if (firstBeat)
				{										 // if it's the first time we found a beat, if firstBeat == TRUE
					firstBeat = false; // clear firstBeat flag
					secondBeat = true; // set the second beat flag
					// IBI value is unreliable so discard it
					return;
				}

				// keep a running total of the last 10 IBI values
				word runningTotal = 0; // clear the runningTotal variable

				for (int i = 0; i <= 8; i++)
				{													 // shift data in the rate array
					rate[i] = rate[i + 1];	 // and drop the oldest IBI value
					runningTotal += rate[i]; // add up the 9 oldest IBI values
				}

				rate[9] = IBI;							// add the latest IBI to the rate array
				runningTotal += rate[9];		// add the latest IBI to runningTotal
				runningTotal /= 10;					// average the last 10 IBI values
				BPM = 60000 / runningTotal; // how many beats can fit into a minute? that's BPM!
				QS = true;									// set Quantified Self flag (we detected a beat)
				FadeLevel = MAX_FADE_LEVEL; // If we're fading, re-light that LED.
			}
		}

		if (Signal < thresh && Pulse == true)
		{												// when the values are going down, the beat is over
			Pulse = false;				// reset the Pulse flag so we can do it again
			amp = P - T;					// get amplitude of the pulse wave
			thresh = amp / 2 + T; // set thresh at 50% of the amplitude
			P = thresh;						// reset these for next time
			T = thresh;
		}

		if (N > 2500)
		{																// if 2.5 seconds go by without a beat
			thresh = threshSetting;				// set thresh default
			P = 512;											// set P default
			T = 512;											// set T default
			lastBeatTime = sampleCounter; // bring the lastBeatTime up to date
			firstBeat = true;							// set these to avoid noise
			secondBeat = false;						// when we get the heartbeat back
			QS = false;
			BPM = 0;
			IBI = 600; // 600ms per beat = 100 Beats Per Minute (BPM)
			Pulse = false;
			amp = 100; // beat amplitude 1/10 of input range.
		}
	}

	void onSampleTime()
	{
		digitalWrite(2, HIGH);
		readSample();
		processLatestSample();
		digitalWrite(2, LOW);
	}
	static PulseSensor *OurThis;

private:
	void initInterrupts()
	{
		// Initializes Timer2 to throw an interrupt every 2mS.
		// Interferes with PWM on pins 9 and 10
		TCCR2A = 0x00;	// Disable PWM and go into CTC mode
		TCCR2B = 0x0C;	// prescaler 256
		OCR2A = 0x007C; // count to 124 for 2mS interrupt
		TIMSK2 = 0x02;	// Enable OCR1A match interrupt	DISABLE BY SETTING TO 0x00
		ENABLE_PULSE_SENSOR_INTERRUPTS;
	}
};

#endif /* _PULSESENSOR_HPP */
