
addpath ("../src");
x = load_hf ("fixed/rowmans.jhf")
#plot_hf (x(2:4))

function glyph = tr (glyph, x1, y1, x2, y2)
  glyph.x(end-10:end-6) += x1;
  glyph.y(end-10:end-6) += y1;
  glyph.x(end-4:end)    += x2;
  glyph.y(end-4:end)    += y2;
endfunction

if (0)
  x(97) = tr (x(97), -3, -2, 3, -2);  #Ä
  x(98) = tr (x(98), -3, -2, 3, -2);  #Ö
  x(99) = tr (x(99), -3, -2, 3, -2);  #Ü

  x(100) = tr (x(100), -2, 4, 3, 4);  #ä
  x(101) = tr (x(101), -2, 4, 3, 4);  #ö
  x(102) = tr (x(102), -2, 4, 3, 4);  #ü
endif

plot_hf (x(97:end))


# Basis/Ausgangspunkt war das i Pünktchen: QFRGSFREQF

# ÄÖÜ
hf_shift ("QFRGSFREQF", -3, -2)
hf_shift ("QFRGSFREQF",  3, -2)

# äöü
hf_shift ("QFRGSFREQF", -2, 4)
hf_shift ("QFRGSFREQF",  3, 4)
