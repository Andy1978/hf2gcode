## Copyright (C) 2013-2022 Andreas Weber <andy.weber.aw@gmail.com>
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
## Render hershey font using octave and export it as SVG.
## Its purpose is to get an overview over the available glyphs.
## @seealso{plot_hf}
## @end deftypefn

function render_hf (fn, svg_out = "./rendered_svg")
  [DIR, NAME, EXT] = fileparts (fn);
  mkdir (svg_out);
  fn_svg = fullfile(svg_out, [NAME,".svg"]);
  printf("Rendering %s from %s...\n", fn_svg, fn);
  fflush(stdout);
  hf = load_hf (fn);
  figure("visible","off");
  plot_hf(hf)
  print (fn_svg)
endfunction
