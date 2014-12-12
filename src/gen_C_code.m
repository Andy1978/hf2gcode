## Copyright (C) 2014 Andreas Weber <andy.weber.aw@gmail.com>
##
## This program is free software: you can redistribute it and/or modify
## it under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## This program is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty of
## MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
## GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

## -*- texinfo -*-
## @deftypefn  {Function File} {} gen_C_code (@var{fn}, @var{fn_out})
## Generate a C var declaration for the hershey font @var{fn}.
## The generated code is very similiar to the original hershey font.
## Differences: no glyph number, there is no length at the beginning. The end is marked with \0
## @seealso{load_hf}
## @end deftypefn

function gen_C_code(fn, fn_out)
  [DIR, NAME] = fileparts (fn);

  hf = load_hf (fn);
  [fid, MSG] = fopen(fn_out,"w");
  if (fid != -1)
    fprintf(fid, "/* This file was generated with gen_C_code.m from %s */\n\n",fn);
    fprintf(fid, "/* number of glyphs in font */\nunsigned char %s_cnt = %d;\n\n", NAME, length(hf));
    fprintf(fid, "/* Format: margin left, margin right, X, Y ... \\0\n< R> is pen-up, see original hershey font */\n");
    fprintf(fid, "const char %s [] PROGMEM=",NAME);
    for i=1:length(hf);
      glyph=hf(i);
      l=length(glyph.x);
      s=char([glyph.leftmargin glyph.x;glyph.rightmargin glyph.y](:)'+'R');
      ## escape some chars
      s = strrep(s, "\\", "\\\\");
      s = strrep(s, "\"", "\\\"");
      if(i>1)
        fprintf(fid, "          ");
      endif
      fprintf(fid,"\"%s\\0\"\\\n",s)
    endfor
    fprintf(fid,";\n")
    fprintf(fid, "/* End of file %s */\n",fn_out);
    fclose(fid);
  else
    error("couldn't create file")
    disp(msg)
  endif
endfunction
