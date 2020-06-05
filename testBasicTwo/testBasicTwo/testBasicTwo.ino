#include <ESP8266WiFi.h>        // Include the Wi-Fi library

#define MAX_MESSAGE 30

static char buff[MAX_MESSAGE];
const char* ssid = "none"; //Enter SSID
const char* password = "password"; //Enter Password
char incoming = 0;

String wifiSel = "noneYet";

bool ssidSelected = "false";

int itNum = 0;
int ind = 0;
int seconds = 0;
int wifiNetwork = 0;

void setup() {
  Serial.begin(115200);

  Serial.print("Scan start ... ");
  int n = WiFi.scanNetworks();
  Serial.print(n);
  Serial.println(" network(s) found");
  for (int i = 0; i < n; i++)
  {
    Serial.print(i);
    Serial.print(" ");
    Serial.print(WiFi.SSID(i));
    Serial.print('\n');
  }
  Serial.println();

  Serial.print("targeting ");//targeting "ssid"
  Serial.print(ssid);
  Serial.print('\n');

  Serial.println("select number of network to targert");

  while(ssid = "none"){
  if (Serial.available() > 0) {
  incoming = Serial.read();
  if(incoming != '\n')
  {
  wifiNetwork = incoming - '0';

  wifiSel = WiFi.SSID(wifiNetwork);
  (WiFi.SSID(wifiNetwork)).toCharArray(buff, MAX_MESSAGE);
  ssid = buff;
  Serial.print("wifiNetwork ");
  Serial.print(ssid);
  Serial.print(" selected");
  Serial.print('\n');
  Serial.print('\n');
  Serial.print('\n');
  Serial.print('\n');
  Serial.print('\n');


  Serial.println("please enter a password to try");
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

      Serial.println("please enter a password to try");
      Serial.print('\n');

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
    Serial.print("Information:");
    Serial.print('\n');
    WiFi.printDiag(Serial);
  }
  }
  }
}

void loop() {

}
