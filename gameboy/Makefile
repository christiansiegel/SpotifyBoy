GBDKDIR = ../gbdk

CC = ${GBDKDIR}/bin/lcc -Wa-l -Wl-m -Wl-j

BIN	= SpotifyBoy.gb

all: $(BIN)

%.o:	%.c
	$(CC) -c -o $@ $<

%.s:	%.c
	$(CC) -S -o $@ $<

%.o:	%.s
	$(CC) -c -o $@ $<

%.gb:	%.o
	$(CC) -o $@ $<

format:
	clang-format --style=google -i *.c

clean:
	rm -f *.o *.lst *.map *.gb *~ *.rel *.cdb *.ihx *.lnk *.sym *.asm

