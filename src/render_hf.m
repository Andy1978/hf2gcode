## 17.5.2013 Andreas Weber
## Render hershey font using octave, export it to an image
## Its purpose is to get an overview over the available glyphs
## Attention: outdir works only once, (not like mkdir -p)

function render_hf (fn, outdir="./")
  [DIR, NAME, EXT, VER] = fileparts (fn);
  hf = load_hf (fn);
  plot_hf(hf), axis square

  if(!exist(outdir,"dir"))
    mkdir(outdir);
  endif

  print(fullfile(outdir,strcat(NAME,".png")),"-S1200,1200")
  print(fullfile(outdir,strcat(NAME,".svg")),"-S1200,1200")
endfunction
