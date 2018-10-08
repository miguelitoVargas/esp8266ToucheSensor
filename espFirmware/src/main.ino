#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <OSCMessage.h>
//#include <app.h>
#include <data_handler.h>
//---------- wifi ssid psswd
//touche_wifi.psswd = "chOc0l4t1n4";

char ssid[] = "Essteban";
char psswd[] = "12345678";

WiFiUDP Udp;
const IPAddress outIp(192,168,43,169);
const unsigned int outPort = 4000;
const unsigned int localPort = 4001;
OSCErrorCode error;

// touche data handler
data_handler my_data_handler(Serial);
// incoming data

/*const int numOfSerialBytes = 8;
  int serialInArray[numOfSerialBytes];
  int serialCount = 0;
// bytes of data
int xMSB, xLSB, yMSB, yLSB;*/

void setup(){

  Serial.begin(115200);
  pinMode(2, OUTPUT);

  //  touche_wifi.ssid = "TC";
  //touche_wifi.psswd = "chOc0l4t1n4";
  WiFi.begin(ssid,psswd);

  while(WiFi.status() != WL_CONNECTED) {

    delay(500);
    Serial.print(".");

  }

  Serial.println("wifi connected");
  Serial.println(WiFi.localIP());
  Serial.println("starting udp");
  Udp.begin(localPort);

  //  my_data_handler.popTestVector();

}

void record_gesture(OSCMessage &inc_msg) {

      int value = inc_msg.getInt(0);
      my_data_handler.setGesture1(value);
}

void loop() {



  my_data_handler._readArduinoTouche(0.01);
  my_data_handler.handle_gestures();

  OSCMessage msg("/touche/1");
  //  msg.add(my_data_handler.test.at(2));

  msg.add(my_data_handler.getData());
  msg.add(my_data_handler.rec_gesture_1);
  msg.add(my_data_handler.gesturePoints[0][1]);
  msg.add(my_data_handler.gestureDist[0]);
  Udp.beginPacket(outIp, outPort);
  msg.send(Udp);
  Udp.endPacket();
  msg.empty(); 
  //--------------
  //digitalWrite(2, HIGH);

  OSCMessage inc_msg;
  int sizeOfmsg = Udp.parsePacket();

  if (sizeOfmsg > 0) {

    while (sizeOfmsg--) {


      inc_msg.fill(Udp.read());
    }
    if (!inc_msg.hasError()) {
      inc_msg.dispatch("/gesto/1",record_gesture );
    } else {
      error = inc_msg.getError();
/*      Serial.print("error: ");
      Serial.println(error);*/
    }
  }


  }




/*void serialEvent() {

  while (Serial.available() <= 0) {

  char inChar = (char)Serial.read();
// char inChar[2] = {(char)Serial.read()};
inputString += inChar;
//inputString = "M";
inputString.toCharArray(outByte,2);
//inByte = new char[2]{(char)Serial.read()};
//    inByte = inChar.at(1);
if (inputString == "\n") {
stringComplete = true;
}
}

}*/
