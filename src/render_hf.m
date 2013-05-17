## 17.5.2013 Andreas Weber
## Render hershey font using octave, export it to an image
## Its purpose is to get an overview over the available glyphs

[fid,msg]=fopen("../hershey_fonts/fixed/scripts.jhf","r");

function plothf(s, xo, yo)
  glyphnumber = str2num (s(1:5));
  len         = str2num (s(6:8));
  ## check length
  if (len != (length(s)-8)/2)
    error ("length doesn't match, corrupted file?")
  endif
  left        = s(9)-'R';
  right       = s(10)-'R';

  x=s(11:2:end)-'R';
  y=s(12:2:end)-'R';

  s=e=1;
  m=numel(x);
  for k=1:m+1
    if (k==m+1 || (((x(k)==-50) && y(k)==0)))
      px=x(s:k-1)+xo;
      ##invertiert, in hershey font ist y nach unten positiv
      py=-y(s:k-1)+yo;
      plot(px, py)
      text(xo,yo-20,num2str(glyphnumber));
      s=k+1;
      hold on
    endif
  endfor
  
endfunction

s = fgetl (fid);

## ToDo: wie den einzelnen Punkt darstellen?

x=0;
y=0;
while ((s = fgetl (fid))!=-1)

  plothf(s,x*30,y*50)
  x+=1;
  if(x>10)
    x=0;
    y+=1;
  endif
  
  
endwhile

axis square
hold off

print("scripts.png")
