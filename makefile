CC = g++
CFLAGS = -g -c -Wall -std=c++11 -O0
OBJ = yess.o PipeReg.o Simulate.o F.o D.o E.o M.o W.o FetchStage.o ExecuteStage.o WritebackStage.o DecodeStage.o MemoryStage.o PipeRegField.o Loader.o ConditionCodes.o RegisterFile.o Tools.o Memory.o

.C.o:
	$(CC) $(CFLAGS) $< -o $@

yess: $(OBJ)
	$(CC) $(OBJ) -o yess

yess.o: Debug.h Memory.h RegisterFile.h ConditionCodes.h Loader.h PipeReg.h Stage.h Simulate.h

PipeReg.o: PipeReg.h

Simulate.o: PipeRegField.h PipeReg.h F.h D.h E.h M.h W.h Stage.h ExecuteStage.h MemoryStage.h DecodeStage.h FetchStage.h WritebackStage.h Simulate.h Memory.h RegisterFile.h ConditionCodes.h

F.o: PipeRegField.h PipeReg.h F.h

D.o: Instructions.h RegisterFile.h PipeReg.h PipeRegField.h D.h Status.h

E.o: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h E.h Status.h

M.o: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h M.h Status.h

W.o: RegisterFile.h Instructions.h PipeRegField.h PipeReg.h W.h Status.h

FetchStage.o: Instructions.h RegisterFile.h Memory.h Tools.h PipeRegField.h PipeReg.h F.h D.h M.h W.h E.h Stage.h FetchStage.h Status.h Debug.h

WritebackStage.o: RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h W.h E.h Stage.h FetchStage.h Status.h Debug.h

ExecuteStage.o: Instructions.h ConditionCodes.h RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h W.h E.h Stage.h FetchStage.h Status.h Debug.h

DecodeStage.o: Instructions.h RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h W.h E.h Stage.h FetchStage.h Status.h Debug.h

MemoryStage.o: RegisterFile.h PipeRegField.h PipeReg.h F.h D.h M.h W.h E.h Stage.h FetchStage.h Status.h Debug.h

PipeRegField.o: PipeRegField.h

Loader.o: Loader.h Memory.h Tools.h

ConditionsCodes.o: Conditions.h Tools.h

RegisterFile.o: RegisterFile.h Tools.h

Tools.o: Tools.h

Memory.o: Memory.h Tools.h

clean:
	rm $(OBJ) yess

run:
	make clean
	make yess
	./run.sh
