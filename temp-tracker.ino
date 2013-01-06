// (c) Copyright Alex Dean 2012
// Released under Apache License, version 2.0
//
// Adapted from:
// * http://learn.adafruit.com/tmp36-temperature-sensor/using-a-temp-sensor
// * http://exosite.com/project/basic-arduino-temperature-web-monitor

#include <Ethernet.h>
// #include <SnowPlow.h>

/*
 * Arduino configuration
 */

// MAC address of this Arduino
const byte kMac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED }; // TODO: update this when my Arduino arrives

// The analog pin to which the TMP36's Vout (sense) pin is connected
const int kSensorPin = 0;

// The Arduino voltage (either 5.0 or a more accurate 3.3)
const float kArduinoV = 5.0;

/*
 * Tracking configuration
 */

// Frequency of taking temperature readings, in seconds
const int kReadingFreq = 15;

// SnowPlow app name
const char kSnowplowAppName[] = "alex-flat";

// SnowPlow CloudFront collector subdomain
const char kSnowplowCfSubdomain[] = "TODO";

// SnowPlow user ID (to identify this specific Arduino)
const char kSnowplowUserId[] = "living-room";

// SnowPlow Tracker
// SnowPlowTracker snowplow(&Ethernet, kMac, kSnowplowAppName);

/*
 * setup() runs once when you turn your
 * Arduino on: use it to initialize and
 * set any initial values.
 *
 * We initialize the serial connection
 * with the computer, the voltage to use
 * with ARef, and the SnowPlow Arduino
 * tracker.
 */
void setup()
{
  // Serial connection lets us debug on the computer
  Serial.begin(9600);

  // Required if setting the ARef to something other than 5v
  analogReference(kArduinoV);

  // Setup SnowPlow Arduino tracker
  /*
  snowplow.initCf(kSnowplowCfSubdomain);
  snowplow.setUserId(kSnowplowUserId); // If not set, defaults to Mac address.
  */
}
 
/*
 * loop() runs over and over again.
 * An empty loop() takes just a few
 * clock cycles to complete.
 *
 * We use loop() to measure the
 * temperature every minute and log
 * it to SnowPlow.
 */
void loop()
{
  // When did we run last? 
  static unsigned long prevTime = 0;

  if (millis() - prevTime >= (kReadingFreq * 1000))
  {
    // Get the temperature
    float tempC = readTempInC(kSensorPin, kArduinoV);
    
    // Debug
    Serial.print(tempC); Serial.println(" degrees C");

    // Track via SnowPlow
    /*
    snowplow.trackEvent("readings", "temp", tempC);
    */

    prevTime = millis();
    delay(500); // Running loop twice a sec is fine
  }
}

/*
 * Gets the temperature in Celsius from
 * a TMP36 temperature sensor.
 *
 * The resolution is 10 mV / degree
 * centigrade with a 500 mV offset
 * to allow for negative temperatures.
 *
 * sensorPin is the analog pin which the
 * TMP36's Vout (sense) pin is connected
 * to.
 *
 * arduinoVoltage is the voltage the
 * Arduino runs on.
 */
float readTempInC(int sensorPin, float arduinoVoltage)
{
  // Get the voltage reading from the TMP36
  int reading = analogRead(sensorPin);

  // Convert that reading to voltage
  float voltage = (reading * arduinoVoltage) / 1024;

  // Convert from 10 mv per degree with 500 mV offset
  // to degrees: (voltage - 500mV) times 100
  return (float)((voltage - 0.5) * 100);
}