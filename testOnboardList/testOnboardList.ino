#include <ESP8266WiFi.h>        // Include the Wi-Fi library
#include <FS.h> //for the file system
//#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library

const char* ssid = "KNAM"; //Enter SSID
char* password = "password"; //Enter Password
char strValue[10];

char *passwords[] = {"This is string 1", "This is string 2", "This is string 3",
                     "This is string 4", "This is string 5", "This is string 6"
                    };

int itNum = 1;
int ind = 0;

void setup(void)
{ 
  Serial.begin(115200);
  while (WiFi.status() != WL_CONNECTED) 
  {
    WiFi.begin(ssid, password); //eventually, you will have to add an identifier, for example, in serial event, check if ch is # and if so, assume it is the ssid
    
    Serial.print("trying password number ");
    Serial.print(itNum);
    Serial.print(": ");
    Serial.print(password);
    Serial.print('\n');
    delay(50);
    itNum++;
  }
  Serial.print('\n');
  Serial.println("WiFi connection Successful");
  Serial.print('\n');
  Serial.print("The IP Address of ESP8266 Module is: ");
  Serial.print(WiFi.localIP());// Print the IP address
  Serial.print('\n');
  Serial.print("The WiFi password for ");
  Serial.print(ssid);
  Serial.print(" is ");
  Serial.print(password);
}


void loop() 
{
  // EMPTY
}
