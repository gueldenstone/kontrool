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
	void drawTemplate();

public:
	bool start();
	void displayLogo();
	void update(int, int, int);
	void updateOledBar(int);
};

#endif /* _UI_HPP */