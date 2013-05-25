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
## @deftypefn  {Function File} {} plot_hf_glyph (@var{glyph}, @var{xo}, @var{yo}, @var{plot_margins}=0)
## Plots a single hershey glyph.
## @seealso{plot_hf}
## @end deftypefn

function plot_hf_glyph(glyph, xo, yo, plot_margins=0)
  s=1;
  if(plot_margins)
    #left and right margins in red
    plot([glyph.leftmargin+xo, glyph.leftmargin+xo], [10+yo -10+yo],"-r");
    hold on
    plot([glyph.rightmargin+xo, glyph.rightmargin+xo], [10+yo -10+yo],"-r");
  endif
  text(xo-5,yo-20,num2str(glyph.num));
  m=numel(glyph.x);
  if (m>0)
    for k=1:m+1
      if (k==m+1 || (((glyph.x(k)==-50) && glyph.y(k)==0))) ## last element or Pen-Up command
        px=glyph.x(s:k-1)+xo;
        ## inverted, in hershey font y positive down
        py=-glyph.y(s:k-1)+yo;
        plot(px, py)
        if(!plot_margins)
          hold on;
        endif
        s=k+1;
      endif
    endfor
  endif
  hold off
endfunction
