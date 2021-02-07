const int accel_pin = A0;
const int brake_pin = A1;
const int wheel_pin = A2;

void setup() {
	Serial.begin(9600);
}

void loop() {

	Serial.print("Min:0");
	Serial.print(",Max:1024");

	Serial.print(",Wheel:");
	Serial.print(analogRead(wheel_pin));

	Serial.print(",Accel:");
	Serial.print(analogRead(accel_pin));
	
	Serial.print(",Brake:");
	Serial.print(analogRead(brake_pin));
	
	Serial.println("");
	delay(2);

}
