CC = gcc
CFLAG = -Wall -Wextra
EXE = bmp
DEPS = bmp_header.h

build: $(EXE)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAG)

$(EXE): tema3.c
	$(CC) $^ $(CFLAG) -o $(EXE)

run:
	./bmp

.PHONY: clean

clean:
	rm -f *.o $(EXE)
