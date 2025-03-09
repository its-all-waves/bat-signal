- Digital information is just 0 and 1 -- on / off, true / false, HIGH / LOW
    - HIGH / LOW - put simply, on / off. Is there a voltage or not (kinda)?
- Voltage is provided by a source. Current is demanded by the load.
- LEDs and push buttons MUST be used with a resistor in series, else you're shorting the voltage / power supply to ground. For LEDs, the amount of resistance chosen determines the current (and power) that passes thru the LED. For push buttons, we would ideally have zero current, as we just need to detect a change in voltage.
- Arduino / C++

```cpp
//============== PREPROCESSOR MACROS ==============
#include "OtherFile.h"     // Include local header file
#include <LibraryName.h>   // Include external library
#define MAX_SENSORS 5      // Maximum number of sensors
  
//============== ARDUINO PROGRAM FUNCTIONS =============
void setup();  // Runs once at start of execution
void loop();  // Runs forever -- is the program
  
//============== SERIAL COMMUNICATION ==============
// Initialize serial communication
Serial.begin(uint32_t baudRate);  // baudRate: 9600, 115200 etc.
  
// Output to serial monitor
Serial.print(T value);            // Print value (any type) without newline
Serial.println(T value);          // Print value (any type) with newline
Serial.printf(const char* format, ...);  // Formatted print (like printf)
  
//============== DIGITAL I/O ==============
// Set pin mode
pinMode(uint8_t pin, uint8_t mode);  // mode: INPUT/OUTPUT
pinMode(BAT_SIGNAL_PIN, OUTPUT);
  
// Write digital value to pin
digitalWrite(uint8_t pin, uint8_t state);  // state: HIGH/LOW (1/0)
// Example
digitalWrite(LED_PIN, HIGH);
  
// Read digital value from pin
uint8_t digitalRead(uint8_t pin);  // Returns: HIGH or LOW
// Example
boolean value = digitalRead(BUTTON_PIN);
  
//============== TIMING ==============
// Pause program execution
delay(uint32_t milliseconds);  // Blocks code execution
  
//============== INTERRUPTS ==============
// Attach interrupt to a pin
attachInterrupt(
    uint8_t interrupt,      // Use digitalPinToInterrupt(pin) to convert pin number
    void (*ISR)(void),      // Function pointer to Interrupt Service Routine
    uint8_t mode            // RISING, FALLING, CHANGE, LOW, or HIGH
);
  
// Helper function to convert pin number to interrupt number
uint8_t digitalPinToInterrupt(uint8_t pin);
```

## Usable Pins on the MCU Board
[NodeMCU 1.0 usable pins](https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/#table)