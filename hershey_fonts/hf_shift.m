# helper function to tweak single chars.
# This functions "shifts" a part of a glyph 

function r = hf_shift (s, x, y)

  m = repmat ([x y], 1, ceil (numel(s)/2));
  m = m(1:numel (s));
  r = char (uint8 (s) + m);

endfunction
