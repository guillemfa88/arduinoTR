int potPin = 0;
int val = 0;

void setup() {
Serial.begin(115200);  // put your setup code here, to run once:

}

void loop() {
val = analogRead(potPin);  
val = map (val, 0, 1023, 0, 200);
Serial.print(val);
delay(100);
}
