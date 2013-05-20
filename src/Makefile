.PHONY: clean
.PHONY: realclean

all: render_hf

render_hf: load_hf.m plot_hf.m plot_hf_glyph.m render_hf.m render_all_hf.m
	$(MAKE) -C ../hershey_fonts/
	octave -q render_all_hf.m

clean:

realclean:
	rm -rf rendered_hf
