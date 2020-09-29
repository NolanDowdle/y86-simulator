CC = g++
CFLAGS = -g -c -Wall -std=c++11 -O0
OBJ = yess.o Memory.o Debug.h RegisterFile.o ConditionCodes.o Loader.o PipeReg.o Stage.o Simulate.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

yess: $(OBJ)
	$(CC) $(OBJ) -o yess

yess.o: Debug.h Memory.h RegisterFile.h ConditionCodes.h Loader.h PipeReg.h Stage.h Simulate.h

PipeReg.o: PipeReg.h

Simulate.o: F.h D.h E.h M.h W.h Stage.h ExecuteStage.h MemoryStage.h DecodeStage.h FetchStage.h WritebackStage.h Simulate.h Memory.h RegisterFile.h ConditionCodes.h

F.o: PipeRegField.h PipeReg.h F.h

D.o: Instructions.h RegisterFile.h PipeReg.h PipeRegField.h D.h Status.h

E.o: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h E.h Status.h

M.o: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h M.h Status.h

W.h: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h W.h Status.h

PipeRegField.o: PipeRegField.h

Loader.o: Loader.h Memory.h Tools.h

ConditionsCodes.o: Conditions.h Tools.h

RegisterFile.o: RegisterFile.h Tools.h

Tools.o: Tools.h

Memory.o: Memory.h Tools.h


clean:
	rm *.o yess

run:
	make clean
	make yess
	./run.sh

