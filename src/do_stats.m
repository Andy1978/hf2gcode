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

for k=1:length(all_hf)
  fn=strcat("../hershey_fonts/fixed/",all_hf{k});
  hf = load_hf (fn);

  ## show min && max in x and y
  tmp_x = [hf.x];
  tmp_y = [hf.y];
  ind = tmp_x == -50;
  tmp_x(ind) = [];
  tmp_y(ind) = [];
  disp([min(tmp_x) max(tmp_x) min(tmp_y) max(tmp_y)])
endfor
