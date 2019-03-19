# Final project

# basic function (detailed function part)
***UI functionality***  
* The user can see the temperature graph and current temperature on UI.
  1. We used a webhook that hook to the thingspeak, which created the temperature graph in our UI.
  2. current temperature is sent by the photon
* If the temperature is too high, the buzzer will alert and you can click I know to stop the buzzer.
  1. if the sensor detects the temperature higher than 30, it will start the buzzer and send the information to the cloud and to UI to alert the user.
  The user can press the button in UI to stop the buzzer.
* User can set the time that he or she feed the fish and know it afterward.
  1. the time is set and send to the database. when you press the last time feed, you can retrieve the information from the database


***fishtank***
* 4-LED-matrix will display the temperature in the screen
  1. it use a library ledmatrix_max7219-max7221 to function the display
* one RGB led to show the color of the temperature
  1. the led can be set to different color by using function: set_color,fade_color.
* one buzzer to alert the user if the temperature is too high.
  1. if the buzzer will start if the temperature is too high.


***Extra credit:***
* 4-led-matrix (display the temperature)
* mysql database for log in (restore information login and register in the database), and the feeding time information
* using bootsrap as an external css library for the UI
* setting up Apache and put our UI in the server so it is more likely to be used as an app


# setup

***circuit setup***
* Buzzer
  1. connect to D3 and ground
* the 4 matrix led is set up by connection
  1. VCC to Vin
  2. GND to ground
  3. CLK to A0
  4. cs to A1
  5. Din to A2
* RGB LED connection
  1. Red LED pin to A3 by connecting 100 ohm resistor
  2. Blue LED pin to D0 by connecting 100 ohm resistor
  3. Green LED pin to D1 by connecting 100 ohm resistor
* temperature sensor
  1. 1k resistor in between middle pin and right pin
  2. right pin to 3.3V
  3. left pin to GND
  4. middle pin to D4 (one wire)


***ino setup***
* library
  1. #include "OneWire.h"
  2. #include"DS18.h"
  3. #include "ledmatrix-max7219-max7221.h"
* pin setup
  1. led = new LEDMatrix(4, 1, A0, A1, A2);
  2. pinMode(A3,OUTPUT);
  3. pinMode(D0,OUTPUT);
  4. pinMode(D1,OUTPUT);
  5. pinMode(D3,OUTPUT);

***back-end setup***
* MySQL database & php
  1. creating the fishing database and using it
  2. creating two tables: one for user info and one for fish feeding time

***front-end setup***
* HTML & JavaScript
  1. login
  2. register
  3. control screen has two sections
  4. fish tank temperature and fish feeding time

# modification (learning material)
* temperature logger tutorial (https://docs.particle.io/tutorials/hardware-projects/maker-kit/#tutorial-4-temperature-logger)
* RGB LED (check the https://build.particle.io/build/new and search for the cheer light)
* 4 matrix LED tutorial(https://appelsiini.net/2011/how-does-led-matrix-work/)
