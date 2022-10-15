#ifndef thermostat_pins_h
#define thermostat_pins_h

#define FURNACE_HEAT_1_PIN  D5 // GPIO14.
#define FURNACE_HEAT_2_PIN  D6 // GPIO12.
#define FURNACE_AC_PIN      D7 // GPIO13.
#define FURNACE_FAN_PIN     D8 // GPIO15. Boot fials if pulled HIGH.

#define CAP_SENSE_OUT       D3 // GPIO0. Flash button.
#define CAP_SENSE_IN        D6 //10 // GPIO10. SDD3.

#endif // thermostat_pins_h