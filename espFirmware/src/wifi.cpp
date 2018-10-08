#include <app.h>
#include <string>

void setupWifi () {

//touche_wifi.ssid = "popo";
  //conect wifi
  Serial.println();
  Serial.print("connecting to:  ");
//  Serial.println(ssid);
// String shit = touche_wifi.ssid;
 // char id[];
  //char passwd[];

//  WiFi.begin(touche_wifi.ssid.c_str(),touche_wifi.psswd.c_str());

  while(WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println("wifi connected");
  Serial.println(WiFi.localIP());

}
