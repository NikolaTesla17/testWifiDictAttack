#include <ESP8266WiFi.h>        // Include the Wi-Fi library
//#include <ESP8266WiFiMulti.h>   // Include the Wi-Fi-Multi library

const char* ssid = "KNAM"; //Enter SSID
const char* password = "password"; //Enter Password
char strValue[10];
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

void serialEvent()
{
  while(Serial.available())
  {
    char ch = Serial.read();
    Serial.write(ch);
    if(ch != ';') { 
            strValue[ind++] = ch; 
    } else if(ch == ','){ 
         strValue[ind] = 0; 
         password = (strValue); 
    }
     strValue;
     ind = 0;
  }
}
