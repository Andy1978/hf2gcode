## 21.5.2013 Andy
## generate C-code for rowmans.jhf

## the generated code is very similiar to the original hershey font.
## Differences:
## - no glyph number
## - there is no length at the beginning. The end is marked with \0

function gen_C_code(fn, fn_out)
  [DIR, NAME, EXT, VER] = fileparts (fn);

  hf = load_hf (fn);
  [fid, MSG] = fopen(fn_out,"w");
  if (fid != -1)
    fprintf(fid, "/* number of glyphs in font */\nunsigned char %s_cnt = %d;\n", NAME, length(hf));
    fprintf(fid, "/* Format: margin left, margin right, X, Y ... \\0\n< R> is pen-up, see original hershey font */\n");
    fprintf(fid, "char %s []=",NAME);
    for i=1:length(hf);
      glyph=hf(i);
      l=length(glyph.x);
      s=char([glyph.leftmargin glyph.x;glyph.rightmargin glyph.y](:)'+'R');
      ## escape some chars
      s = strrep(s, "\\", "\\\\");
      if(i>1)
        fprintf(fid, "          ");
      endif
      fprintf(fid,"\"%s\\0\"\\\n",s)
    endfor
    fprintf(fid,";\n")
    fclose(fid);
  else
    error("couldn't create file")
    disp(msg)
  endif
endfunction
