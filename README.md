# Switch433
IoT RF control server for a range of standard 433mhz protocols. This can run on any ESP8266 device. Make sure to check for the correct pinout. 

## Module structure
This project uses a module like structure. This makes the inner working of this project somewhat functional instead of sequential. Every module has a seperate goal. The RFModule handles the sending and receiving of codes , the WifiModule handles network setup and API calls while the ConfigModule keeps the currect state and holds saved devices.

## Klik aan Klik uit
The project has been tested to work with the Dutch 433 wall socket system Klik aan Klik uit https://www.klikaanklikuit.nl/ but should work with many other systems that basically use the same protocol.

## React frontend
This part of the application only consists of the API. A frontend implementation can be found here: https://github.com/MrStofkat/react-iot


