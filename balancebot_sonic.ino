/* ultrasonic_balance

This uses the ultrasonic sensor to determine the balance of the bot.
If the distance is decreasing, roll forward.
If the distance is increasing, roll backward.

*/

#include <Wire.h>
#include <Adafruit_MotorShield.h>

const int LEFTMOTOR = 1;
const int RIGHTMOTOR = 2;
const int LED = 13;
const int USONICSIGNAL = 8; // Ultrasound signal pin

unsigned long echo = 0;
unsigned long ultrasoundValue = 0;
unsigned long baseline = 0;

Adafruit_MotorShield AFMS = Adafruit_MotorShield();
Adafruit_DCMotor *leftMotor = AFMS.getMotor(LEFTMOTOR);
Adafruit_DCMotor *rightMotor = AFMS.getMotor(RIGHTMOTOR);

unsigned long pingCM() {
  // Returns distance in CM
  
  pinMode(USONICSIGNAL, OUTPUT); // Switch signalpin to output
  digitalWrite(USONICSIGNAL, LOW); // Send low pulse 
  delayMicroseconds(2); // Wait for 2 microseconds
  digitalWrite(USONICSIGNAL, HIGH); // Send high pulse
  delayMicroseconds(5); // Wait for 5 microseconds
  digitalWrite(USONICSIGNAL, LOW); // Holdoff
  pinMode(USONICSIGNAL, INPUT); // Switch signalpin to input
  digitalWrite(USONICSIGNAL, HIGH); // Turn on pullup resistor
  // please note that pulseIn has a 1sec timeout, which may
  // not be desirable. Depending on your sensor specs, you
  // can likely bound the time like this -- marcmerlin
  // echo = pulseIn(USONICSIGNAL, HIGH, 38000)
  echo = pulseIn(USONICSIGNAL, HIGH); //Listen for echo
  ultrasoundValue = (echo / 58.138); //convert to CM
  return ultrasoundValue;
}

void setup() {
  // Setup ultrasonic sensor
  Serial.begin(9600);
  pinMode(USONICSIGNAL,OUTPUT);

  // Setup motors
  pinMode(LED, OUTPUT);
  AFMS.begin();
  leftMotor->setSpeed(255);
  rightMotor->setSpeed(255);
  
  // Calibrate distance sensor
  digitalWrite(LED, HIGH);
  delay(1000);
  baseline = pingCM();
  digitalWrite(LED, LOW);
}

void loop() {
  int cm0 = 0;
  cm0 = pingCM() - baseline;  // negative cm means falling forward
  Serial.println(cm0);
  if (cm0 > 0) {
    leftMotor->run(FORWARD);
    rightMotor->run(FORWARD);
  }
  if (cm0==0) {
    leftMotor->run(RELEASE);
    rightMotor->run(RELEASE);
  }

  if (cm0 < 0) {
    rightMotor->run(BACKWARD);
    leftMotor->run(BACKWARD);
  }
  delay(125); //delay 1/4 seconds.
}

