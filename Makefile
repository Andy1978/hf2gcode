.PHONY:clean

simple:
	$(MAKE) -C src

all:
	$(MAKE) -C hershey_fonts all
	$(MAKE) -C src all

trailing_space_clean:
	find . \( -name "*.m" -or -name "*.c*" -or -name "*.h" \) -exec sed -i 's/[[:space:]]*$$//' {} \;

clean:
	$(MAKE) -C hershey_fonts clean
	$(MAKE) -C src clean
