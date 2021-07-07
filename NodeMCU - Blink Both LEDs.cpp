/*
 * Blink - RCS Academy - Lord
 * Turns on an LED on for one second,
 * then off for one second, repeatedly.
 * Feel free to experiment by changing the delay amount!
 * NodeMCU has following LED pins,
 * CH340              | CP2102
 * --------------------------------------
 * LED_BUILTIN (D4)   | LED_BUILTIN (D4)
 *                    | D0
 * --------------------------------------
 */

#include <Arduino.h>

void setup() // Code below this function runs only once.
{
  // Initialize both of the on-board LEDs, set respective pins as output.
  pinMode(D4, OUTPUT); // Instead of D4 (Digital Pin 4) we could use the word LED_BUILTIN.
  pinMode(D0, OUTPUT); // Only the CP2102 Board has this pin.
}

void loop() // Code below this function runs again and again.
{
  // Turn one LED on, the other LED off. HIGH means give it 3.3 volts, LOW means give it 0 volts.
  digitalWrite(D4, HIGH);
  digitalWrite(D0, LOW); // Only the CP2102 Board has this LED.
  // Wait for one second, in this case, 1000 miliseconds.
  delay(1000);
  // Turn one LED on, the other LED off. HIGH means give it 3.3 volts, LOW means give it 0 volts.
  digitalWrite(D4, LOW);
  digitalWrite(D0, HIGH); // Only the CP2102 Board has this LED.
   // Wait for one second, in this case, 1000 miliseconds.
  delay(1000);
}
