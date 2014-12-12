hf2gcode, a hershey font to g-code tracer
=========================================
![demo_hello_world.ngc in LinuxCNC](http://tech-chat.de/images/demo_hello_world.png)
(The image above shows src/demo_hello_word.ngc in LinuxCNCs simulator with sim/axis/axis_mm.ini)

The current release is "hf2gcode 0.2.1",
please see topic "Bugs" or "TODO" below before you report a bug.

Available fonts in "hf2gcode 0.2.1":
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
*  [GNU argp](http://www.gnu.org/software/libc/manual/html_node/Argp.html)
*  (OPTIONAL) GNU Octave if you want to generate the hershey font overview pngs and svgs or to generated the hershey_font C declarations.
*  (OPTIONAL) photon if you want to generate a static HTML page with rendered fonts like http://www.tech-chat.de/png_album/

Build
-----
    cd src
    make

Usage
-----
    ./hf2gcode --usage
    Usage: hf2gcode [-aclrmqu?V] [-h FONT] [-i FILE] [-o FILE] [-s SCALE]
            [-f FEED] [-x X0] [-y Y0] [-n YINC] [-p PREC] [--append]
            [--font=FONT] [--input=FILE] [--output=FILE] [--scale=SCALE]
            [--feed=FEED] [--xoffset=X0] [--yoffset=Y0] [--z-down=ZDown]
            [--z-up=ZUp] [--align-center] [--align-left] [--interline=YINC]
            [--align-right] [--min-gcode] [--no-post] [--no-pre]
            [--precision=PREC] [--quiet] [--inch] [--help] [--usage]
            [--version] [TEXT]


Help
----
      ./hf2gcode --help
      Usage: hf2gcode [OPTION...] [TEXT]
      hf2gcode, a hershey font to g-code tracer

        -a, --append               Append to output file instead of overwriting
        -h, --font=FONT            Use FONT instead of default font "rowmans"
        -i, --input=FILE           Read text from FILE instead of stdin
        -o, --output=FILE          Output to FILE instead of stdout
        -s, --scale=SCALE          Base unit/hershey font unit (default 0.5)

       G-code base settings:
        -f, --feed=FEED            Feed rate (default 200)
        -x, --xoffset=X0           X-Axis offset (default 0)
        -y, --yoffset=Y0           Y-Axis offset (default 0)
            --z-down=ZDown         PenDown Z value (default -1)
            --z-up=ZUp             PenUp Z value (default 1)

       Multiline settings:
        -c, --align-center         Center multiple lines
        -l, --align-left           Left align multiple lines (default)
        -n, --interline=YINC       Interline spacing in Y direction for multiple
                                   lines (default 15)
        -r, --align-right          Right align multiple lines

       Miscellaneous:
        -m, --min-gcode            Generate minimalistic g-code, suppress comments
            --no-post              Don't include postamble
            --no-pre               Don't include preamble
        -p, --precision=PREC       Precision for G-Code generation (default 3)
        -q, --quiet                Don't produce any output to stdout
        -u, --inch                 Use United States customary units (inch instead of
                                   mm) as base unit

        -?, --help                 Give this help list
            --usage                Give a short usage message
        -V, --version              Print program version

      Mandatory or optional arguments to long options are also mandatory or optional
      for any corresponding short options.

      Report bugs to <andy.weber.aw@gmail.com>.

Design and structure
--------------------
*  hershey_fonts/orig/*.jhf: original hershey fonts, see hershey_fonts/README
*  hershey_fonts/fixed/*.jhf: generated with fix_linebreak.sh from orig
*  OPTIONAL hershey_fonts/rendered_png and rendered_svg: All fonts as PNG or SVG, generated from src/render_hf (see http://www.tech-chat.de/png_album/)
*  hershey_fonts/gen_c_src/*.h: Font declarations, generated with src/gen_C_code.m
*  src/libhf2gcode.c and .h: Actual type setting/tracing
*  src/hf2gcode.c: A command line interface program to call libhf2gcode

The libhf2gcode should also be compilable with avr-gcc and run on an AVR Atmega32. Only font "rowmans" and "scripts" are available if
the MACRO AVR is set. The [Needler project](https://github.com/Andy1978/needler) reads user input from 8x8 matrix keyboard,
generates the g-code with libhf2gcode and sends the generated g-code to [grbl](https://github.com/grbl/grbl)
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

TODO / not yet implemented features
------------------------------------

*  Multiline align-right or align-center ist not yet implemented, please use the default "align-left" instead.

*  Don't lift the pen if the start position of the next glyph is the same as the end position of the current. For example see a text which uses font "scripts".

*  German "Umlaute" äöüß weren't implemented yet.

Bugs
----
Zarro Boogs Found

License
-------
GPLv3, see file LICENSE

Unsorted Links
--------------
*  http://en.wikipedia.org/wiki/Hershey_font
*  http://www.ghostscript.com/doc/current/Hershey.htm
*  http://emergent.unpythonic.net/software/hershey (Download of the fonts and description)
*  http://www.linuxcnc.org/docs/html/gcode.html
