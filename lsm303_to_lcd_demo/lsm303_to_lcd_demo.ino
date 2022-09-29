// Displaying accelerometer values on the LCD display

// Includes.
#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include <Wire.h>


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);

// Initializing the LiquidCrystal library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

float Ax;
float Ax_smooth;
float Ay;
float Ay_smooth;
float Az;
float Az_smooth;

void setup() {
  // put your setup code here, to run once:
  // set up the LCD's number of columns and rows:
  // Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.clear()

  // Initialize sensor, set range and mode.
  ;accel.begin();
  accel.setRange(LSM303_RANGE_4G);
  accel.setMode(LSM303_MODE_NORMAL);


}


void loop() {
  // put your main code here, to run repeatedly:
  // Create sample of accelerometer values and retrieve that sample.
  sensors_event_t event;
  accel.getEvent(&event);

  // Smoothing out accel values with EWMA
  Ax = event.acceleration.x;
  Ax_smooth = 0.85 * Ax_smooth + 0.15 * Ax;

  Ay = event.acceleration.y;
  Ay_smooth = 0.85 * Ay_smooth + 0.15 * Ay;

  Az = event.acceleration.z;
  Az_smooth = 0.85 * Az_smooth + 0.15 * Az;

  /* Display the results (acceleration is measured in m/s^2) */
  lcd.setCursor(0, 0); // Move cursor to 1st column, 1st row
  lcd.print("X: "); // 
  lcd.print(Ax_smooth); // Prints current value immediately after previous print call
  // Serial.print("Ax smoothed: = ");
  // Serial.println(Ax);
  lcd.setCursor(8, 0); 
  lcd.print("Y: ");
  lcd.print(Ay_smooth);
  // Serial.print("Ay smoothed: = ");
  // Serial.println(Ay);  
  lcd.setCursor(0, 1);
  lcd.print("Z: ");
  lcd.print(Az_smooth);
  // Serial.print("Az smoothed: = ");
  // Serial.println(Az);  

  /* Delay before the next sample */
  delay(500);
}
