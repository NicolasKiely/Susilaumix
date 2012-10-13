cmplr=gcc
flags=-g -Wall


slxserver: bin/kernel/main.o bin/kernel/fileReader.o bin/kernel/recordParser.o
	$(cmplr) -g -o slxserver bin/kernel/main.o bin/kernel/fileReader.o bin/kernel/recordParser.o
	
	
bin/kernel/main.o: src/kernel/main.c
	$(cmplr) $(flags) -o bin/kernel/main.o -c src/kernel/main.c
	

bin/kernel/fileReader.o: src/kernel/fileReader.c src/kernel/fileReader.h
	$(cmplr) $(flags) -o bin/kernel/fileReader.o -c src/kernel/fileReader.c


bin/kernel/recordParser.o: src/kernel/recordParser.c src/kernel/recordParser.h
	$(cmplr) $(flags) -o bin/kernel/recordParser.o -c src/kernel/recordParser.c
	
	
include Makefile_utils
