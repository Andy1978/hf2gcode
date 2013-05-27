.PHONY:clean
.PHONY:realclean

fixed_hf = $(subst orig,fixed,$(wildcard orig/*.jhf))
rendered_hf_png = $(patsubst %.jhf,%.png,$(subst orig,rendered_png,$(wildcard orig/*.jhf)))
rendered_hf_svg = $(patsubst %.jhf,%.svg,$(subst orig,rendered_svg,$(wildcard orig/*.jhf)))
gen_c = $(patsubst %.jhf,%.h,$(subst orig,gen_c_src,$(wildcard orig/*.jhf)))

RENDER_DEP = ../src/load_hf.m ../src/plot_hf.m ../src/plot_hf_glyph.m ../src/render_hf.m

## Only the C src, do not render fonts with octave
src: $(gen_c)

## Make all, render even all fonts to PNG and SVG
all: src $(rendered_hf_png) $(rendered_hf_svg)

fixed/%.jhf: orig/%.jhf ./fix_linebreak.sh
	@mkdir -p fixed
	./fix_linebreak.sh $< $@

rendered_png/%.png rendered_svg/%.svg: fixed/%.jhf $(RENDER_DEP)
	@mkdir -p rendered_png
	@mkdir -p rendered_svg
	octave -q --eval 'addpath("../src/"); render_hf("$<","rendered")'

png_album: $(rendered_hf_png)
	photon -J -s 0 -t 800x800 -E -d 1 -l40 -o png_album rendered_png

gen_c_src/%.h:fixed/%.jhf ../src/gen_C_code.m ../src/load_hf.m
	@mkdir -p gen_c_src
	octave -q --eval 'addpath("../src/"); gen_C_code("$<","$@")'

clean:
	rm -rf fixed
	rm -rf rendered_png rendered_svg
	rm -rf png_album

realclean: clean
	./realclean.sh
