BIN=calc
OBJECTS=calc.o main.o
FLAGS=-O2

.SUFFIXES: .o .c

.c.o:
	$(CC) $(FLAGS) -c -o $@ $<

$(BIN): $(OBJECTS)
	$(CC) -o $@ $^

clean:
	rm -f *.o $(BIN)
