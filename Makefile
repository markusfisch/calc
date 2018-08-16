BIN = calc
OBJECTS = calc.o main.o
CFLAGS = -O2 -Wall -Wextra -pedantic

.c.o: *.c *.h
	$(CC) $(CFLAGS) -c -o $@ $<

$(BIN): $(OBJECTS)
	$(CC) -o $@ $^

test: $(BIN)
	./test.sh

clean:
	rm -f *.o $(BIN)
