.PHONY:clean

fixed_hf = $(subst orig,fixed,$(wildcard orig/*.jhf))
rendered_hf_png = $(patsubst %.jhf,%.png,$(subst orig,rendered,$(wildcard orig/*.jhf)))
rendered_hf_svg = $(patsubst %.png,%.svg,$(rendered_hf_png))

RENDER_DEP = ../src/load_hf.m ../src/plot_hf.m ../src/plot_hf_glyph.m ../src/render_hf.m

## Only the really important portion
small_set: $(fixed_hf) rendered/rowmans.png

all: small_set $(rendered_hf_png) $(rendered_hf_svg)

fixed/%.jhf: orig/%.jhf ./fix_linebreak.sh
	@mkdir -p fixed
	./fix_linebreak.sh $< $@

rendered/%.png rendered/%.svg: fixed/%.jhf $(RENDER_DEP)
	@mkdir -p rendered
	octave -q --eval 'addpath("../src/"); render_hf("$<","rendered")'

gen_c_src/%.h:fixed/%.jhf ../src/gen_C_code.m ../src/load_hf.m
	@mkdir -p gen_c_src
	octave -q --eval 'addpath("../src/"); gen_C_code("$<","$@")'

clean:
	rm -rf fixed
	rm -rf rendered
	rm -rf gen_c_src
