/*
  Copyright(C) 2013 Andreas Weber <andy.weber.aw@gmail.com>

  This file is part of hf2gcode.

  hf2gcode is free software: you can redistribute it and/or modify
  it under the terms of the GNU General Public License as published by
  the Free Software Foundation, either version 3 of the License, or
  (at your option) any later version.

  Foobar is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
  GNU General Public License for more details.

  You should have received a copy of the GNU General Public License
  along with hf2gcode.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef AVR
  #define PROGMEM
#else
  #include <avr/pgmspace.h>
#endif

#include "../hershey_fonts/gen_c_src/rowmans.h"
//#include "../hershey_fonts/gen_c_src/scriptc.h"

const char *argp_program_version = "hf2gcode 0.1 alpha";

/*
 http://linuxcnc.org/docs/html/gcode.html

 Kommentar mit ( ) oder ; am Anfang
 G91 ( incremental programming )
 G21 ( units in mm )
 G90 ( absolute distance mode )
 G64 ( best possible speed )
 G40 ( turn off tool diameter compensation )
 G49 ( turns off tool length compensation )
 G94 ( Feed Rate Mode: Units per Minute Mode )
 G17 ( X-Y plane )
 M3 S10000

 G0 X5 Y3.2

 M5 (stop the spindle)
 M30 (Program stop, rewind to beginning of program)

 Überlegen wie man den Needler mit grbl ein- und ausschaltet.
  * Über Spindel ein-/aus? M3, M5?
  * Möglicherweise auch über Kühlmittel Pins?
  * Direction Pin der Z-Achse?
  Am Besten konfigurierbar über Args, über Z-Achse default
*/

/* ToDo:
 * %f precision of generated g-code? %f6.3 per argument?
 * optimize output, check if X or Y command is the same, then drop that
 *
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "libhf2gcode.h"

/* g-code generator init parameter */
static char *_font;
static char *_text;
static double _X0;
static double _Y0;
static double _Z_up;
static double _Z_down;
static double _scale;
static double _feed;
static int _precision;
static char _verbose;
static char _align;
static char _init;

/* get pointer to start of the glyph or NULL if not available */
const char * get_glyph_ptr (const char *font,
                      const unsigned char c)
{
  int cnt   = 0;
  const char *ptr = NULL;
  int index = 0;
  if (!strcmp(font, "rowmans"))
  {
    cnt   = rowmans_cnt;
    ptr   = rowmans;
    index = c-32;
  }
  /*
  else if (!strcmp(font, "scriptc"))
  {
    cnt   = scriptc_cnt;
    ptr   = scriptc;
    index = c-32;
  }
  */
  else
  {
#ifndef AVR
    fprintf(stderr, "ERROR: font %s is not available.\n", font);
#endif
    return NULL;
  }
  if (index<0 || index>cnt-1)
  {
#ifndef AVR
    fprintf(stderr, "ERROR: glyph number %d (ASCII 0x%X) is not available in font \"%s\".\n", index, c, font);
#endif
    return NULL;
  }

  /*search the glyph*/
  int k=0;
  while(k++<index)
  {
#ifndef AVR
    while(*(ptr++)!=0);
#else
    while(pgm_read_byte(ptr++)!=0);

    //char buf[10];
    //itoa(k,buf,10);
    //uart_puts(buf);
    //uart_putc('\n');
#endif
  }
  return ptr;
}
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
             char align)         /* Align lines l(eft) r(ight) c(enter) */
{
  _font=font;
  _text=text;
  _X0=X0;
  _Y0=Y0;
  _Z_up=Z_up;
  _Z_down=Z_down;
  _scale=scale;
  _feed=feed;
  _precision=precision;
  _verbose=verbose;
  _align=align;
  _init=0;

  /* check text (all glyphs available in font?) */
  const char *p=text;
  while(*p)
  {
    const char *tmp= get_glyph_ptr (_font,*p);
    if (!tmp)
      return -1;
    p++;
  }
  _init=1;
  return 0;
}

int get_gcode_line (
             char * buf,          /* buffer for g-code line output */
             size_t buf_len)      /* length of buf */
{
  /* g-code generator state */
  static int char_index=0;
  static char* current_glyph=0;
  static char* glyph_ptr=0;
  static double x_glyph=0;
  static double y_glyph=0;
  static enum ePenState {Up=0, Down} pen_state;
  static char pen_above_pos=0;
  static int g_line=0;
  static char left_margin=0;
  static char right_margin=0;
  static int footer_line=0;

  if (_init==0)  /* not initialized */
  {
    *buf=0;
    return -1;
  }
  else if (_init==1)
  {
    char_index  = 0;
    x_glyph     = _X0;
    y_glyph     = _Y0;
    pen_state   = Up;
    pen_above_pos = 0;
    g_line      = 0;
    left_margin = 0;
    right_margin= 0;
    footer_line = 0;
    _init       = 2;
  }
  switch(g_line)
  {
    case 0: snprintf(buf, buf_len, "( generated with %s )",argp_program_version);
    return g_line++;
    case 1: snprintf(buf, buf_len, "G21%s",_verbose? " ( units in mm )":"");
    return g_line++;
    case 2: snprintf(buf, buf_len, "G90%s",_verbose? " ( absolute distance mode )":"");
    return g_line++;
    case 3: snprintf(buf, buf_len, "G64%s",_verbose? " ( best possible speed )":"");
    return g_line++;
    case 4: snprintf(buf, buf_len, "G40%s",_verbose? " ( turn off tool diameter compensation )":"");
    return g_line++;
    case 5: snprintf(buf, buf_len, "G49%s",_verbose? " ( turns off tool length compensation )":"");
    return g_line++;
    case 6: snprintf(buf, buf_len, "G94%s",_verbose? " ( Feed Rate Mode: Units per Minute Mode )":"");
    return g_line++;
    case 7: snprintf(buf, buf_len, "G17%s",_verbose? " ( X-Y plane )":"");
    return g_line++;
    case 8: snprintf(buf, buf_len, "M3 S10000");
    return g_line++;
    case 9: if(_verbose) {snprintf(buf, buf_len, "( text=\"%s\", font=\"%s\" )",_text, _font); return g_line++;} else g_line++;
    case 10: if(_verbose) {snprintf(buf, buf_len, "( scale=%f, feed=%f )",_scale, _feed); return g_line++;} else g_line++;
    case 11: snprintf(buf, buf_len, "F%f", _feed);
    return g_line++;
    case 12: snprintf(buf, buf_len, "G0 Z%f%s",_Z_up, _verbose? " ( Pen-Up at start)":"");
    return g_line++;
    default:
      break;
  }

  if(g_line>12)
  {
    char c=_text[char_index];

    /*print state*/
    /*
     * printf("DEBUG: c=%c ci=%d cg=%s xg=%f yg=%f ps=%d pa=%d i=%d gl=%d lm=%d rm=%d fl=%d\n",
           c, char_index, glyph_ptr, x_glyph, y_glyph, pen_state, pen_above_pos, _init, g_line, left_margin, right_margin, footer_line);
    */
    if (c)
    {
      if(c=='\\' && _text[char_index+1]=='n')
      {
          x_glyph=0;
          y_glyph-=30*_scale;    /*ToDo, make param*/
          snprintf(buf, buf_len, "( Linefeed )");
          char_index+=2;
          return g_line++;
      }
      if(!current_glyph) /*load new glyph */
      {
        const char *glyph=get_glyph_ptr(_font,c);

#ifndef AVR
        current_glyph = malloc(strlen(glyph)+1);
        strcpy (current_glyph, glyph);
#else
        current_glyph = malloc(strlen_PF(glyph)+1);
        strcpy_PF (current_glyph, glyph);
#endif
        glyph_ptr=current_glyph;
        left_margin = *(glyph_ptr++)-'R';
        right_margin = *(glyph_ptr++)-'R';

        snprintf(buf, buf_len, "( %c = %s )",c, _verbose? glyph_ptr:"");
        return g_line++;
      }
      else if(!(*glyph_ptr)) /*end of glyph*/
      {
        char_index++;
        /* add glyph width to left position */
        x_glyph+=(right_margin-left_margin)*_scale;

        free(current_glyph);
        current_glyph=0;

        snprintf(buf, buf_len, "G0 Z%f%s",_Z_up, _verbose? " ( Pen-Up, EOG )":"");
        pen_state=Up;
        return g_line++;
      }
      char x=*(glyph_ptr)-'R';
      char y=*(glyph_ptr+1)-'R';

      if (x==-50 && y==0) /*Pen-Up*/
      {
        snprintf(buf, buf_len, "G0 Z%f%s",_Z_up, _verbose? " ( Pen-Up )":"");
        pen_state=Up;
        glyph_ptr+=2;
        return g_line++;
      }
      else
      {
        double gx = x_glyph+(x-left_margin)*_scale;
        double gy = y_glyph-y*_scale;
        if( pen_state == Down )
        {
          /*Linear move to position*/
          snprintf(buf, buf_len, "G1 X%f Y%f", gx, gy);
          glyph_ptr+=2;
          return g_line++;
        }
        else
        {
          if (!pen_above_pos)
          {
            /*rapid move because pen is up*/
            snprintf(buf, buf_len, "G0 X%f Y%f", gx, gy);
            glyph_ptr+=2;
            pen_above_pos=1;
            return g_line++;
          }
          else
          {
            /* lower pen*/
            snprintf(buf, buf_len, "G1 Z%f%s",_Z_down, _verbose? " ( Pen-Down )":"");
            pen_state = Down;
            pen_above_pos=0;
            return g_line++;
          }
        }
      }
    }

    /*end of text*/
    switch (footer_line++)
    {
      case 0: snprintf(buf, buf_len, "M5 %s",_verbose? "(stop the spindle)":"");
      return g_line++;
      case 1: snprintf(buf, buf_len, "M30 %s",_verbose? "(Program stop, rewind to beginning of program)":"");
      return g_line++;
      default:
        break;
    }
  }
  return -1;
}
