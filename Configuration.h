struct Device {
   String deviceName;
   int id;
   long address;
   int unit;
   int period;
};

//Describes the actual configuration structure
typedef struct Configuration {
  String ssid;
  String password;
  int deviceCount;
  Device devices[20];
} ;
