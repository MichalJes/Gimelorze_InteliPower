  #include "Arduino.h"
  #include <SPI.h>
  #include <WiFiNINA.h>

  class device
  {
    public:
          device();
      device(bool kind_, int id_, bool state_, int Class_)
      {
         kind=kind_;
         id=id_;
         state=state_;
         Class=Class_;
      }
      void set(bool kind_, int id_, bool state_, int Class_)
      {
      kind=kind_;
      id=id_;
      state=state_;
      Class=Class_;
      }
      String pack()
      {
       return  String(kind, BIN)+ " ID "+String(id, DEC) + " State "+String(state, BIN) + " class "+ String(Class, DEC) + '\n';     
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
    bool kind; //0-sensor, 1- 1xwallAdapter, 2-2xwallAdapter, ...
    int id; //device id, 0-nie podłączone
    bool state; //detecting movement/activated relay
    int Class; // 0-lamps, 1-
  };
