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
#include "DecodeStage.h"
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
bool DecodeStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
    F * freg = (F *) pregs[FREG];
    D * dreg = (D *) pregs[DREG];
    E * ereg = (E *) pregs[EREG];
    uint64_t f_pc = freg->getpredPC()->getOutput();
    uint64_t stat = dreg->getstat()->getOutput(), icode = dreg->geticode()->getOutput(), ifun = dreg->getifun()->getOutput(), valC = dreg->getvalC()->getOutput();
    uint64_t valA = 0, valB = 0, dstE = RNONE, dstM = RNONE, srcA = RNONE, srcB = RNONE;

    freg->getpredPC()->setInput(f_pc);

    setEInput(ereg, stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB);
    return false;
}

/* doClockHigh
 * applies the appropriate control signal to the F
 * and D register intances
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
void DecodeStage::doClockHigh(PipeReg ** pregs)
{
    F * freg = (F *) pregs[FREG];
    //D * dreg = (D *) pregs[DREG];
    E * ereg = (E *) pregs[EREG];

    freg->getpredPC()->normal();
    ereg->getstat()->normal();
    ereg->geticode()->normal();
    ereg->getifun()->normal();
    ereg->getvalC()->normal();
    ereg->getvalA()->normal();
    ereg->getvalB()->normal();
    ereg->getdstE()->normal();
    ereg->getdstM()->normal();
    ereg->getsrcA()->normal();
    ereg->getsrcB()->normal();
}

void DecodeStage::setEInput(E * ereg, uint64_t stat, uint64_t icode,
                            uint64_t ifun, uint64_t valC, uint64_t valA,
                            uint64_t valB, uint64_t dstE, uint64_t dstM,
                            uint64_t srcA, uint64_t srcB) {
    ereg->getstat()->setInput(stat);
    ereg->geticode()->setInput(icode);
    ereg->getifun()->setInput(ifun);
    ereg->getvalC()->setInput(valC);
    ereg->getvalA()->setInput(valA);
    ereg->getvalB()->setInput(valB);
    ereg->getdstE()->setInput(dstE);
    ereg->getdstM()->setInput(dstM);
    ereg->getsrcA()->setInput(srcA);
    ereg->getsrcB()->setInput(srcB);
}