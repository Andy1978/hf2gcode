.PHONY: clean

CFLAGS = -Wall -Wextra -O0 -ggdb

run: hf2gcode demo.txt
	./hf2gcode -i demo.txt -o demo_hello_world.ngc -y 23
	./hf2gcode -m -s 0.3 -p 2 -f 800 -i demo.txt -o demo_hello_world_needler.ngc -y 23
	./hf2gcode -i demo.txt --font=scripts -o demo_hello_world_scripts.ngc -y 23

hf2gcode: hf2gcode.c libhf2gcode.c
	$(MAKE) -C ../hershey_fonts src
	gcc $(CFLAGS) $^ -o $@

clean:
	rm -f hf2gcode
	rm -f *.ngc

