#include <ir_Lego_PF_BitStreamEncoder.h>
#include <boarddefs.h>
#include <IRremoteInt.h>
#include <IRremote.h>

const int ledPin1 = 13; // red led
const int ledPin2 = 12; // blue led
const int ledPin3 = 11; // green led

const int trigPin = 10; // ultrasonic trig
const int echoPin = 9; // ultrasonic echo
int ultraState = 0; // ultrasonic state
int u_oldState = 0; // ultrasonic old state
int u_ledState = 0; // ultrasonic led state
long duration;
int distance;

const int buttonPin = 8; // button
int buttonState = 0; // button value
int b_oldState = 0; // old value
int b_ledState = 0; // led state

const int irPin = 7; // ir sensor
IRrecv irrecv(irPin);
decode_results results; // decode results
int irState = 0; // ir sensor state
int ir_oldState = 0; //ir sensor old state
int ir_ledState = 0; // ir led state

const int potenPin = A0; // potentiometer
int potenVal = 0; // potentiometer value
int ledVal = 0; // led value

void setup() {
  // put your setup code here, to run once:
  pinMode(ledPin1, OUTPUT);
  pinMode(ledPin2, OUTPUT);
  pinMode(ledPin3, OUTPUT);

  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);

  pinMode(buttonPin, INPUT);

  pinMode(irPin, INPUT);
  irrecv.enableIRIn();

  pinMode(potenPin, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  // potentiometer
  potenVal = analogRead(potenPin);

  ledVal = map(potenVal, 0, 1023, 0, 255);
  
  // ultrasonic sensor
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  duration = pulseIn(echoPin, HIGH);
  distance = duration * 0.034 / 2;

  if (distance <= 10) {
    ultraState = 1; 
  } else {
    ultraState = 0;
  }

  if ((ultraState == 1) && (u_oldState == 0)) {
    u_ledState = 1 - u_ledState;
    delay(10);
  }

  u_oldState = ultraState;

  if (u_ledState == 1) {
    analogWrite(ledPin1, ledVal);
  } else {
    digitalWrite(ledPin1, LOW);
  }

  // button
  buttonState = digitalRead(buttonPin);
  
  if ((buttonState == 1) && (b_oldState == 0)) {
    b_ledState = 1 - b_ledState;
    delay(10);
  }

  b_oldState = buttonState;

  if (b_ledState == 1) {
    analogWrite(ledPin2, ledVal);
  } else {
    digitalWrite(ledPin2, LOW);
  }

  // ir sensor
  if (irrecv.decode(&results)) {
    irState = 1;
    irrecv.resume();
  } else {
    irState = 0;
  }

  if ((irState == 1) && (ir_oldState == 0)) {
    ir_ledState = 1 - ir_ledState;
    delay(10);
  }
  
  ir_oldState = irState;

  if (ir_ledState == 1) {
    analogWrite(ledPin3, ledVal);
  } else {
    digitalWrite(ledPin3, LOW);
  }


  delay(60);

  
}
