all: greet clean create  compile

greet:
	@echo "WaveCPP make program, compiling all sources now"

clean:
	-rm target

create:
	mkdir target

CXX = g++

compile: 
	${CXX} src/main.cpp -o target/wavecpp 
