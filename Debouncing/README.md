## Mitchell Hay

# Debouncing
A problem that has arisen in previous labs is something called button bouncing. This is when a button is pressed and the initial contact is made between the button and the base, and the button "bounces" for a small amount of time. This led to some unexpected results when doing things like blinking an LED. This can be corrected by a process called debouncing. With debouncing, the button interrupt flag will not be raised more than it should be. Using timer and button interrupts together corrects this debouncing problem and is illustrated in the lab.

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
The user clicks the button and holds it down to determine the new rate of the button blink. The longer the button is held, the longer the period of one cycle is.
## Extra Work
The extra work for this lab was to add a second button and enable that button for debouncing. This was done using only one timer, and was very similar to the original part of this lab. The board used for the extra work was the MSP430F5529 because it has multiple buttons and was capable of running the program without running out of memory. 

# Known Errors
There are no known errors at this time

# Tasks
* [x] Learn what debouncing is and how it affects a circuit
* [x] Use timer and button interrupts together
