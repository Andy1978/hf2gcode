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
