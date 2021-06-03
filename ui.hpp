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
		oled.setTextSize(2);
		oled.setCursor(oledWidth / 2 - 50, oledHeight / 2 - 10);
		oled.print("KONTROOL");
		oled.display();
	}
	void update(int muscleSensor, int targetBPM, int currentBPM)
	{
		oled.clearDisplay();
		drawTemplate();
		// fill the bar
		int curHeight = map(muscleSensor, 0, 1023, 0, barHeight);
		oled.fillRect(barX0, barY0 + (barHeight - curHeight), barWidth, curHeight, WHITE);

		oled.setTextColor(WHITE);
		oled.setTextSize(2);
		// write target BPM
		oled.setCursor(5, 12);
		oled.print(targetBPM);

		// write current BPM
		oled.setCursor(5, 45);
		oled.print(currentBPM);

		oled.display();
	}
};

#endif /* _UI_HPP */