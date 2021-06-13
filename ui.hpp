/**
 * @file: ui.hpp
 *
 * @date: 2021-06-02
 *
 * @author: Lukas Güldenstein 
 */

#ifndef _UI_HPP
#define _UI_HPP

/* -------------------------------- includes -------------------------------- */
#include <Arduino.h>
#include <Wire.h>							// the I2C library
#include <Adafruit_GFX.h>			// the graphics library
#include <Adafruit_SSD1306.h> // the SSD1306 library

/**
 * use these pins:
 * A4 -> SDA
 * A5 -> SCL
 */

class UI
{
private:
	const int oledAdrr = 0x3c;
	const int oledWidth = 128;
	const int oledHeight = 64;

	const int barX0 = 105;
	const int barY0 = 20;
	const int barWidth = 10;
	const int barHeight = 40;

	Adafruit_SSD1306 oled;

public:
	UI()
	{
		oled = Adafruit_SSD1306(oledWidth, oledHeight, &Wire, -1);
	}
	~UI() {}

private:
	void updateCurrentBPM(const int &currentBPM)
	{
		// write current BPM as value
		oled.fillRect(0, 42, 70, 20, BLACK); // clear part of display
		oled.setTextColor(WHITE);
		oled.setTextSize(2);
		oled.setCursor(5, 45);
		oled.print(currentBPM);
	}
	void updateTargetBPM(const int &targetBPM)
	{
		// write target BPM as value
		oled.fillRect(0, 10, 70, 20, BLACK); // clear part of display
		oled.setTextColor(WHITE);
		oled.setTextSize(2);
		oled.setCursor(5, 12);
		oled.print(targetBPM);
	}
	void updateBar(const int &muscleSensor)
	{
		// clear the bar
		oled.fillRect(barX0 + 1, barY0 + 1, barWidth - 1, barHeight - 1, BLACK);
		// fill the bar
		int curHeight = map(muscleSensor, 0, 1023, 0, barHeight);
		oled.fillRect(barX0, barY0 + (barHeight - curHeight), barWidth, curHeight, WHITE);
	}

public:
	void drawTemplate()
	{
		oled.setTextColor(WHITE);
		oled.setTextSize(1);

		// target value
		oled.setCursor(0, 0);
		oled.print("target BPM");

		// current value
		oled.setCursor(0, 33);
		oled.print("current BPM");

		// UI for Muscle Sensor
		oled.setCursor(90, 0);
		oled.print("Muscle");

		oled.drawLine(barX0, barY0, barX0, barY0 + barHeight, WHITE);												 // left
		oled.drawLine(barX0 + barWidth, barY0, barX0 + barWidth, barY0 + barHeight, WHITE);	 // right
		oled.drawLine(barX0, barY0, barX0 + barWidth, barY0, WHITE);												 // top
		oled.drawLine(barX0, barY0 + barHeight, barX0 + barWidth, barY0 + barHeight, WHITE); //bottom
	}

public:
	bool start()
	{
		Wire.setClock(400000);
		Wire.begin();
		if (!oled.begin(SSD1306_SWITCHCAPVCC, oledAdrr))
		{
			return false;
		}
		return true;
	}
	void displayLogo()
	{
		oled.clearDisplay();
		oled.setTextColor(WHITE);
		oled.fillScreen(BLACK);
		oled.setTextSize(2);
		oled.setCursor(oledWidth / 2 - 50, oledHeight / 2 - 10);
		oled.print("KONTROOL");
		oled.display();
	}
	void clearDisplay()
	{
		oled.clearDisplay();
	}
	void update(const int &muscleSensor, const int &targetBPM, const int &currentBPM)
	{
		static int lastTargetBPM;

		updateBar(muscleSensor);
		updateCurrentBPM(currentBPM);

		// check if target BPM has changed
		if (targetBPM != lastTargetBPM)
		{
			updateTargetBPM(targetBPM);
			lastTargetBPM = targetBPM;
		}

		oled.display();
	}
	void dim(const bool &dim)
	{
		oled.dim(dim);
	}
};

#endif /* _UI_HPP */