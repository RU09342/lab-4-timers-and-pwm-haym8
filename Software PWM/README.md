## Mitchell Hay

# Software PWM
Using timers, a PWM module could be made. Starting at 50% duty cycle, the brightness of an LED increases based on button presses. When the LED is at its maximum brightness, it resets to 0. The brightness goes up 10% every button press. 

# Devices
* MSP430F5529
* MSP430FR2311
* MSP430FR5994
* MSP430FR6989
* MSP430G2553

# Getting Started
## Dependencies
Each device requires no outside dependencies. Everything needed to run the program is included in the folder. Each folder is labeled with the device it is meant for.
## Setup
Each of the device projects in this part of the lab can be imported into Code Composer Studio and run from there. To import it, open CCS and select File -> Import -> Code Composer Studio -> CCS Project and select the desired directory.

# Functionality
## Usage
The user clicks the button and the brightness of the LED increases, until it resets after its maximum brightness.
## Extra Work
Humans do not perceive brightness linearly, but rather on a logarithmic scale. Changing the brightness by 10% every time will not look linear to our eyes. To fix this, the duty cycle increments need to be changed. In this extra work, there were 20 brightness increments, each going up by a factor of 1.4141, or the square root of 2. Multiplying by this number linearizes the logarithmic relationship, and the brightness increases linearly. The brightness was incremented using a switch statement.

# Known Errors
There are no known errors at this time

# Tasks
* [x] Make software PWM program
* [x] Linearize brightness of PWM program
