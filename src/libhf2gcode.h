/*
  Copyright(C) 2014 Andreas Weber <andy.weber.aw@gmail.com>

  This file is part of hf2gcode.

  hf2gcode is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  hf2gcode is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with hf2gcode.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef _LIBHF2GCODE_H_
#define _LIBHF2GCODE_H_

#include <stdio.h>

const char * get_glyph_ptr (const char *font,
                      const unsigned char c);

int init_get_gcode_line (
             char *font,   /* used font, e.g. "rowmans" or "scriptc" */
             char *text,   /* text, e.g. "Hello world!\nsecond line" */
             double X0,          /* the X-Axis offset in mm */
             double Y0,          /* the Y-Axis offset in mm */
             double Z_up,        /* the Z-Axis value in mm when it's up */
             double Z_down,      /* the Z-Axis value in mm when it's down */
             double yinc,        /* increment between to lines for multiline */
             double scale,       /* Scale factor (mm/hershey units) */
             double feed,        /* Linear feed rate in mm/min */
             int precision,      /* Precision for floating points in generated g-code */
             char verbose,       /* Verbose description in generated G-Code */
             char align,         /* Align lines l(eft) r(ight) c(enter) */
             char use_inch,      /* Use inch instead of mm as base unit */
             char no_pre,        /* Don't write preamble */
             char no_post);      /* Don't write postamble */

int get_gcode_line (
             char * buf,          /* buffer for g-code line output */
             size_t buf_len);     /* length of buf */

#endif
