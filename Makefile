all: greet clean create  compile wave_main

greet:
	@echo "WaveCPP make program, compiling all sources now"

clean:
	-rm -r target

create:
	mkdir target

CXX = g++

compile: 
	${CXX} src/main.cpp -o target/wavecpp

wave_main:
	./target/wavecpp 
