addpath ("../src");
fn = "fixed/futural.jhf";
#fn = "fixed/rowmans.jhf";

x = load_hf (fn);

if (numel (x) == 96)
  # ÄÖÜäöü not jetz added
  tmp = strsplit (fileread (fn), "\n");

  # copy AOUaou
  new_lines = tmp([34, 48, 54, 66, 80, 86]);

  # dot on the i
  i_dot = strsplit (strtrim (tmp{74})){2}(4:end);

  for i = 1:numel (new_lines)
    # add the dot twice with default shift
    # this is a rough assumption which needs to be fine tuned below
    if (i <= 3)
      new_lines{i} = strcat (new_lines{i}, " R", hf_shift (i_dot, -3, -3));
      new_lines{i} = strcat (new_lines{i}, " R", hf_shift (i_dot,  3, -3));
    else
      new_lines{i} = strcat (new_lines{i}, " R", hf_shift (i_dot, -2,  0));
      new_lines{i} = strcat (new_lines{i}, " R", hf_shift (i_dot,  3,  0));
    endif
  endfor

  # fix glyphlen
  add_len = (numel(i_dot) / 2 + 1) * 2;
  for i = 1:numel (new_lines)
    [S, E, ~, old_len_str] = regexp (new_lines{i}, "(?<= )[0-9]+(?=[A-Z])", "once");
    old_len = str2double (old_len_str);
    new_len = old_len + add_len;
    tmp = strcat (new_lines{i}(1:S-1), sprintf(" %2i",new_len), new_lines{i}(E+1:end));
    new_lines{i} = tmp;
  endfor

  # Attention: append 6 new lines to original file
  fid = fopen (fn, "a");
  for i = 1:numel (new_lines)
    # debugging out
    disp (new_lines{i});
    fdisp (fid, new_lines{i});
  endfor
  fclose (fid);

endif

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

plot_hf (x([74,97:102]))
