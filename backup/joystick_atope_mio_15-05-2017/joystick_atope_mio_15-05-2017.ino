
//declarar pines digital i analogicos step, dir, y eje x del joystick 
const int stepPin = 3;
const int dirPin = 4;
int Xpin = A0;

//crear variables numeros enteros
int joy = 0;
int var = 0;

//regula velocitat
int vel = 7;

void setup() {
Serial.begin(115200); //serial begin para ver valores desde el ordenador

 pinMode(stepPin,OUTPUT); //declaro step i dir como pines de salida
  pinMode(dirPin,OUTPUT);
pinMode(Xpin, INPUT);//declaro Xpin como entrada


}

void loop() {

joy = analogRead(Xpin);//La leectura analogica de Xpin será igual a la variable joy
var = map(joy, 0, 1023, 0, 200); //Declaro la variable var como un mapeado para establecer la salida de 1023 a 200

joy = analogRead(Xpin);//Lee la entrada de Xpin y la guardamos en joy
Serial.print(var); //imprimimos por consola la variable var

  //
  if (var < 96) {
  digitalWrite(dirPin, LOW);
  
  digitalWrite(stepPin, HIGH);
  delay(vel);
  digitalWrite(stepPin, LOW);
  delay(vel);
  
  }else if(var > 97){
    digitalWrite(dirPin, HIGH);
  
  digitalWrite(stepPin, HIGH);
  delay(vel+1.5);
  digitalWrite(stepPin, LOW);
  delay(vel+1.5);
  }

//Si var es mayor que 97 que se mueva hacia un lado
/*if (var > 97) {
  digitalWrite(dirPin, HIGH);
  
  digitalWrite(stepPin, HIGH);
  delay(vel);
  digitalWrite(stepPin, LOW);
  delay(vel);
  
  }*/
  //delay(10);


}
