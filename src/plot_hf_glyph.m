## 20.05.2013 Andreas Weber
## Plots a single hershey glyph

function plot_hf_glyph(glyph, xo, yo)
  s=1;
  m=numel(glyph.x);
  for k=1:m+1
    if (k==m+1 || (((glyph.x(k)==-50) && glyph.y(k)==0))) ## last element or Pen-Up command
      px=glyph.x(s:k-1)+xo;
      ## inverted, in hershey font y positive down
      py=-glyph.y(s:k-1)+yo;
      plot(px, py)
      text(xo,yo-20,num2str(glyph.num));
      s=k+1;
      hold on
    endif
  endfor
  hold off
endfunction
