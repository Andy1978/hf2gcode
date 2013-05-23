#ifndef _LIBHF2GCODE_H_
#define _LIBHF2GCODE_H_

#include <stdio.h>

char * get_glyph_ptr (const char *font,
                      const unsigned char c);
                      
void g_header(FILE *f);
             
void g_footer(FILE *f);

int g_body ( FILE *f,\
             char verbose,\
             const char *font,\
             double scale,\
             double feed,\
             double Z_up,\
             double Z_down,\
             const char *text);

#endif
