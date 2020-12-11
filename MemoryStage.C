#include <string>
#include <cstdint>
#include "RegisterFile.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "F.h"
#include "D.h"
#include "M.h"
#include "W.h"
#include "E.h"
#include "Stage.h"
#include "MemoryStage.h"
#include "Status.h"
#include "Debug.h"
#include "Instructions.h"
#include "Memory.h"

/*
 * doClockLow:
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register sets (F, D, E, M, W instances)
 * @param: stages - array of stages (FetchStage, DecodeStage, ExecuteStage,
 *         MemoryStage, WritebackStage instances)
 */
bool MemoryStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    W * wreg = (W *) pregs[WREG];
    M * mreg = (M *) pregs[MREG];

    stat = mreg->getstat()->getOutput();
    uint64_t icode = mreg->geticode()->getOutput();
    valM = 0;
    uint64_t valE = mreg->getvalE()->getOutput();
    uint64_t dstE = mreg->getdstE()->getOutput(); 
    dstM = mreg->getdstM()->getOutput();

    uint64_t valA = mreg->getvalA()->getOutput();
    uint64_t addr = mem_addr(icode, valA, valE);

    Memory * m = Memory::getInstance();
    bool error;
    if (mem_read(icode)) {
        valM = m->getLong(addr, error);
    }

    if (mem_write(icode)) {
        m->putLong(valA, addr, error);
    }

    if(error) {
        stat = SADR;
    }
    MemoryStage::setWInput(wreg, stat, icode, valM, valE, dstE, dstM);
    return false;
}

/* doClockHigh
 * applies the appropriate control signal to the F
 * and D register intances
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
void MemoryStage::doClockHigh(PipeReg ** pregs)
{
    F * freg = (F *) pregs[FREG];
    W * wreg = (W *) pregs[WREG];
    wreg->getstat()->normal();
    wreg->geticode()->normal();
    wreg->getvalE()->normal();
    wreg->getvalM()->normal();
    wreg->getdstE()->normal();
    wreg->getdstM()->normal();
}

/*
sets the input to the writeback stage register
*/
void MemoryStage::setWInput(W * wreg, uint64_t stat, uint64_t icode,
    uint64_t valM, uint64_t valE, uint64_t dstE, uint64_t dstM) {
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalM()->setInput(valM);
    wreg->getvalE()->setInput(valE);
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);
}

/*
tells us where to get the memory address from based on the instruction
*/
uint64_t MemoryStage::mem_addr(uint64_t icode, uint64_t valA, uint64_t valE) {
    
    if(icode == IRMMOVQ || icode == IPUSHQ || icode == ICALL || icode == IMRMOVQ) {
            return valE;
        } else if (icode == IPOPQ || icode == IRET) {
            return valA;
        }
    return 0;
}


/*
true if instruction is reading from memory, false otherwise
*/
bool MemoryStage::mem_read(uint64_t icode) {
    if (icode == IMRMOVQ || icode == IPOPQ || icode == IRET) {
        return true;
    }
    return false;
}

/*
true if instruction writes to memory, false otherwise
*/
bool MemoryStage::mem_write(uint64_t icode) {
    if (icode == IRMMOVQ || icode == IPUSHQ || icode == ICALL) {
        return true;
    }
    return false;
}

/*
getter for memory stages dstM
*/
uint64_t MemoryStage::get_dstM()
{
    return dstM;
}

/*
getter for memory stages valM
*/
uint64_t MemoryStage::get_valM()
{
    return valM;
}

/*
getter for memory stages stat
*/
uint64_t MemoryStage::getm_stat()
{
    return stat;
}