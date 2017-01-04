bf : bf.c
	cc -std=c99 -Wall bf.c inc/mpc.c -g -ledit -lm -o bf
