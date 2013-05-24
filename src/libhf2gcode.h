#ifndef _LIBHF2GCODE_H_
#define _LIBHF2GCODE_H_

#include <stdio.h>

char * get_glyph_ptr (const char *font,
                      const unsigned char c);

int init_get_gcode_line (
             char *font,   /* used font, e.g. "rowmans" or "scriptc" */
             char *text,   /* text, e.g. "Hello world!\nsecond line" */
             double X0,          /* the X-Axis offset in mm */
             double Y0,          /* the Y-Axis offset in mm */
             double Z_up,        /* the Z-Axis value in mm when it's up */
             double Z_down,      /* the Z-Axis value in mm when it's down */
             double scale,       /* Scale factor (mm/hershey units) */
             double feed,        /* Linear feed rate in mm/min */
             int precision,
             char verbose,       /* Verbose description in generated G-Code */
             char align);        /* Align lines l(eft) r(ight) c(enter) */

int get_gcode_line (
             char * buf,          /* buffer for g-code line output */
             size_t buf_len);     /* length of buf */

#endif
