hf2gcode, a hershey font to g-code tracer
=========================================
![demo_hello_world.ngc in LinuxCNC](http://tech-chat.de/images/demo_hello_world.png)
(The image above shows src/demo_hello_word.ngc in LinuxCNCs simulator with sim/axis/axis_mm.ini)

The work to this has begun in 17. May 2013. Until now there is
*  Some octave scripts to load, plot and render (see http://www.tech-chat.de/png_album/) hershey fonts
*  An octave script to generate C variable definitions
*  libhf2gcode the lib usable with gcc and AVR-gcc, see https://github.com/Andy1978/needler. Until now the fonts romans and scriptc are usable.
*  hf2gcode, the CLI version (pre-alpha state, some params hard-coded)

Design
------
There will be a libhf2gcode, a static linkable version which does the actual type setting/tracing
and a CLI programm hf2gcode to call it.

The libhf2gcode should also be compilable with avr-gcc and run on an AVR Atmega32.
The [Needler project](https://github.com/Andy1978/needler) will read input from a PS2 keyboard,
generate the g-code with libhf2gcode and send the generated g-code to [grbl](https://github.com/grbl/grbl)
which controlls the stepper for the mill.

The resulting g-code can be tested with LinuxCNC as simulator.
There are binaries for various Ubuntus or compile the source:
http://wiki.linuxcnc.org/cgi-bin/wiki.pl?Build_A_Simulator_Manually

License
-------
GPLv3, see file LICENSE

Dependencies
------------
*  gcc, the GNU Compiler Collection, libc
*  (OPTIONAL) GNU Octave if you want to generate the hershey font overview pngs and svgs or to generated the hershey_font C declarations.
*  (OPTIONAL) photon if you want to generate a static HTML page with rendered fonts like http://www.tech-chat.de/png_album/

Unsorted Links
--------------
*  http://en.wikipedia.org/wiki/Hershey_font
*  http://www.ghostscript.com/doc/current/Hershey.htm
*  http://emergent.unpythonic.net/software/hershey (Download of the fonts and description)
*  http://www.timeguy.com/cradek/truetype (A TrueType tracer with DXF and G-Code output)
*  http://wiki.linuxcnc.org/cgi-bin/wiki.pl?Simple_LinuxCNC_G-Code_Generators#Text_Engraving_Software
*  http://www.evilmadscientist.com/2011/hershey-text-an-inkscape-extension-for-engraving-fonts/ (Inkscape extension)
