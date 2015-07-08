# Copyright 2015 Wojciech A. Koszek <wojciech@koszek.com>

CFLAGS+= -Wall -pedantic -std=c99

all:
	$(CC) $(CFLAGS) ctx_switch_time.c -o ctx_switch_time
