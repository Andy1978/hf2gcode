## Copyright (C) 2013 Andreas Weber <andy.weber.aw@gmail.com>
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
## @deftypefn  {Function File} {@var{ret} =} load_hf (@var{fn})
## Loads a "fixed" hershey font (no linebreaks in glyph and no CR)
## and returns a structure containing the glyphs.
## @end deftypefn

function ret = load_hf (fn)
  [fid,msg]=fopen(fn,"r");
  if (fid != -1)
    cnt=0;
    while ((s = fgetl (fid))!=-1)
      cnt+=1;
      ret(cnt).num = str2num (s(1:5));
      ret(cnt).len = str2num (s(6:8));
      ## check length
      if (ret(cnt).len != (length(s)-8)/2)
        error ("actual length of glyph doesn't match. Perhaps a corrupted file? Check linebreaks in glyph and CR/LF issues.")
      endif
      ret(cnt).leftmargin  = s(9)-'R';
      ret(cnt).rightmargin = s(10)-'R';

      ret(cnt).x = s(11:2:end)-'R';
      ret(cnt).y = s(12:2:end)-'R';

    endwhile
  fclose (fid);
  else
    error("file %s not found",fn)
  endif
endfunction
