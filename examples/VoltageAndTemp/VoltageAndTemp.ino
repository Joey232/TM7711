// Simple example file for the TM7711 driver
// Reads the raw A/D input, converts it to mv, and then reads the temp input
// Created by Joey232 8/20/25

#include "TM7711.h"  // Library include file

// Pin definitions (GPIO)
#define DATA_PIN  13  // DOUT (data output) from TM7711
#define CLOCK_PIN 12  // PD_SCK (clock input) to TM7711

// Create TM7711 object
TM7711 tm7711(DATA_PIN, CLOCK_PIN);

void setup()
{
	Serial.begin(115200);

	tm7711.initialize(); // Initialize the TM7711 using default mode
}

void loop()
{
	int32_t current, voltage, temp;

	// Get the currrent raw A/D reading
	current = tm7711.readRaw(TM7711_TEMP_40HZ);  // Switch to temp input for next reading
	Serial.print("Raw: ");
	Serial.print(current);
	Serial.print("   mv: ");
	
	// Convert to mv, using a 5V referance voltage
	voltage = tm7711.convertToMV(current,5000);
	Serial.print(voltage);

	// Get the currrent raw temp reading
	temp = tm7711.readRaw(TM7711_INPUT_10HZ);  // Switch to A/D input for next reading
	Serial.print("   Temp: ");
	Serial.println(temp);

	return;
}
