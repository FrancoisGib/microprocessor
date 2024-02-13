CFLAGS=-Wall -Wno-int-conversion -g

all: build main test

build:
	mkdir -p build
	gcc -c microprocessor.c -o build/microprocessor.o $(CFLAGS) 
	gcc -c assembly.c -o build/assembly.o
	gcc -c decoder.c -o build/decoder.o
	gcc -c control_unit.c -o build/control_unit.o

main: build
	gcc -c main.c -o build/main.o
	gcc -o main build/decoder.o build/microprocessor.o build/control_unit.o build/assembly.o build/main.o

test: build
	gcc -c test.c -o build/test.o
	gcc -o test build/decoder.o build/assembly.o build/microprocessor.o build/test.o build/control_unit.o

clean:
	rm -rf build
	rm -f main
	rm -f test
	rm -f output.txt
	