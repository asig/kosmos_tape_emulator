# kosmos_tape_emulator

## What is this?
This is a small, Arduino-based Tape replacement for your Kosmos CP1. 

It reads and writes ".bin" files that are compatible with my
[Kosmos CP1 emulator](https://github.com/asig/kosmos-cp1), so you can
can develop with the ease of a modern computer and run it on the
real thing :-) 

Also, you can use the [Kosmos Tape Tool](https://github.com/asig/kosmos_tape_tool)
to transform your old audio cassettes to digital files while there still
is hardware to do this.

## What do I need to build it?
Hardware requirements are minimal. To build it, you need
- an Arduino. Pretty much any version will do, as long as it provides +5V.
- 4 momentary push buttons
- an microSD shield (ideally with a built-in voltage converter)
- an HD44780 compatible LCD display without I2C backpack. 16x2 will do

You can build it on a breadboard, if you want, but of course a perfboard
is more permanent; please let me know if you want to build a real custom
PCB, I will be very interested in that :-)

## Build instructions 
There's a Fritzing doc in the `schematics` directory, and the code contains
plenty of instructions on what to connect how.

There's also an OpenSCAD file included for a 3D-printable enclosure. It is 
somewhat ugly, but it works for me :-) Of course, you will have to adapt
it if you're using different buttons than me.

