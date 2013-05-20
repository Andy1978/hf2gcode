## 20.05.2013 Andreas Weber
## Plots a single hershey glyph

function plot_hf_glyph(glyph, xo, yo)
  s=1;
  #left and right margins in red
  plot([glyph.leftmargin+xo, glyph.leftmargin+xo], [10+yo -10+yo],"-r");
  hold on
  plot([glyph.rightmargin+xo, glyph.rightmargin+xo], [10+yo -10+yo],"-r");
  text(xo-5,yo-20,num2str(glyph.num));
  m=numel(glyph.x);
  if (m<1)
    plot(0,0,"go")
  else
    for k=1:m+1
      if (k==m+1 || (((glyph.x(k)==-50) && glyph.y(k)==0))) ## last element or Pen-Up command
        px=glyph.x(s:k-1)+xo;
        ## inverted, in hershey font y positive down
        py=-glyph.y(s:k-1)+yo;
        plot(px, py)
        s=k+1;
      endif
    endfor
  endif
  hold off
endfunction
