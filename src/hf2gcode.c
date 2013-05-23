 /* 
  * hf2gcode, Copyright 2013 Andreas Weber
  * 
  * There have to be params for:
  * the used hershey font, for example 'rowmans'
  * the rendered text, linefeed with \n for multiple lines
  * if multiple lines, selectable left-, right- or center-align
  * scale factor: in first versions, a double multiplied with the hershey font coordinates
  * 
  * 
  * 
  * 
  */ 

#include <stdlib.h>
#include <string.h>
#include <argp.h>
#include "libhf2gcode.h"

extern const char *argp_program_version;

const char *argp_program_bug_address =
 "<andy.weber.aw@gmail.com>";

static char doc[] =
 "hf2gcode, a hershey font to g-code tracer";

/* A description of the arguments we accept. */
static char args_doc[] = "TEXT FONT SCALE";

/* The options we understand. */
static struct argp_option options[] = {
 {"verbose",  'v', 0,      0,  "Produce verbose output", 0},
 {"quiet",    'q', 0,      0,  "Don't produce any output", 0},
 {"silent",   's', 0,      OPTION_ALIAS, 0, 0},
 {"output",   'o', "FILE", 0,
  "Output to FILE instead of standard output", 0 },
 { 0 }
};

/* Used by main to communicate with parse_opt. */
struct arguments
{
 char *args[2];                /* arg1 & arg2 */
 int silent, verbose;
 char *output_file;
};

/* Parse a single option. */
static error_t
parse_opt (int key, char *arg, struct argp_state *state)
{
 /* Get the input argument from argp_parse, which we
    know is a pointer to our arguments structure. */
 struct arguments *arguments = state->input;

 switch (key)
   {
   case 'q': case 's':
     arguments->silent = 1;
     break;
   case 'v':
     arguments->verbose = 1;
     break;
   case 'o':
     arguments->output_file = arg;
     break;

   case ARGP_KEY_ARG:
     if (state->arg_num >= 2)
       /* Too many arguments. */
       argp_usage (state);

     arguments->args[state->arg_num] = arg;

     break;

   case ARGP_KEY_END:
     if (state->arg_num < 2)
       /* Not enough arguments. */
       argp_usage (state);
     break;

   default:
     return ARGP_ERR_UNKNOWN;
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
 arguments.silent = 0;
 arguments.verbose = 0;
 arguments.output_file = "-";

 /* Parse our arguments; every option seen by parse_opt will
    be reflected in arguments. */
 argp_parse (&argp, argc, argv, 0, 0, &arguments);

 printf ("ARG1 = %s\nARG2 = %s\nOUTPUT_FILE = %s\n"
         "VERBOSE = %s\nSILENT = %s\n",
         arguments.args[0], arguments.args[1],
         arguments.output_file,
         arguments.verbose ? "yes" : "no",
         arguments.silent ? "yes" : "no");



  FILE *fn_gout=NULL;
  if (!strcmp (arguments.output_file, "-"))
    fn_gout = stdout;
  else
    fn_gout = fopen(arguments.output_file, "w");
  if (!fn_gout)
    perror("main.c: Creation of output file failed:");
  else
  {
    g_header (fn_gout);
    g_body (fn_gout, arguments.verbose, arguments.args[0], 1, 500, 0, -3, arguments.args[1]);
    g_footer (fn_gout);

    fclose(fn_gout);    
    
  }


 exit (0);
}
