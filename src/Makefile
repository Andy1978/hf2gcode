.PHONY: clean

CFLAGS = -Wall -Wextra -O0 -ggdb

run: hf2gcode
	./hf2gcode -v rowmans "Hello world!" -o hello.ngc
	./hf2gcode -v scriptc "Hello world!" -o hello_scriptc.ngc

hf2gcode: hf2gcode.c libhf2gcode.c
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f hf2gcode
	rm -f *.ngc

