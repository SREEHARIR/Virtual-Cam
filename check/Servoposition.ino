#include <Servo.h> 
Servo leftright;
Servo updown;
Servo shake;


void setup() {
  Serial.begin(9600);

  leftright.attach(3);
  updown.attach(5);
  shake.attach(6);
  leftright.write(90);
  updown.write(90);
  shake.write(90);  
}

void loop() {

}
