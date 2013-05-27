hf2gcode, a hershey font to g-code tracer
=========================================
![demo_hello_world.ngc in LinuxCNC](http://tech-chat.de/images/demo_hello_world.png)
(The image above shows src/demo_hello_word.ngc in LinuxCNCs simulator with sim/axis/axis_mm.ini)

The current beta release is "hf2gcode 0.1",
please see topic "Bugs or not yet implemented features" below before you report a bug.

Available fonts in "hf2gcode 0.1":
[cursive](http://www.tech-chat.de/hf/cursive.svg),
[futural](http://www.tech-chat.de/hf/futural.svg),
[futuram](http://www.tech-chat.de/hf/futuram.svg),
[gothgbt](http://www.tech-chat.de/hf/gothgbt.svg),
[gothgrt](http://www.tech-chat.de/hf/gothgrt.svg),
[gothiceng](http://www.tech-chat.de/hf/gothiceng.svg),
[gothicger](http://www.tech-chat.de/hf/gothicger.svg),
[gothicita](http://www.tech-chat.de/hf/gothicita.svg),
[gothitt](http://www.tech-chat.de/hf/gothitt.svg),
[greekc](http://www.tech-chat.de/hf/greekc.svg),
[greek](http://www.tech-chat.de/hf/greek.svg),
[greeks](http://www.tech-chat.de/hf/greeks.svg),
[rowmand](http://www.tech-chat.de/hf/rowmand.svg),
[rowmans](http://www.tech-chat.de/hf/rowmans.svg),
[rowmant](http://www.tech-chat.de/hf/rowmant.svg),
[scriptc](http://www.tech-chat.de/hf/scriptc.svg),
[scripts](http://www.tech-chat.de/hf/scripts.svg),
[symbolic](http://www.tech-chat.de/hf/symbolic.svg),
[timesg](http://www.tech-chat.de/hf/timesg.svg),
[timesib](http://www.tech-chat.de/hf/timesib.svg),
[timesi](http://www.tech-chat.de/hf/timesi.svg),
[timesrb](http://www.tech-chat.de/hf/timesrb.svg),
[timesr](http://www.tech-chat.de/hf/timesr.svg)

Overview on http://www.tech-chat.de/png_album/

Dependencies
------------
*  gcc, the GNU Compiler Collection, libc
*  (OPTIONAL) GNU Octave if you want to generate the hershey font overview pngs and svgs or to generated the hershey_font C declarations.
*  (OPTIONAL) photon if you want to generate a static HTML page with rendered fonts like http://www.tech-chat.de/png_album/

Build
-----
    cd src
    make

Usage
-----
    ./hf2gcode --usage
    Usage: hf2gcode [-cilmqr?V] [-d ZDown] [-f FEED] [-h FONT] [-n YINC] [-o FILE]
                [-p PREC] [-s SCALE] [-u ZUp] [-x X0] [-y Y0] [--align-center]
                [--z-down=ZDown] [--feed=FEED] [--font=FONT] [--inch]
                [--align-left] [--min-gcode] [--interline=YINC] [--output=FILE]
                [--precision=PREC] [--quiet] [--align-right] [--scale=SCALE]
                [--z-up=ZUp] [--xoffset=X0] [--yoffset=Y0] [--help] [--usage]
                [--version] TEXT

Help
----
    ./hf2gcode --help
    Usage: hf2gcode [OPTION...] TEXT
    hf2gcode, a hershey font to g-code tracer

      -c, --align-center         Center multiple lines
      -d, --z-down=ZDown         Pen-Down Z value (default -1)
      -f, --feed=FEED            Feed rate (default 200)
      -h, --font=FONT            Use FONT instead of default font "rowmans"
      -i, --inch                 Use inch as base unit (default mm)
      -l, --align-left           Left align multiple lines (default)
      -m, --min-gcode            Generate minimalistic g-code, suppress comments
      -n, --interline=YINC       Interline spacing in Y direction for multiple
                                 lines (default 30)
      -o, --output=FILE          Output to FILE instead of standard output
      -p, --precision=PREC       Precision for G-Code generation (default 3)
      -q, --quiet                Don't produce any output
      -r, --align-right          Right align multiple lines
      -s, --scale=SCALE          Base unit/hershey font unit (default 0.5)
      -u, --z-up=ZUp             Pen-Up Z value (default 1)
      -x, --xoffset=X0           X-Axis offset (default 0)
      -y, --yoffset=Y0           Y-Axis offset (default 0)
      -?, --help                 Give this help list
          --usage                Give a short usage message
      -V, --version              Print program version

Example call of hf2gcode
------------------------
    ./hf2gcode "Hello world!\nThis is hf2gcode." -o demo_hello_world_scriptc.ngc -y 23 --font=scriptc
    
demo_hello_world_scriptc.ngc can then be loaded e.g. into LinuxCNC and looks like the image at the top of the README.

Design and structure
--------------------
*  hershey_fonts/orig/*.jhf: original hershey fonts, see hershey_fonts/README
*  hershey_fonts/fixed/*.jhf: generated with fix_linebreak.sh from orig
*  OPTIONAL hershey_fonts/rendered_png and rendered_svg: All fonts as PNG or SVG, generated from src/render_hf (see http://www.tech-chat.de/png_album/)
*  hershey_fonts/gen_c_src/*.h: Font declarations, generated with src/gen_C_code.m
*  src/libhf2gcode.c and .h: Actual type setting/tracing
*  src/hf2gcode.c: A command line interface program to call libhf2gcode

The libhf2gcode should also be compilable with avr-gcc and run on an AVR Atmega32. Only font "rowmans" is available if
the MACRO AVR is set. The [Needler project](https://github.com/Andy1978/needler) will read input from a PS2 keyboard,
generate the g-code with libhf2gcode and send the generated g-code to [grbl](https://github.com/grbl/grbl)
which controlls the stepper for the CNC mill.

Testing the generated output
----------------------------

The resulting g-code can be tested with LinuxCNC as simulator or any other software which reads g-code.
There are LinuxCNC simulator binaries for various Ubuntus (IMHO outdated) or you might compile the source:

http://wiki.linuxcnc.org/cgi-bin/wiki.pl?Build_A_Simulator_Manually

Short summary for Debian 7.0 wheezy:

      git clone git://git.linuxcnc.org/git/linuxcnc.git linuxcnc
      cd linuxcnc/debian && ./configure sim
      cd .. && dpkg-checkbuilddeps

Install the dependencies with apt-get or aptitude

      cd src && ./autogen.sh && ./configure --enable-simulator
      make

Then run ./linuxcnc in linuxcnc/scripts and select a simulated machinge e.g. sim/axis/axis_mm.ini

Bugs or not yet implemented features
------------------------------------

Switching the base unit from "mm" (default) to inch has not yet an effect in the generated output.

No optimal small g-code generation: the X or Y value for previous set values can be omitted. You will see output like

     G1 X5 Y5
     G1 X5 Y10
which could be written as

     G1 X5 Y5
     G1 Y10

No multiline align-right or align-center, align-left will be used.

License
-------
GPLv3, see file LICENSE

Unsorted Links
--------------
*  http://en.wikipedia.org/wiki/Hershey_font
*  http://www.ghostscript.com/doc/current/Hershey.htm
*  http://emergent.unpythonic.net/software/hershey (Download of the fonts and description)
*  http://www.timeguy.com/cradek/truetype (A TrueType tracer with DXF and G-Code output)
*  http://wiki.linuxcnc.org/cgi-bin/wiki.pl?Simple_LinuxCNC_G-Code_Generators#Text_Engraving_Software
*  http://www.evilmadscientist.com/2011/hershey-text-an-inkscape-extension-for-engraving-fonts/ (Inkscape extension)
