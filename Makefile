.PHONY:clean

rowmans:
	$(MAKE) -C hershey_fonts small_set

all:
	$(MAKE) -C hershey_fonts all
	$(MAKE) -C src all

clean:
	$(MAKE) -C hershey_fonts clean
	$(MAKE) -C src clean
