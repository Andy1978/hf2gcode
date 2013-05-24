.PHONY: clean

CFLAGS = -Wall -Wextra -O0 -ggdb

run: hf2gcode
	./hf2gcode rowmans -v "He" -o He.ngc

#./hf2gcode -v scriptc "Hello world!" -o hello_scriptc.ngc
#./hf2gcode rowmans -v 'Hello world!\nHow are you?' -o hello.ngc

hf2gcode: hf2gcode.c libhf2gcode.c
	$(MAKE) -C ../hershey_fonts gen_c_src/rowmans.h gen_c_src/scriptc.h
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f hf2gcode
	rm -f *.ngc

