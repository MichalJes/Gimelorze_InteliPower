  #include "Arduino.h"
  #include <SPI.h>
  #include <WiFiNINA.h>

  class device
  {
    public:
          device();
      device(int kind_, String id_, bool state_, int Class_)
      {
         kind=kind_;
         id=id_;
         state=state_;
         state2=Class_;
      }
      void set(int kind_, int id_, bool state_, int Class_)
      {
      kind=kind_;
      id=id_;
      state=state_;
      state2=Class_;
      }
      String pack()
      {
       return  String(kind, DEC)+ " "+ id + " " +String(state, BIN) + " "+ String(state2, DEC) +  " "+String(curr1, DEC) +" "+String(curr2, DEC)+'\r';     
      }
      void on()
      {
        state=1;
        }
        void off()
        {
          state=0;
        }
      
    private:
    int kind=1; //0-sensor, 1- 1xwallAdapter, 2-2xwallAdapter, ...
    String id; //device id, 0-nie podłączone
    bool state; //detecting movement/activated relay
    int state2; // 0-lamps, 1-
    int curr1=6;
    int curr2=1;
  };
