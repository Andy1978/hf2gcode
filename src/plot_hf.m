## 20.05.2013 Andreas Weber
## Plots a hershey font (all glyphs)

function plot_hf(hf,cols=10, xsep=30, ysep=50)
  for (i=1:length(hf))
    col = mod (i-1,cols);
    row = floor ((i-1)/cols);
    plot_hf_glyph(hf(i), xsep*col, ysep*row)
    hold on
  endfor
  hold off
endfunction
