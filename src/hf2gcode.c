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
static char args_doc[] = "[TEXT]";

/* The options we understand. */
static struct argp_option options[] =
{
  {"font",         'h', "FONT",   0, "Use FONT instead of default font \"rowmans\"", 0},
  {"output",       'o', "FILE",   0, "Output to FILE instead of stdout", 0 },
  {"scale",        's', "SCALE",  0, "Base unit/hershey font unit (default 0.5)", 0 },
  {"input",        'i', "FILE",   0, "Read text from FILE instead of stdin", 0 },
  {"append",      'a', 0,         0, "Append to output file instead of overwriting", 0},
  {0, 0, 0, 0, "G-code base settings:", 1 },
  {"feed",         'f', "FEED",   0, "Feed rate (default 200)", 1 },
  {"xoffset",      'x', "X0",     0, "X-Axis offset (default 0)", 1 },
  {"yoffset",      'y', "Y0",     0, "Y-Axis offset (default 0)", 1 },
  {"z-up",         1000, "ZUp",   0, "PenUp Z value (default 1)", 1 },
  {"z-down",       1001, "ZDown", 0, "PenDown Z value (default -1)", 1 },
  {0, 0, 0, 0, "Multiline settings:", 2 },
  {"interline",    'n', "YINC",   0, "Interline spacing in Y direction for multiple lines (default 15)", 2 },
  {"align-left",   'l', 0,        0, "Left align multiple lines (default)", 2},
  {"align-right",  'r', 0,        0, "Right align multiple lines", 2},
  {"align-center", 'c', 0,        0, "Center multiple lines", 2},
  {0, 0, 0, 0, "Miscellaneous:", 3 },
  {"min-gcode",    'm', 0,        0, "Generate minimalistic g-code, suppress comments", 3},
  {"precision",    'p', "PREC",   0, "Precision for G-Code generation (default 3)", 3},
  {"inch",         'u', 0,        0, "Use United States customary units (inch instead of mm) as base unit", 3},
  {"verbose",      'v', 0,        0, "Increase verbosity", 3},
  {"no-pre",       1002, 0,       0, "Don't include preamble", 3},
  {"no-post",      1003, 0,       0, "Don't include postamble", 3},
  { 0,0,0,0,0,0 }
};

enum eAlign {left=0, right, center};
enum eBaseUnit {mm=0, inch};
/* Used by main to communicate with parse_opt. */
struct arguments
{
  char *text;
  char *font;
  char *input_file;
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
  int min_gcode, verbose, append, no_pre, no_post;
  int prec;
  int read_stream;
};

const char* get_base_unit(struct arguments arg)
{
  return (arg.base == mm)? "mm":"inch";
}

size_t read_text(char **t, FILE *stream)
{
#define BUFSIZE 100
  size_t s=0;
  *t=malloc(BUFSIZE);
  if(!*t)
    {
      perror("ERROR in read_text, malloc failed: ");
      exit(EXIT_FAILURE);
    }
  size_t cnt;
  size_t sum=0;
  do
    {
      cnt = fread(*t+s, 1, BUFSIZE, stream);
      sum+=cnt;
      if(cnt == BUFSIZE)
        {
          /*BUFSIZE too small to read all chars*/
          s+=BUFSIZE;
          *t=realloc(*t, s+BUFSIZE);
          if(!*t)
            {
              perror("ERROR in read_text, realloc failed: ");
              exit(EXIT_FAILURE);
            }
        }
    }
  while(cnt == BUFSIZE);
  *(*t+sum)=0;
  return sum;
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
    case 'i':
      arguments->input_file = arg;
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
    case 1000:
      arguments->z_up = strtod(arg, &endptr);
      num_parse=1;
      break;
    case 1001:
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
    case 'u':
      arguments->base = inch;
      break;
    case 'v':
      arguments->verbose = 1;
      break;
    case 'a':
      arguments->append = 1;
      arguments->no_pre = 1;
      break;
    case 1002:
      arguments->no_pre = 1;
      break;
    case 1003:
      arguments->no_post = 1;
      break;

    case ARGP_KEY_ARG:
      if (state->arg_num >= 1)
        /* Too many arguments. */
        argp_usage (state);
      arguments->text = arg;
      break;
    case ARGP_KEY_END:
      if (state->arg_num < 1)
        {
          /* No text, use stdin to read text. */
          arguments->read_stream = 1;
        }
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
        printf("( WARNING in parse_opt: Further characters after number: %s )\n", endptr);
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
  arguments.input_file   = "-";
  arguments.output_file  = "-";
  arguments.scale        = 0.5;
  arguments.feed         = 200;
  arguments.xoffset      = 0.0;
  arguments.yoffset      = 0.0;
  arguments.z_up         = 1.0;
  arguments.z_down       = -1.0;
  arguments.y_interline  = 15;
  arguments.align        = left;
  arguments.base         = mm;
  arguments.min_gcode    = 0;
  arguments.verbose      = 0;
  arguments.no_pre       = 0;
  arguments.no_post      = 0;
  arguments.append       = 0;
  arguments.prec         = 3;
  arguments.read_stream  = 0;

  /* Parse our arguments; every option seen by parse_opt will
     be reflected in arguments. */
  argp_parse (&argp, argc, argv, 0, 0, &arguments);

  if(arguments.read_stream)
    {
      FILE *f_in=NULL;
      int cnt;
      if (!strcmp (arguments.input_file, "-"))
        f_in = stdin;
      else
        f_in = fopen(arguments.input_file, "r");
      if (!f_in)
        perror("main.c: Error opening input file:");
      else
        cnt=read_text(&arguments.text, f_in);
      if (arguments.verbose)
        printf("( INFO:read %d bytes from %s)\n", cnt, arguments.input_file);
    }

  if (arguments.verbose)
    {
      if(!arguments.read_stream)
        printf("( Text               : %s )\n", arguments.text);
      printf("( Used hershey font  : %s )\n", arguments.font);
      printf("( G-code Output      : %s )\n", arguments.output_file);
      printf("( Base Unit          : %s )\n", get_base_unit(arguments));
      printf("( Scale              : %f )\n", arguments.scale);
      printf("( Feed rate          : %f %s/min )\n", arguments.feed, get_base_unit(arguments));
      printf("( X-Axis offset      : %f %s )\n", arguments.xoffset, get_base_unit(arguments));
      printf("( Y-Axis offset      : %f %s )\n", arguments.yoffset, get_base_unit(arguments));
      printf("( Pen-Up   Z value   : %f %s )\n", arguments.z_up, get_base_unit(arguments));
      printf("( Pen-Down Z value   : %f %s )\n", arguments.z_down, get_base_unit(arguments));
      printf("( Y interline        : %f %s )\n", arguments.y_interline, get_base_unit(arguments));
      printf("( Multiline align    : %s )\n", (arguments.align == left)? "left": ((arguments.align == right)? "right" : "center"));
      printf("( Minimalistic gcode : %s )\n", (arguments.min_gcode)? "yes": "no");
      printf("( Precision          : %d )\n", arguments.prec);
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

  int init_ret = init_get_gcode_line (arguments.font,
                                      arguments.text,
                                      arguments.xoffset,
                                      arguments.yoffset,
                                      arguments.z_up,
                                      arguments.z_down,
                                      arguments.y_interline,
                                      arguments.scale,
                                      arguments.feed,
                                      arguments.prec,
                                      !arguments.min_gcode,
                                      'l',
                                      arguments.base == inch,
                                      arguments.no_pre,
                                      arguments.no_post);

  if (!init_ret)  /* init successful */
    {
      FILE *fn_gout=NULL;
      if (!strcmp (arguments.output_file, "-"))
        fn_gout = stdout;
      else
        fn_gout = fopen(arguments.output_file, (arguments.append)? "a" : "w");
      if (!fn_gout)
        perror("main.c: Creation of output file failed:");
      else
        {
          char buf[1000];
          int gl;
          while((gl = get_gcode_line (buf, 1000))!=-1)
            {
              fprintf(fn_gout, "%s\n",buf);
            }
          if (arguments.verbose)
            printf("( FINISHED )\n");
          fclose(fn_gout);
        }
    }
  else /*init failed*/
    {
      fprintf(stderr, "ERROR: Initialisation of g-code generator failed. Your text may contain some characters which are not available in the selected font.\n");
      fprintf(stderr, "INFO: available fonts: rowmans, cursive, futural, futuram, gothgbt, gothgrt, gothiceng, gothicger, gothicita, gothitt, greekc, greek, greeks, rowmand, rowmant, scriptc, scripts, symbolic, timesg, timesib, timesi, timesrb, timesr\n");
      exit(EXIT_FAILURE);
    }
  if (arguments.read_stream)
    free(arguments.text);
  exit (0);
}
