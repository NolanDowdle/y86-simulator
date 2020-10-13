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

    freg->getpredPC()->setInput(f_pc);

    uint64_t srcA = DecodeStage::d_srcA(icode, dreg->getrA()->getOutput());
    uint64_t srcB = DecodeStage::d_srcB(icode, dreg->getrB()->getOutput());
    uint64_t dstE = DecodeStage::d_dstE(icode, dreg->getrB()->getOutput());
    uint64_t dstM = DecodeStage::d_dstM(icode, dreg->getrA()->getOutput());
    uint64_t valA = DecodeStage::d_valA(dreg->getrA()->getOutput());
    uint64_t valB = DecodeStage::d_valB(dreg->getrB()->getOutput());
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

uint64_t DecodeStage::d_srcA(uint64_t icode, uint64_t rA) {
    uint64_t IRRMOVQ = 2;
    uint64_t IOPQ = 6;
    uint64_t IPUSHQ = 10;
    uint64_t IRMMOVQ = 4;
    uint64_t IRET = 9;
    uint64_t rsp = 4;
    if(icode == IRRMOVQ || icode == IRMMOVQ || icode == IOPQ || icode == IPUSHQ) {
        return rA;
    } else if (icode == IOPQ || icode == IRET) {
        return rsp;
    } else {
        return RNONE;
    }
}

uint64_t DecodeStage::d_srcB(uint64_t icode, uint64_t rB) {
    uint64_t IOPQ = 6;
    uint64_t IMRMOVQ = 5;
    uint64_t IPUSHQ = 10;
    uint64_t IRMMOVQ = 4;
    uint64_t IRET = 9;
    uint64_t ICALL = 8;
    uint64_t rsp = 4;
    if(icode == IOPQ || icode == IMRMOVQ || icode == IRMMOVQ) {
        return rB;
    } else if (icode == IOPQ || icode == IRET || icode == ICALL || icode == IPUSHQ) {
        return rsp;
    } else {
        return RNONE;
    }
}

uint64_t DecodeStage::d_dstE(uint64_t icode, uint64_t rB) {
    uint64_t IOPQ = 6;
    uint64_t IRRMOVQ = 2;
    uint64_t IIRMOVQ = 3;
    uint64_t IPUSHQ = 10;
    uint64_t IPOPQ = 6;
    uint64_t ICALL = 8;
    uint64_t IRET = 9;
    uint64_t rsp = 4;
    if (icode == IOPQ || icode == IRRMOVQ || icode == IIRMOVQ) {
        return rB;
    } else if (icode == IPUSHQ || icode == IPOPQ || icode == ICALL || icode == IRET) {
        return rsp;
    } else  {
        return RNONE;
    }
}

uint64_t DecodeStage::d_dstM(uint64_t icode, uint64_t rA) {
    uint64_t IMRMOVQ = 5;
    uint64_t IPOPQ = 6;
    if (icode == IMRMOVQ || icode == IPOPQ) {
        return rA;
    } else  {
        return RNONE;
    }
}

uint64_t DecodeStage::d_valA(uint64_t rA) {
    RegisterFile * regInstance = RegisterFile::getInstance();
    bool error;
    return regInstance->readRegister(rA, error);//value from register file
}

uint64_t DecodeStage::d_valB(uint64_t rB) {
    RegisterFile * regInstance = RegisterFile::getInstance();
    bool error;
    return regInstance->readRegister(rB, error);//value from register file
}