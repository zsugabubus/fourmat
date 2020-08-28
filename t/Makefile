RM ?= rm -f

run : check
	./$^

c1/% :
	git submodule update --init c1

check : . *.test.? c1/test.? ../*.?
	$(CC) -o $@ -g -Wall -Wno-format -O2 -Ic1 -I.. c1/test.h c1/test.c *.c ../*.c

clean :
	$(RM) check
.PHONY : run clean
