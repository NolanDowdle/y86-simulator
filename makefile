CC = g++
CFLAGS = -g -c -Wall -std=c++11 -O0
OBJ = lab4.o MemoryTester.o Memory.o Tools.o RegisterFile.o \
RegisterFileTester.o ConditionCodes.o ConditionCodesTester.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

lab4: $(OBJ)
	$(CC) $(OBJ) -o lab4

lab4.o: Memory.h RegisterFile.h MemoryTester.h RegisterFileTester.h ConditionCodes.h ConditionCodesTester.h

ConditionsCodes.o: Conditions.h Tools.h

ConditionsCodesTester.o: ConditionCodes.h conditioncodesTester.h

RegisterFileTester.o: RegisterFile.h RegisterFileTester.h

RegisterFile.o: RegisterFile.h Tools.h

Tools.o: Tools.h

Memory.o: Memory.h Tools.h

MemoryTester.o: Memory.h MemoryTester.h


clean:
	rm $(OBJ) lab4

run:
	make clean
	make lab4
	./run.sh

