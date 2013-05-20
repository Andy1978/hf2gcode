
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

outdir = "rendered_hf";
for (k=1:length(all_hf))
  fn=strcat("../hershey_fonts/fixed/",all_hf{k});
  printf("Rendering %s to %s...\n",fn,outdir);
  fflush(stdout);
  render_hf(fn,outdir);
endfor
