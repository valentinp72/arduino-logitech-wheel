#include <Joystick.h>
#include <Keypad.h>

const int accel_pin = A0;
const int brake_pin = A1;
const int wheel_pin = A2;

const byte ROWS = 4;
const byte COLS = 4;

char keys[ROWS][COLS] = {
	{1, 13, 10, 3},
	{8, 7,  12, 4},
	{2, 15, 11, 14},
	{6, 16, 9,  5}
};


byte rowPins[ROWS] = {4, 7, 8, 9};
byte colPins[COLS] = {10, 14, 15, 16};
const int vibration_led_pin = 3; 
const int vibration_btn_pin = 2;

Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);

Joystick_ Joystick(
	JOYSTICK_DEFAULT_REPORT_ID,
	JOYSTICK_TYPE_JOYSTICK,
	16,                   // button count
	0,                    // hat switch count
	true, true, true,     // X Y Z axis
	false, false, false,     // rX rY rZ axis
	false,                 // rudder
	false,                 // throttle
	false,                 // accelerator
	false,                 // brake
	false                  // steering
);

//float EMA_a = 0.25;
//int EMA_s = 0;
const int numReadings = 100;

int readings[numReadings];      // the readings from the analog input
int readIndex = 0;              // the index of the current reading
long int total = 0;                  // the running total
long int average = 0;                // the average

int vibration_toggle = 1;


int wheel_min = 0, wheel_max = 0;
int brake_min = 0, brake_max = 0;
int accel_min = 0, accel_max = 0;
int wheel_offset = 0;

void buttonWait(int pin) {
  int buttonState = digitalRead(pin);
  while(buttonState == HIGH) {
    buttonState = digitalRead(pin);
  }
  while(buttonState == LOW) {
    buttonState = digitalRead(pin);
  }
}

void ledBlink(int pin) {
  for (int i = 0 ; i < 5 ; i++) {
    digitalWrite(pin, HIGH);
    delay(50);
    digitalWrite(pin, LOW);
    delay(20);
  }
}

void calibrate() {
  
  buttonWait(vibration_btn_pin);
  ledBlink(vibration_led_pin);

  wheel_offset = analogRead(wheel_pin) - 512;

  buttonWait(vibration_btn_pin);
  ledBlink(vibration_led_pin);

  brake_min = analogRead(brake_pin);
  accel_min = analogRead(accel_pin);
  wheel_min = analogRead(wheel_pin);

  buttonWait(vibration_btn_pin);
  ledBlink(vibration_led_pin);

  brake_max = analogRead(brake_pin);
  accel_max = analogRead(accel_pin);
  wheel_max = analogRead(wheel_pin);

  Serial.print("Brake "); Serial.print(brake_min); Serial.print(" "); Serial.println(brake_max);
  Serial.print("Accel "); Serial.print(accel_min); Serial.print(" "); Serial.println(accel_max);
  Serial.print("Wheel "); Serial.print(wheel_min); Serial.print(" "); Serial.println(wheel_max);
  Serial.print("Wheel offset "); Serial.println(wheel_offset);

    buttonWait(vibration_btn_pin);
}

void setup() {
	Serial.begin(9600);
	Joystick.begin();
	Joystick.setXAxisRange(-512, 511);
	Joystick.setYAxisRange(0, 1023);
	pinMode(vibration_led_pin, OUTPUT);
	pinMode(vibration_btn_pin, INPUT_PULLUP);

  // while(1) {
  // 	Serial.print("0,1023,");
  //   Serial.print(wheel_offset);
  //   Serial.print(",");
  //   Serial.println(analogRead(wheel_pin) - wheel_offset);
  //   delay(50);
  // }
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0;
  }
	//EMA_s = map(analogRead(wheel_pin), 0, 1023, -512, 511);

  calibrate();
}

int old_wheel = 0;
int old_accel = 0;
int old_brake = 0;

int last_key = 0;

void loop() {
  if(digitalRead(vibration_btn_pin) == LOW) {
    calibrate();
  }
  
	int key = (int) keypad.getKey();

	if (key) {
		if (keypad.getState() == PRESSED) {
			last_key = key - 1;
			Joystick.setButton(key - 1, HIGH);
		}
	}
	if (keypad.getState() == RELEASED) {
		Joystick.setButton(last_key, LOW);
	}

	// int wheel = map(analogRead(wheel_pin), 0, 1023, -512, 511);

  // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(wheel_pin) - wheel_offset;
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;

  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }

  // calculate the average:
  average = total / numReadings;
 
	//EMA_s = (EMA_a * wheel) + ((1 - EMA_a) * EMA_s);
	//wheel = EMA_s;
	if (average != old_wheel || 1) {
    int x_value = map(average, wheel_min, wheel_max, -512, 511);
		// Joystick.setXAxis(x_value);
		old_wheel = average;
		// Serial.print("-512,512,");
    // Serial.print(wheel_offset); Serial.print(",");
    // Serial.print(toto); Serial.print(",");
    int smoothed = 0;
    if (x_value > 0) {
      smoothed = map(x_value, 0, 407, 0, 511);
    }
    else {
      smoothed = map(x_value, -650, 0, -512, 0);
    }
    Joystick.setXAxis(smoothed);
    // Serial.print(smoothed); Serial.print(",");
		// Serial.println(x_value);
	}

	int accel = analogRead(accel_pin);
	if (accel != old_accel) {
		Joystick.setYAxis(map(accel, accel_min, accel_max, 0, 1023));
		old_accel = accel;
	}

	int brake = analogRead(brake_pin);
	if (brake != old_brake) {
		Joystick.setZAxis(map(brake, brake_min, brake_max, 0, 1023));
		old_brake = brake;
	}

	delay(5);
}
