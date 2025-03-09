1. [NOTES.md](./docs/NOTES.md)
2. [TROUBLESHOOTING.md](./docs/TROUBLESHOOTING.md)

## Start
1. [CREATE ACCOUNT](https://login.arduino.cc/login?state=hKFo2SBTRG5CSXJOM2VPTDdjLWxTNTg4VVJGamw5MEp0QXNXV6FupWxvZ2luo3RpZNkgUWlXR29qWGN1V1d5MFZrRm9zM2ZyMWdMM09ZVnNJeUOjY2lk2SBkdHFwT1VlMDVxckx2YkdST0lXWWkyeG80M1ZQMFVyMw&client=dtqpOUe05qrLvbGROIWYi2xo43VP0Ur3&protocol=oauth2&authorizeTimeoutInSeconds=5&redirect_uri=https%3A%2F%2Fid.arduino.cc&scope=openid+profile+email&response_type=code&response_mode=query&nonce=WHhyU01IX1B5b2FEWndYb0cya3FqZUZWSkVUaHF0dlR2OEpIVjVETjJ1Vg%3D%3D&code_challenge=1RLR5WQYHOI3w_bo5ntv-eXzjcTkUcG31N1BDX3hS3c&code_challenge_method=S256&auth0Client=eyJuYW1lIjoiYXV0aDAtc3BhLWpzIiwidmVyc2lvbiI6IjEuMTIuMSJ9#/sso/login)
2. [ARDUINO ESP32 DOCS](https://docs.arduino.cc/arduino-cloud/guides/esp32/)
3. TOP RIGHT BROWSER:
	1. Go to Arduino Cloud -> 

![Pasted image 20250307183536](https://github.com/user-attachments/assets/fd8fd4f4-6fad-47a0-a4a9-e253cb0cd2e3)

4. -> DEVICES ON LEFT NAV BAR
5. -> CLICK 

![Pasted image 20250307183801](https://github.com/user-attachments/assets/a7b1004f-47f2-462e-9d48-c48f775d7519)

6. -> SELECT THIRD PARTY DEVICE
7. -> SETUP DEVICE MODAL
	1. ESP8266 SELECT
	2. NodeMCU 1.0 (ESP-12E Module)
	3. Continue...
8. -> GIVE YOUR DEVICE A NAME
	1. NEXT
9. *IMPORTANT*
	1. -> SAVE ID
	2. -> SAVE SECRET KEY
	3. -> (DOWNLOAD PDF)
	4. CONFIRM SAVED + CONTINUE
10. DONE
## Create a Thing
1. THING == VIRUTAL TWIN OF HARDWARE
	1. FOR SYHNCING BETWEEN CLOUD AND THE BOARD
2. THINGS (left NAVBAR)
	1. -> CLICK CREATE A THING
3. -> ASSOCIATED DEVICE TOP RIGHT
4. -> CLICK THE LINK ICON
	1. MODAL: -> SELECT THE DEVICE YOU NAMED IN PREV. STEP
## Create a Cloud Variable
1. CLICK "ADD" Middle Top
2. "Add Variable" MODAL
	1. -> ENTER A NAME
		1. This will be a BOOLEAN VARIABLE in your CODE
	2. LEAVE EVERYTHING DEFAULT*
	3. -> CLICK "Add Variable" BUTTON
## Code (Sketch)
1. `.ino` is your `Main` program file
## Install Agent
1. -> NAVIGATE BACK TO YOUR WEB BROWSER
2. -> DOWNLOAD cloud.arduino.cc/download-agent
3. -> NAVIGATE TO THE [DOCS](https://docs.arduino.cc/arduino-cloud/guides/esp32/)
4. FOLLOW COMPILE & UPLOAD STEP
## Back to Things
1. SETUP NETWORK
2. NAVIGATE TO THINGS (LEFT SIDE NAV BAR) in CLOUD INTERFACE (not dashboard?)
3. -> CLICK ON YOUR THING
4. -> RIGHT NAVBAR UNDER "NETWORK"
	1. CLICK "LINK" CONFIGURE/CHANGE ICON
5. -> HARDCODE WIFI NETWORK AND PASSWORD INTO DEVICE
## MCU BOARD PLACEMENT
1. PUT THE BOARD ON THE BREADBOARD WITH THE USB ALIGNED WITH THE SIDE OF THE BREADBOARD WHERE THE BREADBOARD NUMBERS ARE THE HIGHEST
## Hello World
1. STILL INSIDE THE THING
2. OPEN SKETCH TAB (TOP RIGHT)
3. FAR LEFT VERTICAL NAV BAR
	1. CLICK EXAMPLES BUTTON
	2. SEARCH FOR "BLINK"
	3. CLICK "BLINK"
4. SELECT BOARD TOP CENTER ("same as last time"?)
	1. -> BOARD NAME: "NodeMCU 1.0"
5.  -> TOP LEFT CLICK the ARROW ->
	1. "UPLOAD" BUTTON
## Code (Sketch) (READY TO DEVELOP)
1. BACK TO "THING"
2. **CODE**
## MILESTONE ONE:
**GOAL: CREATEA CLOUD DASHBOARD GUI, ADD BUTTON, ADD BOOL VARIABLE** 
1. *IN CLOUD INTERFACE*
	- -> *GO TO DASHBOARDS* (left side navbar)
	- -> ADD A DASHBOARD
2. *INSIDE NEW DASHBOARD*
	- -> NAME YOUR DASHBOARD
	- -> CLICK "EDIT" BUTTON TOP RIGHT
	- -> CLICK "ADD" BUTTON TOP CENTER
	- -> SELECT THE "SWITCH"
3. *LINK A VARIABLE*
	- -> NAME IT (e.g. `bool isOn`)
	- -> SELECT IT (`isOn`)
	- -> CLICK "DONE"
*TIME TO GET LED LIGHT WORKING*
## MILESTONE TWO:
**GOAL: TRIGGER AN LED LIGHT ON/OFF VIA THE CLOUD DASHBOARD**
- HARDWARE SPECS:
	- 1 LED
	- 1 RESISTOR - VALUE: 200 - 300 ohms
1. *FROM PIN WHERE CONNECTING LED*...
	1. RESISTOR AND LED IN SERIES GO INTO GROUND
		- (THE LED IS POLARIZED SO CURRENT CAN ONLY FLOW IN ONE DIRECTION) (a "DIODE")
		- (IF YOU HAVE LED BACKWARDS IT WILL NOT WORK)
		- (LONG SIDE PIN IS POSITIVE VOLTAGE SIDE)
		- (SHORT SIDE WILL GO TO GROUND)
- **CODE**
	- TODO: `SET THE MODE OF The PIN WE ARE USING IN THE SETUP FUNCTION`
	- `USE EVENT LISTENER TO LISTEN TO EVENTS FROM CLOUD AND RESPOND TO THEM`
		- (IF `isOn` (your `bool` variable) is TRUE turn on LED, ELSE turn off)
## MILESTONE THREE:
**GOAL: ADD A RELAY AND ITS POWER SUPPLY**
 > (SHOW DIAGRAM)
1. _CONNECT WIRES_:
	1. BREADBOARD-RAIL-GROUND GOES TO RELAY-GROUND
	2. BREADBOARD-POSITIVE GOES TO RELAY-VCC
	3. DATA PIN GOES INTO RELAY "IN" CONNECTION
	    1. OUR DATA PIN IS `D6` (`D6` CONTROLS THE RELAY)
**CONNECTION IS NOW ESTABLISHED (RELAY IS CONNECTED TO THE BOARD)**
2. *GRAB A 9V BATTERY AND ATTACH IT TO THE BATTERY ADAPTER*
    1. PLUG IT INTO THE POWER SUPPLY
    2. (double check +/- going into relay correctly)
3. *TURN ON THE POWER SUPPLY WITH PHYSICAL BUTTON ON IT*
	1. (CONFIRM YOU HEAR A CLICKING SOUND)
4. `CODE`
    1. TODO: `EDIT EVENT HANDLER TO TRIGGER THE RELAY`
## MILESTONE FOUR:
**GOAL: SPLICE THE RELAY INTO THE LAMP CIRCUIT**
1. _MAKE A 2"-3" SLICE HALF WAY INTO THE LAMP POWER CORD JACKET_:
	- (BETWEEN POWER SWITCH AND USB CONNECTOR)
	- (DON'T CUT INTO WIRE ONLY PLASTIC JACKET)
2. *PULL OUT RED WIRE*
	1. CUT THIS WIRE IN THE MIDDLE (in half)
	2. STRIP 1/4" OF JACKET AWAY ON EACH END
3. *INSERT THE NEW EXPOSED ENDS OF RED WIRE INTO RELAY*
	1. ONE INTO "ON"
	2. ONE INTO "COMMON"
**PLUG THE LAMP INTO POWER**
4. *CONFIRM THE LAMP POWERS ON WHEN RELAY POWERS ON*
## Physical Tofu **Space**
1.



## Prerequisite Knowledge
- Digital information is just 0 and 1 -- on / off, true / false, HIGH / LOW
	- HIGH / LOW - put simply, on / off. Is there a voltage or not (kinda)?
- Voltage is provided by a source. Current is demanded by the load. 
- LEDs and push buttons MUST be used with a resistor in series, else you're shorting the voltage / power supply to ground. For LEDs, the amount of resistance chosen determines the current (and power) that passes thru the LED. For push buttons, we would ideally have zero current, as we just need to detect a change in voltage.
- Arduino / C++
```cpp
//============== PREPROCESSOR MACROS ==============
#include "OtherFile.h"     // Include local header file
#include <LibraryName.h>   // Include external library
#define MAX_SENSORS 5      // Maximum number of sensors

//============== ARDUINO PROGRAM FUNCTIONS =============
void setup();  // Runs once at start of execution
void loop();  // Runs forever -- is the program

//============== SERIAL COMMUNICATION ==============
// Initialize serial communication
Serial.begin(uint32_t baudRate);  // baudRate: 9600, 115200 etc.

// Output to serial monitor
Serial.print(T value);            // Print value (any type) without newline
Serial.println(T value);          // Print value (any type) with newline
Serial.printf(const char* format, ...);  // Formatted print (like printf)

//============== DIGITAL I/O ==============
// Set pin mode
pinMode(uint8_t pin, uint8_t mode);  // mode: INPUT/OUTPUT
pinMode(BAT_SIGNAL_PIN, OUTPUT);

// Write digital value to pin
digitalWrite(uint8_t pin, uint8_t state);  // state: HIGH/LOW (1/0)
// Example
digitalWrite(LED_PIN, HIGH);

// Read digital value from pin
uint8_t digitalRead(uint8_t pin);  // Returns: HIGH or LOW
// Example
boolean value = digitalRead(BUTTON_PIN);

//============== TIMING ==============
// Pause program execution
delay(uint32_t milliseconds);  // Blocks code execution

//============== INTERRUPTS ==============
// Attach interrupt to a pin
attachInterrupt(
    uint8_t interrupt,      // Use digitalPinToInterrupt(pin) to convert pin number
    void (*ISR)(void),      // Function pointer to Interrupt Service Routine
    uint8_t mode            // RISING, FALLING, CHANGE, LOW, or HIGH
);

// Helper function to convert pin number to interrupt number
uint8_t digitalPinToInterrupt(uint8_t pin);
```
## Usable Pins on the MCU Board
[NodeMCU 1.0 usable pins](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/#table)
