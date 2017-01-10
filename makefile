bil : bil.c
	cc -std=c99 -Wall bil.c inc/mpc.c -g -ledit -lm -o bil.out
