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

## 21.5.2013 Andy
## plot text with rowmans.jhf

fn="../hershey_fonts/fixed/rowmans.jhf"
hf = load_hf (fn);

## translation table ASCII -> hershey_font entry for rowmans
tt=zeros(256,1);
tt(32:126)=1:95;

s="Hello world!";

left=0;
for i=1:length(s)
  glyph=hf(tt(s(i)));
  plot_hf_glyph(glyph, left-glyph.leftmargin, 0, false)
  hold on
  left+=glyph.rightmargin-glyph.leftmargin;
endfor
hold off
axis equal
