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


/* TODO
 * precision noch implementieren,
       An optional precision, in the form of a period ('.')  followed by an optional decimal digit string.  Instead of a  decimal
       digit  string  one  may write "*" or "*m$" (for some decimal integer m) to specify that the precision is given in the next
       argument, or in the m-th argument, respectively, which must be of type int.  If the precision is given as just '.', or the
       precision  is negative, the precision is taken to be zero.  This gives the minimum number of digits to appear for d, i, o,
       u, x, and X conversions, the number of digits to appear after the radix character for a, A, e, E, f,  and  F  conversions,
       the maximum number of significant digits for g and G conversions, or the maximum number of characters to be printed from a
       string for s and S conversions.
 * multiline, right, center
*/

#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include <errno.h>
#include "libhf2gcode.h"

extern const char *argp_program_version;

const char *argp_program_bug_address =
 "<andy.weber.aw@gmail.com>";

static char doc[] =
 "hf2gcode, a hershey font to g-code tracer";

/* A description of the arguments we accept. */
static char args_doc[] = "TEXT";

/* The options we understand. */
static struct argp_option options[] = {
 {"font",         'h', "FONT",  0, "Use FONT instead of default font \"rowmans\"", 0},
 {"output",       'o', "FILE",  0, "Output to FILE instead of standard output", 0 },
 {"scale",        's', "SCALE", 0, "Base unit/hershey font unit (default 0.5)", 0 },
 {"feed",         'f', "FEED",  0, "Feed rate (default 200)", 0 },
 {"xoffset",      'x', "X0",    0, "X-Axis offset (default 0)", 0 },
 {"yoffset",      'y', "Y0",    0, "Y-Axis offset (default 0)", 0 },
 {"z-up",         'u', "ZUp",   0, "Pen-Up Z value (default 1)", 0 },
 {"z-down",       'd', "ZDown", 0, "Pen-Down Z value (default -1)", 0 },
 {"interline",    'n', "YINC",  0, "Interline spacing in Y direction for multiple lines (default 30)", 0 },
 {"align-left",   'l', 0,       0, "Left align multiple lines (default)", 0},
 {"align-right",  'r', 0,       0, "Right align multiple lines", 0},
 {"align-center", 'c', 0,       0, "Center multiple lines", 0},
 {"min-gcode",    'm', 0,       0, "Generate minimalistic g-code, suppress comments", 0},
 {"precision",    'p', "PREC",  0, "Precision for G-Code generation (default 3)", 0},
 {"inch",         'i', 0,       0, "Use inch as base unit (default mm)", 0},
 {"quiet",    'q', 0,      0,  "Don't produce any output", 0},
 { 0 }
};

enum eAlign {left=0, right, center};
enum eBaseUnit {mm=0, inch};
/* Used by main to communicate with parse_opt. */
struct arguments
{
 char *text;
 char *font;
 char *output_file;
 double scale;
 double feed;
 double xoffset;
 double yoffset;
 double z_up;
 double z_down;
 double y_interline;
 enum eAlign align;
 enum eBaseUnit base;
 int min_gcode, quiet;
 int prec;
};

const char* get_base_unit(struct arguments arg)
{
  return (arg.base == mm)? "mm":"inch";
}


/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
 /* Get the input argument from argp_parse, which we
    know is a pointer to our arguments structure. */
 struct arguments *arguments = state->input;

 errno = 0;
 char *endptr=NULL;
 int num_parse=0;
 switch (key)
   {
   case 'h':
     arguments->font = arg;
     break;
   case 'o':
     arguments->output_file = arg;
     break;
   case 's':
     arguments->scale = strtod(arg, &endptr);
     num_parse=1;
     break;
   case 'f':
     arguments->feed = strtod(arg, &endptr);
     num_parse=1;
     break;
   case 'x':
     arguments->xoffset = strtod(arg, &endptr);
     num_parse=1;
     break;
   case 'y':
     arguments->yoffset = strtod(arg, &endptr);
     num_parse=1;
     break;
   case 'u':
     arguments->z_up = strtod(arg, &endptr);
     num_parse=1;
     break;
   case 'd':
     arguments->z_down = strtod(arg, &endptr);
     num_parse=1;
     break;
   case 'n':
     arguments->y_interline = strtod(arg, &endptr);
     num_parse=1;
     break;
   case 'l':
     arguments->align = left;
     break;
   case 'r':
     arguments->align = right;
     break;
   case 'c':
     arguments->align = center;
     break;
   case 'm':
     arguments->min_gcode = 1;
     break;
   case 'p':
     arguments->prec=strtol(arg, &endptr, 10);
     num_parse=1;
     break;
   case 'i':
     arguments->base = inch;
     break;
   case 'q':
     arguments->quiet = 1;
     break;

   case ARGP_KEY_ARG:
     if (state->arg_num >= 1)
       /* Too many arguments. */
       argp_usage (state);

     arguments->text = arg;
     break;

   case ARGP_KEY_END:
     if (state->arg_num < 1)
       /* Not enough arguments. */
       argp_usage (state);
     break;

   default:
     return ARGP_ERR_UNKNOWN;
   }

  if(errno)
  {
    perror("ERROR in parse_opt");
    exit(EXIT_FAILURE);
  }
  if (num_parse)
  {
    if(endptr == arg)
    {
      fprintf(stderr, "ERROR in parse_opt: No digits found.\n");
      exit(EXIT_FAILURE);
    }
    if (*endptr != '\0')
      printf("WARNING in parse_opt: Further characters after number: %s\n", endptr);
  }
  return 0;
}

/* Our argp parser. */
static struct argp argp = { options, parse_opt, args_doc, doc, 0, 0, 0};

int
main (int argc, char **argv)
{
  struct arguments arguments;

  /* Default values. */
  arguments.font         = "rowmans";
  arguments.output_file  = "-";
  arguments.scale        = 0.5;
  arguments.feed         = 200;
  arguments.xoffset      = 0.0;
  arguments.yoffset      = 0.0;
  arguments.z_up         = 1.0;
  arguments.z_down       = -1.0;
  arguments.y_interline  = 30;
  arguments.align        = left;
  arguments.base         = mm;
  arguments.min_gcode    = 0;
  arguments.quiet        = 0;
  arguments.prec         = 3;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  /* Don't print stats if quiet*/
  if (!arguments.quiet)
  {
    printf("Input Text         : %s\n", arguments.text);
    printf("Used hershey font  : %s\n", arguments.font);
    printf("G-code Output      : %s\n", arguments.output_file);
    printf("Base Unit          : %s\n", get_base_unit(arguments));
    printf("Scale              : %f\n", arguments.scale);
    printf("Feed rate          : %f %s/min\n", arguments.feed, get_base_unit(arguments));
    printf("X-Axis offset      : %f %s\n", arguments.xoffset, get_base_unit(arguments));
    printf("Y-Axis offset      : %f %s\n", arguments.yoffset, get_base_unit(arguments));
    printf("Pen-Up   Z value   : %f %s\n", arguments.z_up, get_base_unit(arguments));
    printf("Pen-Down Z value   : %f %s\n", arguments.z_down, get_base_unit(arguments));
    printf("Y interline        : %f %s\n", arguments.y_interline, get_base_unit(arguments));
    printf("Multiline align    : %s\n", (arguments.align == left)? "left": ((arguments.align == right)? "right" : "center"));
    printf("Minimalistic gcode : %s\n", (arguments.min_gcode)? "yes": "no");
    //printf("Number of G0 moves :
    //printf("Number of G1 moves :

  }

  /* check not implemented params */
  /* TODO: implement the functions... */
  if (arguments.align == right)
  {
    fprintf(stderr ,"ERROR: I'm sorry but align-right isn't implemented yet. Please use align-left instead\n");
    exit(EXIT_FAILURE);
  }
  else if (arguments.align == center)
  {
    fprintf(stderr ,"ERROR: I'm sorry but align-center isn't implemented yet. Please use align-left instead\n");
    exit(EXIT_FAILURE);
  }

  FILE *fn_gout=NULL;
  if (!strcmp (arguments.output_file, "-"))
    fn_gout = stdout;
  else
    fn_gout = fopen(arguments.output_file, "w");
  if (!fn_gout)
    perror("main.c: Creation of output file failed:");
  else
  {
    int r = init_get_gcode_line (arguments.font,
                                 arguments.text,
                                 arguments.xoffset,
                                 arguments.yoffset,
                                 arguments.z_up,
                                 arguments.z_down,
                                 arguments.scale,
                                 arguments.feed,
                                 arguments.prec,
                                 !arguments.min_gcode,
                                 'l');
    char buf[200];
    int gl;
    while((gl = get_gcode_line (buf, 200))!=-1)
    {
      fprintf(fn_gout, "%s\n",buf);
    }
    fclose(fn_gout);
  }
 exit (0);
}
