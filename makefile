CFLAGS=-Wall -Wno-int-conversion -g

lib:
	mkdir -p build
	gcc -c microprocessor.c -o build/microprocessor.o $(CFLAGS) 
	gcc -c decoder.c -o build/decoder.o
	gcc -o lib lib.c build/decoder.o build/microprocessor.o

compile:
	mkdir -p build
	gcc -c microprocessor.c -o build/microprocessor.o $(CFLAGS) 
	gcc -c decoder.c -o build/decoder.o
	gcc -c assembly.c -o build/assembly.o
	gcc -c lib.c -o build/lib.o -lm
	gcc -c control_unit.c -o build/control_unit.o -lm
	gcc -c main.c -o build/main.o
	gcc -c test.c -o build/test.o
	gcc -o main build/decoder.o build/assembly.o build/microprocessor.o build/lib.o build/main.o build/control_unit.o -lm
	gcc -o test build/decoder.o build/assembly.o build/microprocessor.o build/lib.o build/test.o build/control_unit.o -lm
	rm -rf build

remake:
	mkdir -p build
	gcc -c microprocessor.c -o build/microprocessor.o $(CFLAGS) 
	gcc -c decoder_remake.c -o build/decoder_remake.o
	gcc -o decoder_remake build/decoder_remake.o build/microprocessor.o

clean:
	rm -rf build
	rm main
	rm lib
	rm decoder_remake
	