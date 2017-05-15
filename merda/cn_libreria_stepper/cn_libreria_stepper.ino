#include <Stepper.h>
int STEPS = 0;
Stepper stepper(STEPS, 8, 9); //Stepper nombre motor (número de pasos por vuelta, pins de control)
 
int pot;  //Variable lectura potenciómetro
int derecha=3;  //Pulsador derecha
int izquierda=2;  //Pulsador izquierda
int direccion;  //Variable para indicar la direccón
 
void setup()
{
  pinMode(derecha,INPUT);
  pinMode(izquierda,INPUT);
}
 
void loop()
{
  pot=analogRead(A0); //Lectura potenciómetro
  pot = map(pot, 0, 1023, 30, 150); //Establecemos la velocidad entre 30 y 150 rpm
 
  stepper.setSpeed(pot); //Indicamos la velocidad al motor
 
  stepper.step(direccion); //Indicamos la dirección al motor
 
  if(digitalRead(izquierda)==HIGH)
  {
     direccion=200;  //Si pulsamos el pulsador izquierdo, el motor gira a la izquierda
  }
 
  if(digitalRead(derecha)==HIGH)
  {
     direccion=-200;  //Si pulsamos el pulsador derecho, el motor gira a la derech
  }
}

