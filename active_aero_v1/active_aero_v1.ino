#include <Adafruit_LSM303_Accel.h>
#include <Adafruit_Sensor.h>
#include <LiquidCrystal.h>
#include <Wire.h>

// defining pin names:
#define DIR_2 4 // the Arduino pin connected to the DIR2 pin MDD10
#define PWM_2 6 // the Arduino pin connected to the PWM2 pin MDD10
#define POTENTIOMETER_PIN   A1 // the Arduino pin connected to the potentiometer of the actuator


/* Assign a unique ID to this sensor at the same time */
Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);


// Initializing the LiquidCrystal library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 7, en = 8, d4 = 9, d5 = 10, d6 = 11, d7 = 12;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);


// Variables for targets/conditions:
int DRS = 140; // Drag Reduction System
int DF = 200; // Downforce
int AB = 260; // Aerobrake
int LAT = 4;
int ACCEL = 2;
int DECCEL = -4;
int deadband = 12; // MoE so actuator is not bouncing between values
int target_pos_H;
int target_pos_L;
int counter;

// Potentiometer constants
int POTENTIOMETER_MIN = 46;
int POTENTIOMETER_MAX = 539;
int STROKE_LENGTH = 305;

// Variables for WMA (smoothing) of accel:
int alpha = .96;
float Ax;
float Ax_smooth;
float Ay;
float Ay_smooth;


void setup() {
  // put your setup code here, to run once:

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.clear();

  // Setting up pins:
  pinMode(DIR_2, OUTPUT);
  pinMode(PWM_2, OUTPUT);
  pinMode(POTENTIOMETER_PIN, INPUT_PULLUP);

  // Initialize sensor, set range and mode.
  ;accel.begin();
  accel.setRange(LSM303_RANGE_4G);
  accel.setMode(LSM303_MODE_NORMAL);
}

void loop() {
  // put your main code here, to run repeatedly:
  sensors_event_t event;
  accel.getEvent(&event);
  

  // Reading potentiometer values and mapping them to mm scale
  int potentiometer_value = analogRead(POTENTIOMETER_PIN);
  int stroke_pos = map(potentiometer_value, POTENTIOMETER_MIN, POTENTIOMETER_MAX, 0, STROKE_LENGTH);


  // Smoothing out accel values with low-pass
  Ax = event.acceleration.x;
  Ax_smooth = (Ax_smooth * alpha + Ax * 1-alpha);

  Ay = event.acceleration.y;
  Ay_smooth = (Ay_smooth * alpha + Ay * 1-alpha);

  /* Display the results (acceleration is measured in m/s^2) */
  lcd.setCursor(0, 0); // Move cursor to 1st column, 1st row
  lcd.print("X: "); // 
  lcd.print(Ax_smooth); // Prints current value immediately after previous print call

  lcd.setCursor(0, 1); 
  lcd.print("Y: ");
  lcd.print(Ay_smooth);

  // Move to 8th column of row 1 (2) and print "MODE" (to show current mode)
  lcd.setCursor(11, 0);
  lcd.print("MODE:");

  // Establish target position
  if (abs(Ay_smooth) >= LAT) {
    target_pos_H = DF + deadband;
    target_pos_L = DF - deadband;
    lcd.setCursor(13, 1);
    lcd.print(" ");
    lcd.setCursor(14, 1);
    lcd.print("DF");
    counter = 0;
  } else if (Ax_smooth <= DECCEL && abs(Ay_smooth) <= LAT) {
    target_pos_H = AB + deadband;
    target_pos_L = AB - deadband;
    lcd.setCursor(13, 1);
    lcd.print(" ");
    lcd.setCursor(14, 1);
    lcd.print("AB");
    counter = 0;
  } else if (Ax_smooth >= ACCEL && abs(Ay_smooth) <= LAT) {
    target_pos_H = DRS + deadband;
    target_pos_L = DRS - deadband;
    lcd.setCursor(13, 1);
    lcd.print("DRS");
    counter = 0;
  } else if (counter > 40) {
    target_pos_H = DRS + deadband;
    target_pos_L = DRS - deadband;
    lcd.print("DRS");    
  } else {
    counter++;
  }


  // Extend/Retract to target position or stay put based on current pos
  if (stroke_pos > target_pos_H) { // Retract
    digitalWrite(DIR_2, LOW);
    analogWrite(PWM_2, 255);
  } else if (stroke_pos < target_pos_L) { // Extend
    digitalWrite(DIR_2, HIGH);
    analogWrite(PWM_2, 255);
  } else if (stroke_pos > target_pos_L - (deadband/2) && stroke_pos < target_pos_H - (deadband/2)) { // stay @ position
    digitalWrite(DIR_2, LOW);
    analogWrite(PWM_2, 0);
  }

  
  // Delay before next accel sample
  delay(50);
}
