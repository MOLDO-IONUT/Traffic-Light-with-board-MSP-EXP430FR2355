# Traffic-Light-with-board-MSP-EXP430FR2355
This project represents a traffic light made using the MSP-EXP430FR2355 board.

Necessary components:
-board development MSP-EXP430FR2355
-LED RED: 2 
-LED YELLOW: 1
-LED GREEN: 2 
-BUZZER
-7 SEGMENT
-wire cable
-USB-A to micro-USB cable
-resistors

It simulates a traffic light intersection consisting of 3 LEDs: red, yellow, green for cars and 2 for pedestrians: red, green, a buzzer that emits a sound signal for 10 seconds, representing the signal for blind people.
To display the traffic light type, we used a 7 SEGMENT type display.
For the complexity of the project, I used the UART module to transmit the time and status of both traffic lights (cars/pedestrians) and these data were displayed on the PC using the TeraTerm app
