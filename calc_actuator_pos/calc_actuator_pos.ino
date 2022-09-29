// the code for getting the feedback when the actuator fully extended and retracted

#define DIR_2 4 //  DIR2 pin MDD10A
#define PWM_2 6 //  PWM2 pin MDD10A
#define POTENTIOMETER   A1 // potentiometer of the actuator

void setup() {
  // initialize serial monitor
  Serial.begin(9600);
  // initialize inputs and outputs.
  pinMode(DIR_2, OUTPUT);
  pinMode(PWM_2, OUTPUT);
  pinMode(POTENTIOMETER,INPUT);

}

void loop() {
  // extend the actuator
  digitalWrite(DIR_2, HIGH);
  analogWrite(PWM_2, 255);
  delay(30000); // giving the actuator time to reach max
  int POTENTIOMETER_MAX = analogRead(POTENTIOMETER);
  Serial.print("POTENTIOMETER_MAX = ");
  Serial.println(POTENTIOMETER_MAX);

  // retracts the actuator
  digitalWrite(DIR_2, LOW);
  analogWrite(PWM_2, 255);
  delay(30000); // giving the actuator time to reach min
  int POTENTIOMETER_MIN = analogRead(POTENTIOMETER);
  Serial.print("POTENTIOMETER_MIN = ");
  Serial.println(POTENTIOMETER_MIN);
}
