#include <LBT.h>
#include <LBTServer.h>

int readval;
String inputString = "";
String outputString = "";

int firstpoint=0;
int secondpoint=0;

void setup() {
 if(!LBTServer.begin((uint8_t*)"Sivam_LIO"))
 {
 return;
 }
// Serial.begin(9600); 
 Serial1.begin(9600); 
}
void loop() {
 uint8_t buf[64];
 int bytesRead;
 if(LBTServer.connected())
 {
 while(true)
 {
 bytesRead = LBTServer.readBytes(buf, 32);
 if(!bytesRead)
 break;
 inputString="";
 for(int j=0;j<32;j++)
 {
    char inChar = char(buf[j]);
    // add it to the inputString:
    inputString += inChar;   
 }
// Serial.println(inputString);
 Serial1.println(inputString);
// firstpoint=inputString.indexOf('>');
// Serial.println(inputString);
// Serial.println(firstpoint);
// if (firstpoint>0)
// {
//   secondpoint=inputString.indexOf('>',firstpoint+1);
//   if (secondpoint>0)
//   {
//     outputString=inputString.substring(firstpoint,secondpoint);
//     inputString=inputString.substring(secondpoint);
//     Serial.println(outputString);
//   }
// } 
 
 }
 delay(100);
 }
 else
 {
 LBTServer.accept(5);
 }
}
