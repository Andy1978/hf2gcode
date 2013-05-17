.PHONY:clean

all: fixed

fixed: fix_linebreak.sh
	mkdir fixed
	find orig -name "*.jhf" -execdir ../fix_linebreak.sh {} \;

clean:
	rm -rf fixed
