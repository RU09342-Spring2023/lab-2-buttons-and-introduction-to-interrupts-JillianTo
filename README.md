# Part 4: Occupancy Detection and Alert System

States and functions:
- The system when turned on blinks the green LED once every 3 seconds to show it is armed.
- When the occupancy sensor detects someone, the system moves into a "Warning" state, where the Green LED stops blinking, and the red LED Blinks once per second (500ms on, 500ms off).
- If the occupancy detector still shows someone there after 10 seconds, the system indicates this with the RED Led constantly staying on and it moves into the ALERT state.
- If the occupancy detector **before** the 10 seconds goes back to a 0, indicating the room is now empty, then the system goes back to the armed state.
- When in the ALERT State, the only way for the system to go back to the Armed state is to press the P4.1 Button.

Port 2 interrupt:
- When person is detected:
  - "detection" char is set to 0
  - P2.3 edge type is switched from Low->High to High->Low
- When person leaves detection:
  - "detection" char is set to 1
  - P2.3 edge type is switched from High->Low to Low->High

Port 4 interrupt:
- resets system to armed state and turns off red LED

Warning to Alert state transition:
In order to blink the red LED every 500ms and count the 10 seconds to go into alert, a counter increments every 500ms until it reaches 20, at which 10 seconds will have elapsed and state is switched to alert.

