// pins.h — GPIO pin map for Edu-Car.
// Every pin used in the project must be declared here only.
// To change a pin, edit this file only, never the logic code.

#pragma once
// Prevents double inclusion: if multiple .cpp files #include "pins.h",
// the compiler only reads this file once, avoiding "redefinition" errors.

// constexpr = a constant known at compile time.
// Unlike a regular variable: uses no RAM, can't be reassigned (compile error if you try).
// Unlike #define: has a real type (int), so the compiler can catch type errors.

constexpr int PIN_LED_ONBOARD = 2;   // Onboard LED (GPIO 2)
constexpr int PIN_LED         = 23;  // External LED on breadboard, via 220Ω resistor
constexpr int PIN_BUTTON      = 18;  // Push button, wired to GND, uses INPUT_PULLUP
constexpr int PIN_BUTTON_BOOT = 0;   // Onboard BOOT button (GPIO 0)