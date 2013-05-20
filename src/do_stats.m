## temporäre Datei, 20.5.2013 Andy
## Statistiken sammeln um funtkion von marginleft+right zu verstehen.
## Außerdem Grundlage für eine kompakte binäre Representation
## vielleicht kann man mehrere Koordinaten in ein Byte packen?
## +/- 16 (4Bit sind wahrscheinlich zu wenig)


all_hf={"astrology.jhf",
        "cursive.jhf",
        "cyrilc_1.jhf",
        "cyrillic.jhf",
        "futural.jhf",
        "futuram.jhf",
        "gothgbt.jhf",
        "gothgrt.jhf",
        "gothiceng.jhf",
        "gothicger.jhf",
        "gothicita.jhf",
        "gothitt.jhf",
        "greekc.jhf",
        "greek.jhf",
        "greeks.jhf",
        "japanese.jhf",
        "markers.jhf",
        "mathlow.jhf",
        "mathupp.jhf",
        "meteorology.jhf",
        "music.jhf",
        "rowmand.jhf",
        "rowmans.jhf",
        "rowmant.jhf",
        "scriptc.jhf",
        "scripts.jhf",
        "symbolic.jhf",
        "timesg.jhf",
        "timesib.jhf",
        "timesi.jhf",
        "timesrb.jhf",
        "timesr.jhf"};

%~ for (k=1:length(all_hf))
  %~ fn=strcat("../hershey_fonts/fixed/",all_hf{k});
  %~ printf("Using %s...\n",fn);
  %~ fflush(stdout);
  %~ #render_hf(fn,outdir);
%~ endfor

fn=strcat("../hershey_fonts/fixed/",all_hf{23})
hf = load_hf (fn);

#wie hängt rightmargin mit der maximalen Ausdehnung zusammen?
for k=1:length(hf)
  i = find (hf(k).x==-50);
  #hf(k).x(i)=[];
  #hf(k).y(i)=[];
  
  printf("%d %d %d %d\n",\
    min(hf(k).x),\
    hf(k).leftmargin,\
    max(hf(k).x),\
    hf(k).rightmargin);

endfor
