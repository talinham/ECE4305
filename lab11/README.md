# Lab 11: Keyboard or mouse controlled chasing led (Group 10)
lab-11-keyboard-or-mouse-controlled-chasing-led-group10 created by GitHub Classroom

Consider the Chasing LEDs experiment (Lab 6). Instead of switches and buttons, we can use the 
keyboard to set the speed and the pause operation (i.e. moving the LED to the rightmost position) 
- Use the P (for “pause) key to pause and resume the chasing operation 
- Use the following key sequence to enter the desired flashing period: F1 and then three-digit keys (i.e. 000 to 999) 
- All other keys or illegal sequences should be ignored 
<br></br>

If your keyboard doesn’t support Boot HID, use a USB mouse instead to implement a similar 
functionality: 
- Use the left mouse button to pause and resume the flashing operating. 
- Use right mouse button to enter a new speed. Use the value of x to increment or decrement the period, then press the right mouse button again to resume chasing with the new speed. 
<br></br>

Whether you use a Mouse or a Keyboard, use the seven-segment display to show the status of the 
system: 
- Display the letter “P.” on the left most seven-segment (segment: 7) whenever the system is in a pause state 
- Display the letters “SP:” in the seven-segments (3, 4, 5) 
- Display the system’s speed (000 to 999) on the right most seven-segment (segments: 0, 1, 2) 
- The image below shows an example of a paused system with a speed of 834 
<br></br>
<p align="left"><img src="https://github.com/aseddin-teaching/lab-11-keyboard-or-mouse-controlled-chasing-led-group10/blob/main/system_example.png" width="700"></p>

