CC = g++
CFLAGS = -g -c -Wall -std=c++11 -O0
OBJ = lab6.o Memory.o Tools.o RegisterFile.o ConditionCodes.o Loader.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

lab6: $(OBJ)
	$(CC) $(OBJ) -o lab6

lab6.o: Memory.h RegisterFile.h ConditionCodes.h Loader.h

Loader.o: Loader.h Memory.h

ConditionsCodes.o: Conditions.h Tools.h

RegisterFile.o: RegisterFile.h Tools.h

Tools.o: Tools.h

Memory.o: Memory.h Tools.h


clean:
	rm $(OBJ) lab6

run:
	make clean
	make lab6
	./run.sh

