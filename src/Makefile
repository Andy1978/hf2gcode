.PHONY: clean

CFLAGS = -Wall -Wextra -O0 -ggdb

run: hf2gcode
	./hf2gcode "Hello world!\nThis is hf2gcode." -o demo_hello_world.ngc -y 23
	./hf2gcode "Hello world!\nThis is hf2gcode." -o demo_hello_world_scriptc.ngc -y 23 --font=scriptc

hf2gcode: hf2gcode.c libhf2gcode.c
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f hf2gcode
	rm -f *.ngc

