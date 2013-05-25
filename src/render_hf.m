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
## @deftypefn  {Function File} {} render_hf (@var{fn}, @var{outdir})
## Render hershey font using octave, export it to PNG and SVG image.
## Its purpose is to get an overview over the available glyphs.
## Attention: outdir works only once, (not like mkdir -p).
## @seealso{plot_hf}
## @end deftypefn

function render_hf (fn, outdir="./")
  [DIR, NAME, EXT, VER] = fileparts (fn);
  fn_png = fullfile(strcat(outdir,"_png"),strcat(NAME,".png"));
  fn_svg = fullfile(strcat(outdir,"_svg"),strcat(NAME,".svg"));
  printf("Rendering %s and %s from %s...\n",fn_png,fn_svg,fn);
  fflush(stdout);

  hf = load_hf (fn);
  figure("visible","off");
  plot_hf(hf), axis square

  print(fn_png,"-S1200,1200")
  print(fn_svg,"-S1200,1200")
endfunction
