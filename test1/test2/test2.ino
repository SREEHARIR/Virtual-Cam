#define DEBUG false  // turn debug message on or off in serial
#include <Servo.h>
#include <SoftwareSerial.h>

// Create servo objects
Servo panServo;
Servo tiltServo;

// Center servos
int tiltVal = 90;
int panVal = 90;

int flag=0;
int offset=0;

// sensoduino stuff
String inText;
int value0, value1, value2;

SoftwareSerial esp(11,10);

void setup()
{
Serial.begin(9600);
esp.begin(9600);
//while (!Serial) {
//; // wait for serial port to connect. Needed for Leonardo only
//}

delay(2000);

pinMode(13,OUTPUT);
digitalWrite(13,LOW);
  panServo.attach(5);
  tiltServo.attach(6);
sendData("AT+RST\r\n",1000,DEBUG); // reset module
//sendData("AT+CWJAP=\"AFR\",\"mia9159afrafr\"\r\n",1000,DEBUG); // cridentials for router
delay(2000);
//sendData("AT+CWMODE=3\r\n",1000,DEBUG); // configure as access point
sendData("AT+CIFSR\r\n",1000,DEBUG); // get ip address //192.168.4.1
delay(1000);
//sendData("AT+CIPMUX=0\r\n",1000,DEBUG); // configure for multiple connections
//sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG); // turn on server on port 80
Serial.print("AT+CIPSTART=\"UDP\",\"192.168.1.2\",8080,9090,2\r\n");//,1000,DEBUG); // configure for multiple connections
delay(1000);

  int pos;
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    panServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    panServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    tiltServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  } 
  for(pos = 180; pos>=0; pos-=1)     // goes from 180 degrees to 0 degrees 
  {                                
    tiltServo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(10);                       // waits 15ms for the servo to reach the position 
  } 
  tiltServo.write(90);              // tell servo to go to position in variable 'pos' 
  panServo.write(90);              // tell servo to go to position in variable 'pos' 
}


void loop()
{
if(Serial.available()) // check if the esp is sending a message
{
//Serial.println("available");
if(Serial.find("+IPD,"))
{
  Serial.println("IPD");
delay(1000); // wait for the serial buffer to fill up (read all the serial data)
// get the connection id so that we can then disconnect
int connectionId = esp.read()-48; // subtract 48 because the read() function returns
// the ASCII decimal value and 0 (the first decimal number) starts at 48

// on your browser type http://192.168.4.1/?pin=13

Serial.find("81"); // advance cursor to "pin="
Serial.print("81 ");
// get 3 digits

  value0 = Serial.parseFloat();  // 1st sensor value
  Serial.print(" valuez=");
  Serial.print(value0);
  value1 = Serial.parseFloat();  // 2rd sensor value if exists
  Serial.print(" valuex=");
  Serial.print(value1);
  value2 = Serial.parseFloat();  // 3rd sensor value if exists
  Serial.print(" valuey=");
  Serial.print(value2);
    if((value0>(0))&&(value0<360))
    {
    tiltVal = value0; // value0 = yaw (pan) left-right
    }
    else 
    {
      return;
    }
    if((value1>(-180))&&(value1<180))
    {
      if(value1<0)
      {
        //value1=value1*(-1);
        panVal = value1; // value1 = pitch (tilt) up-down
      }
    }
    else 
    {
      return;
    }
    if(flag==0)
    {
      flag=1;
      offset=panVal+90;
    }
    //tiltVal = map(tiltVal, -10, 10, 0, 179);   // Map Accelerometer Y value to tilt servo angle.
    tiltServo.write((tiltVal));
    Serial.print(" tilt=");
    Serial.print(tiltVal);
    
    //delay(10);
    
    //panVal = map(panVal, -10, 10, 0, 179);  // Map Accelerometer X value to pan servo angle.
    panServo.write(panVal);//-offset);
    Serial.print(" pan=");
    Serial.print(panVal);//-offset);
    //delay(10);

//digitalWrite(pinNumber, !digitalRead(pinNumber)); // toggle pin
//
//// generate web page
//String webpage = String(pinNumber);
//String cipSend = "AT+CIPSEND=";
//cipSend += connectionId;
//cipSend += ",";
//cipSend +=webpage.length();
//cipSend +="\r\n";
//sendData(cipSend,1000,DEBUG);
//sendData(webpage,1000,DEBUG);

//// make close command
//String closeCommand = "AT+CIPCLOSE=";
//closeCommand+=connectionId; // append connection id
//closeCommand+="\r\n";
//
//sendData(closeCommand,1000,DEBUG); // close connection
}
}
}


/*
* Name: sendData
* Description: Function used to send data to ESP8266.
* Params: command - the data/command to send; timeout - the time to wait for a response; debug - print to Serial window?(true = yes, false = no)
* Returns: The response from the esp8266 (if there is a reponse)
*/

String sendData(String command, const int timeout, boolean debug)
{
String response = "";

Serial.print(command); // send the read character to the esp8266
//Serial.print(command); // send the read character to the esp8266
long int time = millis();

while( (time+timeout) > millis())
{
while(Serial.available())
{

// The esp has data so display its output to the serial window
char c = Serial.read(); // read the next character.
response+=c;
}
}

if(debug)
{
Serial.print(response);
}

return response;
}
