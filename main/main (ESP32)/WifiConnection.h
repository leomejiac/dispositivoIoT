#include <WiFi.h>


// Replace the SSID/Password details as per your wifi router
//const char* ssid = "Claro_MERA0000918816";
//const char* password = "1919191515151";
const char* ssid = "CLARO_MEJIA";
const char* password = "0400918264";
//const char* ssid = "iPhone de Leonardo";
//const char* password = "Lalcuadrado1";

void setup_wifi() {
  delay(50);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  int c=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000); //
    Serial.print(".");
    c=c+1;
    if(c>10){
        ESP.restart(); //restart ESP after 10 seconds
    }
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  
}


 

