RM ?= rm -f

run : check
	./$^

c1/% :
	git submodule update --init c1

check : c1/test.? . *.c ../*.?
	$(CC) -o $@ -g -Wall -Wno-format -O2 -Ic1 -I.. c1/test.c *.c ../*.c

clean :
	$(RM) check

.PHONY : run clean
