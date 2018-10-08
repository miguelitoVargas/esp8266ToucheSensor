#include <data_handler.h>
#include <Arduino.h>




data_handler::data_handler(Stream &out) : _out(out) 
{

}

void data_handler::_readArduinoTouche( unsigned long timeout) {

  unsigned long start = millis();

  wdt_disable();

  do {

    while(_out.available()) {

      handleData();
    }

  }while(millis() - start < timeout);

  wdt_enable(5);

}
void data_handler::handleData() {

//  if(Serial.available()){

    int inByte = _out.read();


    if(inByte == 0) serialCount = 0;


    // add a byte 
    serialInArray[serialCount] = inByte;
    serialCount++;

    Error = true;
    if (serialCount >= NUM_BYTES) {
      serialCount = 0;

      TotalRecieved++;

      int Checksum=0;
      //    
      //    Checksum = (Command + yMSB + yLSB + xMSB + xLSB + zeroByte)%255;
      for (int x=0; x<NUM_BYTES-1; x++) {
        Checksum=Checksum+serialInArray[x];
      }

      Checksum=Checksum%255;
      if (Checksum==serialInArray[NUM_BYTES-1]) {
        Error = false;
        DataRecieved=true;
      }
      else {
        Error = true;
        //  println("Error:  "+ ErrorCounter +" / "+ TotalRecieved+" : "+float(ErrorCounter/TotalRecieved)*100+"%");
        DataRecieved=false;
        ErrorCounter++;
        //        println("Error:  "+ ErrorCounter +" / "+ TotalRecieved+" : "+float(ErrorCounter/TotalRecieved)*100+"%");
      }
    }
    //-------
    if (!Error) {

      int zeroByte = serialInArray[6];

      // println (zeroByte & 2);

      xLSB = serialInArray[3];
      if ( (zeroByte & 1) == 1) xLSB=0;
      xMSB = serialInArray[2];      
      if ( (zeroByte & 2) == 2) xMSB=0;

      yLSB = serialInArray[5];
      if ( (zeroByte & 4) == 4) yLSB=0;

      yMSB = serialInArray[4];
      if ( (zeroByte & 8) == 8) yMSB=0;


      //   println( "0\tCommand\tyMSB\tyLSB\txMSB\txLSB\tzeroByte\tsChecksum"); 
      //  println(serialInArray[0]+"\t"+Command +"\t"+ yMSB +"\t"+ yLSB +"\t"+ xMSB +"\t"+ xLSB+"\t" +zeroByte+"\t"+ serialInArray[7]); 

      // >=====< combine bytes to form large integers >==================< //

      Command  = serialInArray[1];

      xValue   = xMSB << 8 | xLSB;                    // Get xValue from yMSB & yLSB  
      yValue   = yMSB << 8 | yLSB;                    // Get yValue from xMSB & xLSB

      // println(Command+ "  "+xValue+"  "+ yValue+" " );

      /*
         How that works: if xMSB = 10001001   and xLSB = 0100 0011 
         xMSB << 8 = 10001001 00000000    (shift xMSB left by 8 bits)                       
         xLSB =          01000011    
         xLSB | xMSB = 10001001 01000011    combine the 2 bytes using the logic or |
         xValue = 10001001 01000011     now xValue is a 2 byte number 0 -> 65536  
         */

      /*  ==================================================================
          Command, xValue & yValue have now been recieved from the chip
          ==================================================================  */
      switch(Command) {


        /*  ==================================================================
            Recieve array1 and array2 from chip, update oscilloscope      
            ==================================================================  */

        case 1: // Data is added to dynamic arrays

    //      DataRecieved3 = false;// make sure we don't analyze as we push data, and make sure
          DynamicArrayTime3.push_back(xValue);//=append( DynamicArrayTime3, (xValue) );
          DynamicArray3.push_back(yValue);//=append( DynamicArray3, (yValue) );
          yield();
          //      Serial.println("added values to the vectors");

          break;

        case 2: // An array of unknown size is about to be recieved, empty storage arrays


          Time3=DynamicArrayTime3;
          Voltage3=DynamicArray3;

          DataRecieved3=true;

         /* Serial.print("added some data   ");
          Serial.println(Time3.size());*/ 

          DynamicArrayTime3.clear(); 
          DynamicArray3.clear();
        //   Serial.println("assigned values and cleared serial buffers");
          break;    

      }
    }
    // ------


 // }
}
//---------------
// calc dist between the sampled points
// 
float data_handler::cal_dist (float x, float y, float xi, float yi) {

  float x_diff = x - xi;
  float y_diff = y - yi;
  float distance = sqrt(x_diff*x_diff + y_diff*y_diff);

  return distance;

}
//----------
//sample through values en get max value
void data_handler::sample_max_value () {

/*  Serial.print("voltajes  ");
  Serial.print(Voltage3.size());
  Serial.print("tiempos  ");
  Serial.println(Time3.size());*/

      //sample the max value in the array, only if it has data
  if(Time3.size() > 0) {
    maxVoltage = 0;
    maxTime = 0;
    for(int i=0; i<Time3.size()-1; i++){

      if (maxVoltage < Voltage3.at(i)){

        maxVoltage = Voltage3.at(i);
        maxTime = i;
      }

    }

  }

}
//handle gesture input
//
void data_handler::handle_gestures () {

  if (DataRecieved3) {

    // sample en get max values for time and voltage
    // 

//        Serial.print("gonna handle a gesture");



    sample_max_value();
    gesture1 = maxVoltage;


    // push values to gesture array when recieved osc 
    // request
    switch (rec_gesture_1) {

      case 1:
        //sample_values();
        gesturePoints[0][0] = maxTime;
        gesturePoints[0][1] = maxVoltage;

      Serial.print("added gesture point ");
        Serial.println(gesturePoints[0][1]);
        break;
      case 2:

        break;
    }

    Serial.print("THE VOLTAGE:  ");
    Serial.println(maxVoltage);
    gestureDist[0] = cal_dist(maxTime,maxVoltage,gesturePoints[0][0],gesturePoints[0][1]);

  }
}
//-----
//
void data_handler::setGesture1(int g) {
  rec_gesture_1 = g;
}
//---- return the gesture
int data_handler::getData() {
  return gesture1;
}
//---------------



