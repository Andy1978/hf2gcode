.PHONY:clean

rendered_hf_png = $(patsubst %.jhf,%.png,$(subst orig,rendered,$(wildcard orig/*.jhf)))
rendered_hf_svg = $(patsubst %.png,%.svg,$(rendered_hf_png))

RENDER_DEP = ../src/load_hf.m ../src/plot_hf.m ../src/plot_hf_glyph.m ../src/render_hf.m

all: $(rendered_hf_png) $(rendered_hf_svg)

fixed/%.jhf: orig/%.jhf ./fix_linebreak.sh
	@mkdir -p fixed
	./fix_linebreak.sh $< $@

rendered/%.png rendered/%.svg: fixed/%.jhf $(RENDER_DEP)
	@mkdir -p rendered
	octave -q --eval 'addpath("../src/"); render_hf("$<","rendered")'

clean:
	rm -rf fixed
	rm -rf rendered
