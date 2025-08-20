// Include file for TM7711 driver library
// Created by Joey232 8/20/25

#ifndef TM7711_H
#define TM7711_H

#include <Arduino.h>

// Next conversion defines
#define TM7711_INPUT_10HZ	1	// Measure differental input at 10Hz rate
#define TM7711_TEMP_40HZ	2	// Measure temperature at 40Hz rate
#define TM7711_INPUT_40HZ	3	// Measure differental input at 40Hz rate

// Max/Min conversion value
#define TM7711_MAX_VALUE 8388607
#define TM7711_MIN_VALUE -8388608

// Conversion error return value
#define TM7711_ERROR 0x1000000

class TM7711
{
public:
	// Constructor
	TM7711(uint8_t data, uint8_t clock);

	// Public functions
	int32_t initialize(int mode=TM7711_INPUT_10HZ);
	int32_t readRaw(int newMode = 0);
	int32_t readAverage(int numReadings, int newMode = 0);
	int32_t convertToMV(int32_t rawValue, int32_t Vref = 5000);

private:
	// Internal data
	int dataPin;
	int clkPin;
	int curMode;
};

#endif

