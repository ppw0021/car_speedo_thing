# GPS Speed Monitor
This was a small project completed in a day at the request of one of my friends. His car does not have a working speedometer, and he wanted a means to see how fast he was going.

# Description
This project uses an Arduino Uno, an I2C 16x2 Display, and a ublox NEO 7m GPS module. The GPS module has been programmed by the software ublox to update its position twice a second, the arduino then communicates with the module to determine the calculated speed and displays it on the screen. 
