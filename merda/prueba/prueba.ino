

//simple A4988 connection
//jumper reset and sleep together
//connect  VDD to Arduino 3.3v or 5v
//connect  GND to Arduino GND (GND near VDD)
//connect  1A and 1B to stepper coil 1
//connect 2A and 2B to stepper coil 2
//connect VMOT to power source (9v battery + term)
//connect GRD to power source (9v battery - term)


int stp = 13;  //connect pin 13 to step
int dir = 12;  // connect pin 12 to dir
int a = 0;     //  gen counter
int potPin = 0;
int val = 0;

void setup() 
{                
 Serial.begin(115200);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);       
}


void loop() 
{
  val = analogRead(potPin);  
val = map (val, 0, 1023, 0, 200);
  if (a <  val)  //sweep 200 step in dir 1
   {
    digitalWrite(stp, HIGH);   
    delay(3);              
    digitalWrite(stp, LOW);  
    delay(3);              
   }
  else 
   {
    digitalWrite(dir, HIGH);
    val++;
    digitalWrite(stp, HIGH);  
    delay(3);              
    digitalWrite(stp, LOW);  
    delay(3);
    

    }
    
}
