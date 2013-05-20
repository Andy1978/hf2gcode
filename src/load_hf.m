## 20.05.2013 Andreas Weber
## loads a "fixed" hershey font (no linebreaks in glyph and no CR)
## And returns a structure containing the glyphs

function ret = load_hf (fn)
  [fid,msg]=fopen(fn,"r");
  if (fid != -1)
    cnt=0;
    while ((s = fgetl (fid))!=-1)
      cnt+=1;
      ret(cnt).num = str2num (s(1:5));
      ret(cnt).len = str2num (s(6:8));
      ## check length
      if (ret(cnt).len != (length(s)-8)/2)
        error ("actual length of glyph doesn't match. Perhaps a corrupted file? Check linebreaks in glyph and CR/LF issues.")
      endif
      ret(cnt).leftmargin  = s(9)-'R';
      ret(cnt).rightmargin = s(10)-'R';

      ret(cnt).x = s(11:2:end)-'R';
      ret(cnt).y = s(12:2:end)-'R';

    endwhile
  fclose (fid);
  endif
endfunction

  %~ s=e=1;
  %~ m=numel(x);
  %~ for k=1:m+1
    %~ if (k==m+1 || (((x(k)==-50) && y(k)==0)))
      %~ px=x(s:k-1)+xo;
      %~ ##invertiert, in hershey font ist y nach unten positiv
      %~ py=-y(s:k-1)+yo;
      %~ plot(px, py)
      %~ text(xo,yo-20,num2str(glyphnumber));
      %~ s=k+1;
      %~ hold on
    %~ endif
  %~ endfor
  %~ 
%~ endfunction
%~ 
%~ s = fgetl (fid);
%~ 
%~ ## ToDo: wie den einzelnen Punkt darstellen?
%~ 
%~ x=0;
%~ y=0;
%~ while ((s = fgetl (fid))!=-1)
%~ 
  %~ plothf(s,x*30,y*50)
  %~ x+=1;
  %~ if(x==10)
    %~ x=0;
    %~ y+=1;
  %~ endif
  %~ 
  %~ 
%~ endwhile
%~ 
%~ axis square
%~ hold off
%~ 
%~ print("scripts.svg","-S1200,1200")
