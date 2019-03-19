# Final project


***ui***  
* database.php (the foundation php for querying the database)
* fish.js (use fish.js to comunicate with the particle)
* fishApp.html (the UI for fish tank)
* login.php (the php handles the login which making queries to the database)
* logout.php (the php handles the logout)
* main.js (the front-end functions for both UI and and fish tank functions )
* register.php (the php handles registering new users)

***fishtank***
1. fishtank.ino
(main ino that perform basic function, like detect temperature, fade light to another color and start the alert when temperature is too high)
2. library:
OneWire for detecting the Temperature
ledmatrix-max7219-max7221 for 4-matrix led-matrix


***Extra credit:***
* 4-led-matrix (display the temperature)
* mysql database for log in (restore information login and register in the database), and the feeding time information
* using bootsrap as an external css library for the UI
* setting up Apache and put our UI in the server so it is more likely to be used as an app
