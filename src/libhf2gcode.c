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

#ifndef AVR
  #define PROGMEM
#else
  #include <avr/pgmspace.h>
#endif

#include "../hershey_fonts/gen_c_src/rowmans.h"
#include "../hershey_fonts/gen_c_src/scripts.h"

#ifndef AVR
  #include "../hershey_fonts/gen_c_src/cursive.h"
  #include "../hershey_fonts/gen_c_src/futural.h"
  #include "../hershey_fonts/gen_c_src/futuram.h"
  #include "../hershey_fonts/gen_c_src/gothgbt.h"
  #include "../hershey_fonts/gen_c_src/gothgrt.h"
  #include "../hershey_fonts/gen_c_src/gothiceng.h"
  #include "../hershey_fonts/gen_c_src/gothicger.h"
  #include "../hershey_fonts/gen_c_src/gothicita.h"
  #include "../hershey_fonts/gen_c_src/gothitt.h"
  #include "../hershey_fonts/gen_c_src/greekc.h"
  #include "../hershey_fonts/gen_c_src/greek.h"
  #include "../hershey_fonts/gen_c_src/greeks.h"
  #include "../hershey_fonts/gen_c_src/rowmand.h"
  #include "../hershey_fonts/gen_c_src/rowmant.h"
  #include "../hershey_fonts/gen_c_src/scriptc.h"
  #include "../hershey_fonts/gen_c_src/symbolic.h"
  #include "../hershey_fonts/gen_c_src/timesg.h"
  #include "../hershey_fonts/gen_c_src/timesib.h"
  #include "../hershey_fonts/gen_c_src/timesi.h"
  #include "../hershey_fonts/gen_c_src/timesrb.h"
  #include "../hershey_fonts/gen_c_src/timesr.h"
#endif

#define FONT_TABLE(x) if (!strcmp(font, #x)) \
  { cnt   = x##_cnt; \
    ptr   = x;     \
    index = c-32;} /*for ASCII fonts, will not work with e.g. japanese*/

const char *argp_program_version = "hf2gcode 0.2.2";

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
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
static char _use_inch;
static char _init;
static int _yinc;
static char _no_pre;
static char _no_post;
static double _X, _Y, _Z;

/* get pointer to start of the glyph or NULL if not available */
const char * get_glyph_ptr (const char *font,
                      const unsigned char c)
{
  int cnt   = 0;
  const char *ptr = NULL;
  int index = 0;

  FONT_TABLE(rowmans)
  else FONT_TABLE(scripts)
#ifndef AVR
  else FONT_TABLE(cursive)
  else FONT_TABLE(futural)
  else FONT_TABLE(futuram)
  else FONT_TABLE(gothgbt)
  else FONT_TABLE(gothgrt)
  else FONT_TABLE(gothiceng)
  else FONT_TABLE(gothicger)
  else FONT_TABLE(gothicita)
  else FONT_TABLE(gothitt)
  else FONT_TABLE(greekc)
  else FONT_TABLE(greek)
  else FONT_TABLE(greeks)
  else FONT_TABLE(rowmand)
  else FONT_TABLE(rowmant)
  else FONT_TABLE(scriptc)
  else FONT_TABLE(symbolic)
  else FONT_TABLE(timesg)
  else FONT_TABLE(timesib)
  else FONT_TABLE(timesi)
  else FONT_TABLE(timesrb)
  else FONT_TABLE(timesr)
#endif
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
             double yinc,        /* increment between to lines for multiline */
             double scale,       /* Scale factor (mm/hershey units) */
             double feed,        /* Linear feed rate in mm/min */
             int precision,      /* Precision for floating points in generated g-code */
             char verbose,       /* Verbose description in generated G-Code */
             char align,         /* Align lines l(eft) r(ight) c(enter) */
             char use_inch,      /* Use inch instead of mm as base unit */
             char no_pre,        /* Don't write header, output only movement */
             char no_post)       /* Don't write tail */
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
  _no_pre=no_pre;
  _no_post=no_post;
  _align=align;
  _use_inch=use_inch;
  _yinc=yinc;
  _init=0;
  _X=-1e20;
  _Y=-1e20;
  _Z=-1e20;

  /* check text (all glyphs available in font?) */
  const char *p=text;
  while(*p)
  {
    /* do not check linefeed */
    while(*p=='\n') p++;
    if(*p)
    {
      const char *tmp= get_glyph_ptr (_font,*p);
      if (!tmp)
        return -1;
      p++;
    }
  }
  _init=1;
  return 0;
}

#ifndef FIXED_PRECISION
void g_move(char * buf, size_t buf_len, int type, double gx, double gy)
{
  int cnt;
  if(gx!=_X || gy!= _Y)
  {
    snprintf(buf, buf_len, "G%d%n", type, &cnt);
    buf+=cnt;
    buf_len-=cnt;
  }
  else
    *buf=0;
  if (gx != _X)
  {
    snprintf(buf, buf_len, " X%.*f%n", _precision, gx, &cnt);
    buf+=cnt;
    buf_len-=cnt;
  }
  if (gy != _Y)
    snprintf(buf, buf_len, " Y%.*f", _precision, gy);
  _X = gx;
  _Y = gy;
}
#else
void g_move(char * buf, size_t buf_len, int type, double gx, double gy)
{
  /*avr-libc doesn't have * or %n so always output with fixed precision*/
  snprintf(buf, buf_len, "G%d X%.2f Y%.2f", type, gx, gy);
  _X = gx;
  _Y = gy;
}
#endif

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
  static int8_t left_margin=0;
  static int8_t right_margin=0;
  static int footer_line=0;

  buf[0]=0;
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

  if (_no_pre && (12 >= g_line))
  {
    if (2 == g_line)
      return g_line += 4;
    else
      return g_line++;
  }
  else
  {
    switch(g_line)
    {
      case 0: snprintf(buf, buf_len, _verbose? "( generated with %s )":"", argp_program_version);
      return g_line++;
      case 1:
        if(!_use_inch)
          snprintf(buf, buf_len, "G21%s",_verbose? " ( base unit mm )":"");
        else
          snprintf(buf, buf_len, "G20%s",_verbose? " ( base unit inch )":"");
      return g_line++;
      case 2: snprintf(buf, buf_len, "G90%s",_verbose? " ( absolute distance mode )":"");
      return g_line+=4;
      //grbl reports "Unsupported statement" for G64
      //case 3: snprintf(buf, buf_len, "G64%s",_verbose? " ( best possible speed )":"");
      //grbl reports "Unsupported statement" for G40
      //case 4: snprintf(buf, buf_len, "G40%s",_verbose? " ( turn off tool diameter compensation )":"");
      //grbl reports "Unsupported statement" for G49
      //case 5: snprintf(buf, buf_len, "G49%s",_verbose? " ( turns off tool length compensation )":"");
      case 6: snprintf(buf, buf_len, "G94%s",_verbose? " ( Feed Rate Mode: Units per minute Mode )":"");
      return g_line++;
      case 7: snprintf(buf, buf_len, "G17%s",_verbose? " ( X-Y plane )":"");
      return g_line++;
      case 8: snprintf(buf, buf_len, "M3 S10000");
      return g_line++;
      case 9:
        if(_verbose)
        {
          /* make a copy and replace newline with | */
          size_t len=strlen(_text);
          char *tmp=malloc(len+1);
          strcpy(tmp, _text);
          size_t i;
          for(i=0;i<len;++i) if(tmp[i]=='\n') tmp[i]='|';
          snprintf(buf, buf_len, "( text=\"%s\", font=\"%s\" )", tmp, _font);
          free(tmp);
          return g_line++;
        }
        else
        g_line++;
      case 10:
        if(_verbose)
        {
          snprintf(buf, buf_len, "( scale=%f, feed=%f, precision=%d )",_scale, _feed, _precision);
          return g_line++;
        }
        else g_line++;
      case 11:
#ifndef FIXED_PRECISION
        snprintf(buf, buf_len, "F%.*f", _precision, _feed);
#else
        snprintf(buf, buf_len, "F%.2f", _feed);
#endif
        return g_line++;
      case 12:
#ifndef FIXED_PRECISION
        snprintf(buf, buf_len, "G0 Z%.*f%s", _precision, _Z_up, _verbose? " ( Pen-Up at start)":"");
#else
        snprintf(buf, buf_len, "G0 Z%.2f%s", _Z_up, _verbose? " ( Pen-Up at start)":"");
#endif
        _Z = _Z_up;
        return g_line++;
      default:
        break;
    }
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
      if(c=='\n')
      {
          x_glyph=_X0;
          y_glyph-=_yinc;
          snprintf(buf, buf_len, "( Linefeed )");
          char_index++;
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

        if (_verbose)
          snprintf(buf, buf_len, ";%c = %s",c, glyph_ptr);
        //grbl hast problems with ;x comments and shows error "expected command letter"
        //else
        //  snprintf(buf, buf_len, ";%c",c);
        return g_line++;
      }
      else if(!(*glyph_ptr)) /*end of glyph*/
      {
        char_index++;
        /* add glyph width to left position */
        x_glyph+=(right_margin-left_margin)*_scale;

        free(current_glyph);
        current_glyph=0;

        _Z = _Z_up;
#ifndef FIXED_PRECISION
        snprintf(buf, buf_len, "G0 Z%.*f%s", _precision, _Z, _verbose? " ( Pen-Up, EOG )":"");
#else
        snprintf(buf, buf_len, "G0 Z%.2f%s", _Z, _verbose? " ( Pen-Up, EOG )":"");
#endif
        pen_state=Up;
        return g_line++;
      }
      int8_t x=*(glyph_ptr)-'R';
      int8_t y=*(glyph_ptr+1)-'R';

      if (x==-50 && y==0) /*Pen-Up*/
      {
        _Z = _Z_up;
#ifndef FIXED_PRECISION
        snprintf(buf, buf_len, "G0 Z%.*f%s", _precision, _Z, _verbose? " ( Pen-Up )":"");
#else
        snprintf(buf, buf_len, "G0 Z%.2f%s", _Z, _verbose? " ( Pen-Up )":"");
#endif
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
          g_move(buf, buf_len, 1, gx, gy);
          glyph_ptr+=2;
          return g_line++;
        }
        else
        {
          if (!pen_above_pos)
          {
            /*rapid move because pen is up*/
            g_move(buf, buf_len, 0, gx, gy);
            glyph_ptr+=2;
            pen_above_pos=1;
            return g_line++;
          }
          else
          {
            /* lower pen*/
            _Z = _Z_down;
#ifndef FIXED_PRECISION
            snprintf(buf, buf_len, "G1 Z%.*f%s", _precision, _Z, _verbose? " ( Pen-Down )":"");
#else
            snprintf(buf, buf_len, "G1 Z%.2f%s", _Z, _verbose? " ( Pen-Down )":"");
#endif
            pen_state = Down;
            pen_above_pos=0;
            return g_line++;
          }
        }
      }
    }

    /*end of text*/
    if (!_no_post)
    {
      switch (footer_line++)
      {
        case 0: snprintf(buf, buf_len, "M5 %s",_verbose? "(stop the spindle)":"");
        return g_line++;
        case 1: snprintf(buf, buf_len, "M30 %s",_verbose? "(Program stop, rewind to beginning of program)":"");
#ifdef AVR
        buf[0]=0;   //No M30 on AVR
#endif
        return g_line++;
        default:
          break;
      }
    }
  }
  return -1;
}
