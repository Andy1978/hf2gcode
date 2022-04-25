.PHONY:clean
.PHONY:realclean

# keep intermediate files (for example "fixed")
.SECONDARY:

fixed_hf = $(subst orig,fixed,$(wildcard orig/*.jhf))
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
	find ./patches -name "$(basename $(notdir $@))_*.patch" -exec sh -c "patch -p1 < {}" \;

rendered_svg/%.svg: fixed/%.jhf $(RENDER_DEP)
	octave -q --eval 'addpath("../src/"); render_hf("$<", "rendered_png", "rendered_svg")'

gen_c_src/%.h:fixed/%.jhf ../src/gen_C_code.m ../src/load_hf.m
	@mkdir -p gen_c_src
	octave -q --eval 'addpath("../src/"); gen_C_code("$<","$@")'

clean:
	rm -rf fixed
	rm -rf rendered_png rendered_svg
	rm -rf png_album

realclean: clean
	./realclean.sh
