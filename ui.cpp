/**
 * @file: ui.cpp
 *
 * @date: 2021-06-02
 *
 * @author: Lukas Güldenstein 
 *
 * @brief: functions to control oled for kontrool
 */

/* -------------------------------- includes -------------------------------- */
#include "ui.hpp"

bool UI::start()
{
	Wire.begin();
	if (!oled.begin(SSD1306_SWITCHCAPVCC, oledAdrr))
	{
		return false;
	}
	return true;
}

void UI::displayLogo()
{
	oled.clearDisplay();
	oled.setTextColor(WHITE);
	oled.setTextSize(2);
	oled.setCursor(oledWidth / 2 - 50, oledHeight / 2 - 10);
	oled.print("KONTROOL");
	oled.display();
}

void UI::drawTemplate()
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

void UI::update(int muscleSensor, int targetBPM, int currentBPM)
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
