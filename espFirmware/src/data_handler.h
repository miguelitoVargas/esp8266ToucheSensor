#pragma once
#ifndef DATA_HANDLER_H
#define DATA_HANDLER_H

#include <Arduino.h>
#include <vector>
#include <Stream.h>
#define NUM_BYTES 8


  class data_handler {

    public:

      data_handler(Stream &out);
      void _readArduinoTouche(unsigned long timeout);


      // data array
      int serialInArray[NUM_BYTES];
      int serialCount;
      /*   =================================================================================       
           Global variables
           =================================================================================*/ 
      int xValue, yValue, Command; 
      bool Error=true;
      int ErrorCounter=0;
      int TotalRecieved=0; 

      /*   =================================================================================       
           Local variables
           =================================================================================*/

      bool   DataRecieved3=false;

      std::vector<float> Time3; 
      std::vector<float> DynamicArrayTime3; 
      std::vector<float> Voltage3;
      std::vector<float> DynamicArray3;


      bool DataRecieved=false;
      int xMSB, xLSB, yMSB, yLSB;		                // Bytes of data

      ///---- gesture types
      //
      float gesturePoints[4][2];
      float gestureDist[4];
      float maxTime, maxVoltage;
      int gesture1;

      int increment;

      float cal_dist (float x, float y, float xi, float yi);
      void handle_gestures();
      void sample_max_value();
      void setGesture1(int g);
      int rec_gesture_1;
      void get_record_gesture();
      int getData();

    private:


      void handleData();
      Stream &_out;

  };

#endif
