# Rare-and-Side-Object-detection-for-ebikes
![image](https://user-images.githubusercontent.com/47792802/215340900-f6a6261b-9ae5-4d8f-a19c-69be15e3a8f0.png)

In this project, two different kinds of Doppler Radar Sensors are used: 
the RCWL-0561, which has 360 detection angle and range of 5m-7m used for motion detection 
and HB100 with 160 degrees detection angle and range of 15m.

Three HB100 sensors are installed to cover the blind spots and to measure the speed and direction 
of the approaching vehicles. 
The detection range of the device is kept within 5-7ms, which means that data from the hb100 sensors 
is shown only when the RCWL-0561 detects the movement of any oncoming vehicle. 

An Arduino Uno is used to process the sensors data, which is then displayed on the mobile app via ESP32 Wi-Fi Module.
