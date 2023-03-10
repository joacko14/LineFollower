#include <Arduino.h>
#define ledPin 13
#define enA 3   // Left gear motor speed (pwm)
#define in1A 2  // Left gear motor
#define in2A 4  // Left gear motor
#define enB 6   // Right gear motor speed (pwm)
#define in3B 5  // Right gear motor
#define in4B 7  // Right gear motor
#define irel 8  // Infrared external left sensor
#define iril 9  // Infrared internal left sensor
#define irir 10 // Infrared internal right sensor
#define irer 11 // Infrared external right sensor

bool S1;
bool S2;
bool S3;
bool S4;
bool start = false;

int a = 0;

// motor control
void motor(char action, int Speed, float CorrectionLeft, float CorrectionRight)
{
  // Motor A - Izquierdo
  // Motor B - derecho
  if (action == 'F')
  { // Forward
    // Left motor
    analogWrite(enA, Speed * CorrectionLeft);
    digitalWrite(in1A, LOW);
    digitalWrite(in2A, HIGH); // Forward
    // Right motor
    analogWrite(enB, Speed * CorrectionRight);
    digitalWrite(in3B, HIGH); // Forward
    digitalWrite(in4B, LOW);
  }
  else if (action == 'B')
  { // Backward
    // Left motor
    analogWrite(enA, Speed * CorrectionLeft);
    digitalWrite(in1A, HIGH); // Backward
    digitalWrite(in2A, LOW);
    // Right motor
    analogWrite(enB, Speed * CorrectionRight);
    digitalWrite(in3B, LOW);
    digitalWrite(in4B, HIGH); // Backward
  }
  else if (action == 'L')
  { // Turn left
    // Left motor
    analogWrite(enA, Speed * CorrectionLeft);
    digitalWrite(in1A, HIGH); // Backward
    digitalWrite(in2A, LOW);
    // Right motor
    analogWrite(enB, Speed * CorrectionRight);
    digitalWrite(in3B, HIGH); // Forward
    digitalWrite(in4B, LOW);
  }
  else if (action == 'R')
  { // Turn right
    // Left motor
    analogWrite(enA, Speed * CorrectionLeft);
    digitalWrite(in1A, LOW);
    digitalWrite(in2A, HIGH); // Forward
    // Right motor
    analogWrite(enB, Speed * CorrectionRight);
    digitalWrite(in3B, LOW);
    digitalWrite(in4B, HIGH); // Backward
  }
  else if (action == 'S')
  { // stop
    analogWrite(enA, 0);
    digitalWrite(in1A, LOW);
    digitalWrite(in2A, LOW);

    analogWrite(enB, 0);
    digitalWrite(in3B, LOW);
    digitalWrite(in4B, LOW);
  }
}

void test()
{
  motor('F', 255, 0.85, 1);
  delay(2000);
  motor('B', 255, 1, 1);
  delay(2000);
  motor('R', 255, 1, 1);
  delay(2000);
  motor('L', 255, 1, 1);
  delay(2000);
  motor('S', 0, 0, 0);
}

void setup()
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);

  pinMode(enA, OUTPUT);
  pinMode(in1A, OUTPUT);
  pinMode(in2A, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in3B, OUTPUT);
  pinMode(in4B, OUTPUT);

  pinMode(irel, INPUT);
  pinMode(iril, INPUT);
  pinMode(irir, INPUT);
  pinMode(irer, INPUT);

  test();
  Serial.println("Setup OK");
}

void loop()
{
  // put your main code here, to run repeatedly:

  S1 = digitalRead(irer); // sensor one
  S2 = digitalRead(irir); // sensor one
  S3 = digitalRead(iril); // sensor one
  S4 = digitalRead(irel); // sensor one

  // delay(200);
  Serial.print(S1);
  Serial.print(" - ");
  Serial.print(S2);
  Serial.print(" - ");
  Serial.print(S3);
  Serial.print(" - ");
  Serial.println(S4);

  if (S1 && S2 && S3 && S4)
    motor('S', 0, 0, 0);
  if (!S1 && !S2 && !S3 && !S4)
    motor('S', 0, 0, 0);
  if (!S1 && S2 && S3 && !S4)
    motor('F', 200, 0.85, 1);
  if ((S1 && !S2 && !S3 && !S4) || (S1 && S2 && !S3 && !S4))
    motor('R', 200, 0.85, 0);
  if ((!S1 && !S2 && !S3 && S4) || (!S1 && !S2 && S3 && S4))
    motor('L', 200, 0, 1);
}
