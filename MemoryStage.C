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
    F * freg = (F *) pregs[FREG];
    W * wreg = (W *) pregs[WREG];
    M * mreg = (M *) pregs[MREG];

    uint64_t stat = mreg->getstat()->getOutput(), icode = mreg->geticode()->getOutput();
    uint64_t valM = 0;
    uint64_t valE = mreg->getvalE()->getOutput();
    uint64_t dstE = mreg->getdstE()->getOutput(), dstM = mreg->getdstM()->getOutput();

    uint64_t f_pc = freg->getpredPC()->getOutput();

    freg->getpredPC()->setInput(f_pc);
    setWInput(wreg, stat, icode, valM, valE, dstE, dstM);
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

    freg->getpredPC()->normal();
    wreg->getstat()->normal();
    wreg->geticode()->normal();
    wreg->getvalE()->normal();
    wreg->getvalM()->normal();
    wreg->getdstE()->normal();
    wreg->getdstM()->normal();
}

void MemoryStage::setWInput(W * wreg, uint64_t stat, uint64_t icode,
    uint64_t valM, uint64_t valE, uint64_t dstE, uint64_t dstM) {
    wreg->getstat()->setInput(stat);
    wreg->geticode()->setInput(icode);
    wreg->getvalM()->setInput(valM);
    wreg->getvalE()->setInput(valE);
    wreg->getdstE()->setInput(dstE);
    wreg->getdstM()->setInput(dstM);
}