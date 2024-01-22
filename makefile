CFLAGS=-Wall -Wno-int-conversion

compile:
	mkdir -p build
	gcc -c microprocessor.c -o build/microprocessor.o $(CFLAGS)
	gcc -c decoder.c -o build/decoder.o
	gcc -c assembly.c -o build/assembly.o
	gcc -c main.c -o build/main.o
	gcc -o main build/decoder.o build/assembly.o build/microprocessor.o build/main.o
clean:
	rm -rf build
	rm main
	