/*
 Stepper Motor Joystick Control
 This program drives a bipolar stepper motor with DRV8825 driver.
 The driver is attached to digital pins 8 - 13 of the Arduino.
 The joystick is attached to analog pin A0 (X axis) and digital pin 2 (switch)
 The motor rotation is controlled by X axis of the joystick.
 The switch brings motor back to its initial position
 This program can also receive values from IIC bus through
 address 0x12
 Created 30 Sep. 2016
 by Cyril BOSSELUT
 */
 
#include <Wire.h>
#include "DRV8825.h"

#define DIR  8
#define ST  9
#define M0  10
#define M1  11
#define M2  12
#define EN  13
#define X_pin A0    // Pin A0 connected to joystick x axis
#define SWITCH 2 // Pin 2 connected to joystick switch
#define MAX_WAIT 100000 //max inactivity time in seconds before disabling motor
#define STEPS 200 // change this to fit the number of steps per revolution for your motor
#define SLAVE_ADDRESS 0x12
#define FOCUS 6
#define SHUTTER 7
#define RELAY 3
#define EORR 4 // Right limit switch
#define EORL 5 // Left limit switch

DRV8825 myStepper(STEPS, DIR, ST, EN, M0, M1, M2);
int pos = 0;
bool changed = false;
bool isEnable = false;
int timeout = 0;
int wireValue = 0;
int rpm = 240;

void setup() {
  pinMode(FOCUS, OUTPUT);
  pinMode(SHUTTER, OUTPUT);
  pinMode(RELAY, OUTPUT);
  pinMode(EORR, INPUT);
  pinMode(EORL, INPUT);
  digitalWrite(FOCUS, LOW);
  digitalWrite(SHUTTER, LOW);
  // set the speed:
  myStepper.setRPM(rpm);
  // set microstepping ON
  myStepper.setMicrostep(1);
  // release motor
  myStepper.disable();
  // initialize the serial port:
  Serial.begin(115200);
  // init switch
  digitalWrite(SWITCH, HIGH);
  // init I2C client
  Wire.begin(SLAVE_ADDRESS);
  Wire.onReceive(receiveData);
  Wire.onRequest(sendData);
  delay(10);
  // power on motor
  digitalWrite(RELAY, HIGH);
}

void loop() {
  if(digitalRead(EORR) == HIGH || digitalRead(EORL) == HIGH){
    digitalWrite(RELAY,LOW);
  }
  else{
    if(Serial.available() > 0){
      int value = readSerialInt();
      if(value != 0){
        if(!isEnable) enable();
        stepper(value, true);
        delay(500);
      }
    }
    else if(wireValue != 0){
      if(!isEnable) enable();
      stepper(wireValue, true);
      wireValue = 0;
      delay(500);
    }
    else{
      int x_value = analogRead(X_pin);
      if(x_value <= 400 && x_value > 200){
        // counterclockwise half-speed
        if(!isEnable) enable();
        stepper(-1, false);
      }
      else if(x_value <= 200){
        // counterclockwise full-speed
        if(!isEnable) enable();
        stepper(-1, true);
      }
      else if(x_value >= 600 && x_value < 800){
        // clockwise half-speed
        if(!isEnable) enable();
        stepper(1, false);
      }
      else if(x_value >= 800){
        // clockwise fullf-speed
        if(!isEnable) enable();
        stepper(1, true);
      }
      else if(digitalRead(SWITCH) == LOW && pos != 0){
        // back to home
        if(!isEnable) enable();
        stepper(-pos, true);
        while(digitalRead(SWITCH) == LOW){}
        //delay(500);
      }
      else if(changed){
        changed = false;
        Serial.print("Current pos: ");
        Serial.println(pos);
      }
      else if(isEnable){
        timeout -= 2;
        if(timeout == 0){
            disable();      
        }
        delay(2);
      }
    }
  }
}

void stepper(int value, bool fullSpeed){
    if(!isEnable){
      return;
    }
    myStepper.setRPM((fullSpeed)?rpm:rpm/2);
    myStepper.move(value);
    Serial.println("Motor rotation done");      
    pos += value;  
    changed = true;
    timeout = MAX_WAIT;
}

void enable(){
    myStepper.enable();
    isEnable = true;
    timeout = MAX_WAIT;
    Serial.println("Motor enable");
}

void disable(){
    myStepper.disable();
    isEnable = false;
    timeout = 0;
    Serial.println("Motor disable");  
}

void setSpeed(int sp){
  if(sp < 0){
    sp = 0;
  }
  else if(sp > 255){
    sp = 255;
  }
  Serial.print("Change speed to: ");
  Serial.println(sp);
  myStepper.setRPM(sp);
  rpm = sp; 
}

void focus(){
  Serial.println("Camera focus");
  digitalWrite(FOCUS, HIGH);
  delay(15000);
  digitalWrite(FOCUS, LOW);
  delay(10);
}

void shutter(){
  Serial.println("Camera shutter 15");
  digitalWrite(SHUTTER, HIGH);
  delay(7500);
  digitalWrite(SHUTTER, LOW);
  delay(10);
}

int readSerialInt(){
    int value = 0;
    char inByte;
    bool negativeNumber = false;
    while(1){
      inByte = Serial.read();
      if(inByte == '\n') break;
      if(inByte == 0x45){
        enable();
        break;
      }
      if(inByte == 0x44){
        disable();
        break;
      }
      if(inByte == 0x46){
        focus();
        break;
      }
      if(inByte == 0x53){
        shutter();
        break;
      }
      if(inByte == '-') {
        negativeNumber = true;
        continue;
      }
      if(inByte < 0x30 || inByte > 0x39) continue;
      value *= 10;
      value = ((inByte - 48) + value);
    }
    Serial.flush();
    if(negativeNumber) value = -value;
    return value;
}

void receiveData(int byteCount){
  int value = 0;
  bool negativeNumber = false;
  Serial.println("Receive data on I2C bus");
  Serial.print("Length: ");
  Serial.println(byteCount);
  for(int i = 0; i < byteCount; i++){
    byte b = Wire.read();
    if(b == 0x46){
      focus();
    }
    else if(b == 0x53){
      shutter();
    }
    else if(b == '-') {
      negativeNumber = true;
    }
    else if(b == 0x4f){
      pos = 0;
    }
    else if(b == 0x52){
      setSpeed(Wire.read());
      i++;
    }
    else{
      value = b << 8*i | value;
    }
    Serial.print(b);
  }
  if(negativeNumber) value = -value;
  wireValue = value;
  Serial.println("");
  if(value != 0){
    Serial.print("Value: ");
    Serial.println(value);
  }
}

void sendData(){
  Serial.println(pos);
  Wire.write((const uint8_t*)&pos, sizeof(int16_t));
}
