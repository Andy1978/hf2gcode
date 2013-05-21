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
  else
    error("file %s not found",fn)
  endif
endfunction
