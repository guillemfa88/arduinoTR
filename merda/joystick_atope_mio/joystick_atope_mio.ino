
const int stepPin = 3;
const int dirPin = 4;
int Xpin = A0;
int joy = 0;
int var = 0;
void setup() {
Serial.begin(115200);
 pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
pinMode(Xpin, INPUT);

// digitalWrite(dirPin, HIGH);
}

void loop() {

joy = analogRead(Xpin);
var = map(joy, 0, 1023, 0, 200);

joy = analogRead(Xpin);
Serial.print(var);


if (var > 97) {
  digitalWrite(dirPin, HIGH);
  
  digitalWrite(stepPin, HIGH);
  delay(4);
  digitalWrite(stepPin, LOW);
  delay(4);
  
  }
  delay(10);
  if (var < 96) {
  digitalWrite(dirPin, LOW);
  
  digitalWrite(stepPin, HIGH);
  delay(4);
  digitalWrite(stepPin, LOW);
  delay(4);
  
  }
}
