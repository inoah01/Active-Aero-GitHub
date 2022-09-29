// code for getting the actuator position in milimeters

#define DIR_2 4 // DIR2 pin MDD10A
#define PWM_2 6 // PWM2 pin MDD10
#define POTENTIOMETER   A1 // potentiometer pin

// Defining some constants to be used later. Measured in mm
#define MAX_STROKE_LENGTH  305 
#define POTENTIOMETER_MAX  539  
#define POTENTIOMETER_MIN  46  


void setup() {
  Serial.begin(9600);
  // Initializing inputs and outputs
  pinMode(DIR_2, OUTPUT);
  pinMode(PWM_2, OUTPUT);
  pinMode(POTENTIOMETER,INPUT);
}

void loop() {
  // extend the actuator
  digitalWrite(DIR_2, HIGH);
  analogWrite(PWM_2, 255);

  int potentiometer_value = analogRead(POTENTIOMETER); // reading potentiometer value
  int actuator_position = map(potentiometer_value, POTENTIOMETER_MIN, POTENTIOMETER_MAX, 0, MAX_STROKE_LENGTH); // mapping value to a scale of mm
  Serial.print("The actuator's position is = ");
  Serial.print(actuator_position);
  Serial.println(" mm");

  // Stop actuator
  if (actuator_position >= STROKE_LENGTH) {
    digitalWrite(DIR_2, HIGH);
    analogWrite(PWM_2, 0);
  }
}
