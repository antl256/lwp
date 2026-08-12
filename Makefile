.PHONY: all clean

CC = clang
CFLAGS = -std=c11 -Wall -Werror
CINCLUDE = -Iinclude

all: out/lwpdbg out/lwp

clean:
	rm -r out

out/lwpdbg: src/* include/*
	@mkdir -p out
	$(CC) $(CFLAGS) -O0 $(CINCLUDE) -o out/lwpdbg -lasound \
	src/lwpplayer.c \
	src/lwpwav.c \
	src/lwpmain.c

out/lwp: src/* include/*
	@mkdir -p out
	$(CC) $(CFLAGS) -O2 $(CINCLUDE) -o out/lwp -lasound \
	src/lwpplayer.c \
	src/lwpwav.c \
	src/lwpmain.c

