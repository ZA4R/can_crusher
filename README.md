# Hardware
- Arduino R3 Uno as the microcontroller
- 700 Newton linear actuator for can crushing
- IR transmitter/receiver pair for object detection
- BTS7960 motor driver; note this may be overkill but the linear actuator I got did not come with a datasheet or max amps so I decided to be safe
- 12V power supply for actuator
- usb-C to power arduino
- breadboard, basic push-button switches, and old wire
- custom housing made of old screws and wood pieces made by yours truly


# Code
- Loop checks if the start button has been pressed or if the IR sensors detect an object; this is temporary, showing the options to my user and he will eventullay decide which he likes more (it may be awhile)
- Interrupt detects when the shutdown button has been pressed and stops the current movement of the actuator; I am currently investigating a bug with this feature where the arduino detects a shutdown button press when a signal is sent to the actuator to change directions
- the actuator can be restarted by reseting the arduino manually or (for now) pressing the start button
