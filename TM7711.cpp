// Source code for TM7711 driver library
// Created by Joey232 8/20/25

#include "TM7711.h"

// Class constructor
TM7711::TM7711(uint8_t data, uint8_t clock)
{
	dataPin = data;
	clkPin = clock;
	curMode = 0;
}

// Initialize the TM7711
// Set the supplied mode, or default to input 10Hz
int32_t TM7711::initialize(int mode)
{
	curMode = mode;  // Save starting mode
	pinMode(dataPin, INPUT);   // Data pin is an input, no pullup

	pinMode(clkPin, OUTPUT); // Clock is an output
	digitalWrite(clkPin, LOW); // Start clock low

	// Force a "restart" (Actually entrance/exit from power down)
	delay(1); // Wait a bit
	digitalWrite(clkPin, LOW); // Make high for a bit to ensure chip is reset
	delay(1); // Wait a bit
	return(readRaw());
}

// Read raw data from the TM7711
// Returne a 32 bit signed number
int32_t TM7711::readRaw(int newMode)
{
	unsigned long start = millis();
	int32_t value = 0;
	int cnt;

	// Wait until TM7711 is ready
	while (digitalRead(dataPin) != LOW)  // Data goes low when ready
		// Check for a timeout (> 500ms)
		if ((millis() - start) > 500)
			return (TM7711_ERROR);

	// Read 24 bits of data
	for (cnt = 0; cnt < 24; ++cnt)
	{
		digitalWrite(clkPin, HIGH); // Pulse the clock high
		delayMicroseconds(1);
		value = (value << 1) | digitalRead(dataPin); // Shift in data bit
		digitalWrite(clkPin, LOW); // Pulse the clock low
		delayMicroseconds(1);
	}

	// Sign-extend the value for two's complement if MSB is set
	if (value & 0x800000)
	{
		value |= 0xFF000000; // Sign-extend to 32 bits
	}

	// Send 25-27 clock pulses total to set the next read mode
	if (!newMode)
		newMode = curMode;
	for (cnt = 0; cnt < newMode; ++cnt)
	{
		digitalWrite(clkPin, HIGH);
		delayMicroseconds(1);
		digitalWrite(clkPin, LOW);
		delayMicroseconds(1);
	}

	return(value);
}

// Read multiple raw values and return the average
// Sets a new conversion mode after the last reading
int32_t TM7711::readAverage(int numReadings, int newMode)
{
	int32_t total = 0;

	for (int i = 1; i < numReadings; i++)
		total += readRaw();
	total += readRaw(newMode);

	return (total / numReadings);
}

// Convert raw value to voltage in mv
// Takes a raw reading and the ref voltage in mv
int32_t TM7711::convertToMV(int32_t rawValue, int32_t Vref)
{
	return ((rawValue * Vref) / TM7711_MAX_VALUE); // Scale raw value to voltage in mv
}
