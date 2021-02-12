#define PIN_RESET     3
#define PIN_MODE      4
#define PIN_START     5
#define PIN_HALL_1    6

#define PIN_MOTOR_IN1 9
#define PIN_MOTOR_IN2 10

#define PIN_BRAKE_LED 13

#define MODE_STOP   0
#define MODE_BRAKE  1

#define STATUS_STOP   LOW
#define STATUS_START  HIGH

#define START_SPEED   0

int mode = MODE_STOP;
int speed = 0;
int status = STATUS_STOP;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial.println("Motor Stop/Brake --- start!");
  pinMode(PIN_MODE, INPUT);
  pinMode(PIN_START, INPUT);
  pinMode(PIN_HALL_1, INPUT);
  pinMode(PIN_MOTOR_IN1, OUTPUT);
  pinMode(PIN_MOTOR_IN2, OUTPUT);
  pinMode(PIN_BRAKE_LED, OUTPUT);
  
  // PWM 30kHz
  TCCR1A = 0b00000001;
  TCCR1B = 0b00000001;

  // Stop
  analogWrite(PIN_MOTOR_IN1, 0);
  analogWrite(PIN_MOTOR_IN2, 0);

  // MODE LED
  digitalWrite(PIN_BRAKE_LED, mode);
  Serial.print("mode=");
  Serial.println(mode);
  Serial.print("status=");
  Serial.println(status);
}

void loop() {
  // put your main code here, to run repeatedly:
  int temp;
  temp = digitalRead(PIN_RESET);
  if (temp == LOW) {
    analogWrite(PIN_MOTOR_IN1, 0);
    analogWrite(PIN_MOTOR_IN2, 0);
    speed = 0;
    status = STATUS_STOP;
    Serial.println("Reset!");
    delay(200);
  }
  if (status == STATUS_STOP) {
    temp = digitalRead(PIN_MODE);
    if (temp == LOW) {
      if (mode == MODE_STOP) {
        mode = MODE_BRAKE;
      } else {
        mode = MODE_STOP;
      }
      digitalWrite(PIN_BRAKE_LED, mode);
      Serial.print("mode=");
      Serial.println(mode);
      delay(200);
    }
    temp = digitalRead(PIN_START);
    if (temp == LOW) {
      status = STATUS_START;
      speed = START_SPEED;
      Serial.print("status=");
      Serial.println(status);
      delay(200);
    }
  } else {
    //Serial.println("PIN_HALL_1!");
    temp = digitalRead(PIN_HALL_1);
    if (temp == HIGH) {
      if (mode == MODE_BRAKE) {
        analogWrite(PIN_MOTOR_IN1, 255);
        analogWrite(PIN_MOTOR_IN2, 255);
        Serial.println("Brake!");
      } else {
        analogWrite(PIN_MOTOR_IN1, 0);
        analogWrite(PIN_MOTOR_IN2, 0);
        Serial.println("Stop!");
      }
      speed = 0;
      status = STATUS_STOP;
    } else {
      if (speed < 250) {
        speed += 1;
        analogWrite(PIN_MOTOR_IN1, speed);
        analogWrite(PIN_MOTOR_IN2, 0);
        Serial.print("speed=");
        Serial.println(speed);
      }
    }
  }
  delay(20);
}
