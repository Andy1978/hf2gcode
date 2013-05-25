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
## @deftypefn  {Function File} {} plot_hf (@var{hf}, @var{cols}=10, @var{xsep}=30, @var{ysep}=50)
## Plots a hershey font (all glyphs)
## @end deftypefn

function plot_hf(hf,cols=10, xsep=30, ysep=50)
  for (i=1:length(hf))
    col = mod (i-1,cols);
    row = floor ((i-1)/cols);
    plot_hf_glyph(hf(i), xsep*col, ysep*row, 1)
    hold on
  endfor
  hold off
endfunction
