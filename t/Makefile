RM ?= rm -f

run : check.out
	./$^

check.out : . *.test.? c1/test.h c1/test.c ../*.?
	$(CC) -o $@ -g -Wall -Wno-format -ldl -O2 -Ic1 -I.. c1/test.h c1/test.c *.c ../*.c

clean :
	$(RM) check.out
.PHONY : run clean
