.PHONY:clean

rowmans:
	$(MAKE) -C hershey_fonts small_set

all:

clean:
	$(MAKE) -C hershey_fonts clean
	$(MAKE) -C src clean
