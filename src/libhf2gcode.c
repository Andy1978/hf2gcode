#include "../hershey_fonts/gen_c_src/rowmans.h"
#include "../hershey_fonts/gen_c_src/scriptc.h"

const char *argp_program_version = "hf2gcode 0.1";

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

#include <stdio.h>
#include <string.h>
#include "libhf2gcode.h"

/* get pointer to start of the glyph or NULL if not available */
char * get_glyph_ptr (const char *font,
                      const unsigned char c)
{
  int cnt   = 0;
  char *ptr = NULL;
  int index = 0;
  if (!strcmp(font, "rowmans"))
  {
    cnt   = rowmans_cnt;
    ptr   = rowmans;
    index = c-32;
  }
  else if (!strcmp(font, "scriptc"))
  {
    cnt   = scriptc_cnt;
    ptr   = scriptc;
    index = c-32;
  }
  else
  {
    fprintf(stderr, "ERROR: font %s is not available.\n", font);
    return NULL;
  }
//printf("index=%d, ptr=%x s=%s\n",index,ptr,ptr);
  if (index<0 || index>cnt-1)
  {
    fprintf(stderr, "ERROR: glyph number %d (ASCII 0x%X) is not available in font \"%s\".\n", index, c, font);
    return NULL;
  }
  
  /*search the glyph*/
  int k=0;
  while(k++<index)
  {
    while(*(ptr++)!=0);
  }
//printf("%s\n",ptr);
  return ptr;
}

/* generate g-code header */
void g_header(FILE *f)
{
  fprintf (f, "( generated with %s )\n",argp_program_version);
  fprintf (f, "G21 ( units in mm )\n");
  fprintf (f, "G90 ( absolute distance mode )\n");
  fprintf (f, "G64 ( best possible speed )\n");
  fprintf (f, "G40 ( turn off tool diameter compensation )\n");
  fprintf (f, "G49 ( turns off tool length compensation )\n");
  fprintf (f, "G94 ( Feed Rate Mode: Units per Minute Mode )\n");
  fprintf (f, "G17 ( X-Y plane )\n");
  fprintf (f, "M3 S10000\n");
}

/* generate g-code footer */
void g_footer(FILE *f)
{
  fprintf (f, "M5 (stop the spindle)\n");
  fprintf (f, "M30 (Program stop, rewind to beginning of program)\n");
}

/* generate g-code body for all glyphs
 * 
 * RETURN -1 if target buffer is to small, else 0 if success
 */
int g_body ( FILE *f,\
             char verbose,\
             const char *font,\
             double scale,\
             double feed,\
             double Z_up,\
             double Z_down,\
             const char *text)
{
  fprintf (f, "( Text=\"%s\", font=\"%s\" )\n",text, font);
  fprintf (f, "( Scale=%f, feed=%f )\n",scale, feed);
  fprintf (f, "F%f\n", feed);
  
  double left=0;
  char pen_down=0;
  const char *p=text;
  char c;
  while((c=*(p++)))
  {
    char *glyph=get_glyph_ptr(font,c);
    if (verbose)
      fprintf(f, "( %c = %s )\n",c, glyph);

    char left_margin = *(glyph++)-'R';
    char right_margin = *(glyph++)-'R';
    
    while(*glyph)
    {
      char x=*(glyph++)-'R';
      char y=*(glyph++)-'R';
      
      if (x==-50 && y==0) /*Pen-Up*/
      {
        fprintf(f, "G0 Z%f\n", Z_up);
        pen_down=0;
      }
      else
      {
        double gx = left+(x-left_margin)*scale;
        double gy = -y*scale;
        if(pen_down)
        {
          fprintf(f, "G1 X%f Y%f\n", gx, gy);
        }  
        else
        {
          fprintf(f, "G0 X%f Y%f\n", gx, gy);
          fprintf(f, "G1 X%f Y%f Z%f\n", gx, gy, Z_down);
          pen_down = 1;
        }
      }
    }
    
    /* Pen-Up */
    fprintf(f, "G0 Z%f\n", Z_up);
    pen_down=0;
    
    /* add glyph width to left position */
    left+=(right_margin-left_margin)*scale;
  
  }
  return 0;
}

