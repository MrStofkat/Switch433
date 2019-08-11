struct Device {
  unsigned long address;
  unsigned long unit;
  unsigned long period;
  boolean lastState;
  String deviceName;
};

//Describes the actual configuration structure
typedef struct Configuration {
  String ssid;
  String password;
  int deviceCount;
  Device devices[20];
};
