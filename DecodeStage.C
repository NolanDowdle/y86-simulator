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
#include "Instructions.h"
#include "ExecuteStage.h"
#include "MemoryStage.h"

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
    ExecuteStage * es = (ExecuteStage *) stages[ESTAGE];
    uint64_t f_pc = freg->getpredPC()->getOutput();
    uint64_t stat = dreg->getstat()->getOutput(), icode = dreg->geticode()->getOutput(), ifun = dreg->getifun()->getOutput(), valC = dreg->getvalC()->getOutput();

    freg->getpredPC()->setInput(f_pc);

    uint64_t srcA = d_srcA(icode, dreg->getrA()->getOutput());
    uint64_t srcB = d_srcB(icode, dreg->getrB()->getOutput());
    uint64_t dstE = d_dstE(icode, dreg->getrB()->getOutput());
    uint64_t dstM = d_dstM(icode, dreg->getrA()->getOutput());
    uint64_t valA = d_valA(icode, dreg->getrA()->getOutput(), pregs, stages);
    uint64_t valB = d_valB(icode, dreg->getrB()->getOutput(), pregs, stages);

    d_srcA_var = d_srcA(icode, srcA);
    d_srcB_var = d_srcB(icode, srcB);
    uint64_t E_icode = ereg->geticode()->getOutput();
    uint64_t E_dstM = ereg->getdstM()->getOutput();
    uint64_t e_Cnd = es->gete_Cnd();

    //ExecuteStage * es = (ExecuteStage*) stages[ESTAGE];
    //uint64_t e_Cnd = es->gete_Cnd();
    E_bubble_var = calculateControlSignals(E_icode, E_dstM, d_srcA_var, d_srcB_var, e_Cnd);

    DecodeStage::setEInput(ereg, stat, icode, ifun, valC, valA, valB, dstE, dstM, srcA, srcB);
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

    //freg->getpredPC()->normal();
    
    if (E_bubble_var) {
        bubbleE(pregs);
    }
    else {
        normalE(pregs);
    }
}

void DecodeStage::normalE(PipeReg ** pregs) {
    E * ereg = (E *) pregs[EREG];

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

void DecodeStage::bubbleE(PipeReg ** pregs) {
    E * ereg = (E *) pregs[EREG];

    ereg->getstat()->bubble(SAOK);
    ereg->geticode()->bubble(INOP);
    ereg->getifun()->bubble();
    ereg->getvalC()->bubble();
    ereg->getvalA()->bubble();
    ereg->getvalB()->bubble();
    ereg->getdstE()->bubble(RNONE);
    ereg->getdstM()->bubble(RNONE);
    ereg->getsrcA()->bubble(RNONE);
    ereg->getsrcB()->bubble(RNONE); 
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
    if(icode == IRRMOVQ || icode == IRMMOVQ || icode == IOPQ || icode == IPUSHQ) {
        return rA;
    } else if (icode == IPOPQ || icode == IRET) {
        return RSP;
    } else {
        return RNONE;
    }
}

uint64_t DecodeStage::d_srcB(uint64_t icode, uint64_t rB) {
    if(icode == IOPQ || icode == IMRMOVQ || icode == IRMMOVQ) {
        return rB;
    } else if (icode == IPOPQ || icode == IRET || icode == ICALL || icode == IPUSHQ) {
        return RSP;
    } else {
        return RNONE;
    }
}

uint64_t DecodeStage::d_dstE(uint64_t icode, uint64_t rB) {
    if (icode == IOPQ || icode == IRRMOVQ || icode == IIRMOVQ) {
        return rB;
    } else if (icode == IPUSHQ || icode == IPOPQ || icode == ICALL || icode == IRET) {
        return RSP;
    } else  {
        return RNONE;
    }
}

uint64_t DecodeStage::d_dstM(uint64_t icode, uint64_t rA) {
    if (icode == IMRMOVQ || icode == IPOPQ) {
        return rA;
    } else  {
        return RNONE;
    }
}

uint64_t DecodeStage::d_valA(uint64_t icode, uint64_t rA, PipeReg ** pregs, Stage ** stages) {
    RegisterFile * regInstance = RegisterFile::getInstance();
    ExecuteStage * e = (ExecuteStage*) stages[ESTAGE];
    MemoryStage * m = (MemoryStage*) stages[MSTAGE];
    M * mreg = (M *) pregs[MREG];
    W * wreg = (W *) pregs[WREG];
    D * dreg = (D *) pregs[DREG];
    bool error;
    uint64_t d_srcA1 = d_srcA(icode, rA);

    if (icode == ICALL || icode == IJXX) {
        //return valP
        return dreg->getvalP()->getOutput();
    }

    if (d_srcA1 == RNONE) {
        return 0;
    }

    uint64_t e_dstE1 = e->gete_dstE();
    uint64_t m_dstM1 = m->get_dstM();
    uint64_t M_dstE1 = mreg->getdstE()->getOutput();
    uint64_t W_dstE1 = wreg->getdstE()->getOutput();
    uint64_t W_dstM1 = wreg->getdstM()->getOutput();


    if (d_srcA1 == e_dstE1) {
        return e->gete_valE();
    }
    if(d_srcA1 == m_dstM1) {
        return m->get_valM();
    }
    if (d_srcA1 == M_dstE1) {
        return mreg->getvalE()->getOutput();
    }

    if (d_srcA1 == W_dstM1) {
        return wreg->getvalM()->getOutput();
    }

    if (d_srcA1 == W_dstE1) {
        return wreg->getvalE()->getOutput();
    }
    //printf("rA: %X\n", egInstance->readRegister(rA, error))
    return regInstance->readRegister(d_srcA1, error);//value from register file
}

uint64_t DecodeStage::d_valB(uint64_t icode, uint64_t rB, PipeReg ** pregs, Stage ** stages) {
    RegisterFile * regInstance = RegisterFile::getInstance();
    ExecuteStage * e = (ExecuteStage*) stages[ESTAGE];
    MemoryStage * m = (MemoryStage*) stages[MSTAGE];
    M * mreg = (M *) pregs[MREG];
    W * wreg = (W *) pregs[WREG];
    bool error;
    uint64_t d_srcB1 = d_srcB(icode, rB);

    if (d_srcB1 == RNONE) {
        return 0;
    }

    uint64_t e_dstE1 = e->gete_dstE();
    uint64_t m_dstM1 = m->get_dstM();
    uint64_t M_dstE1 = mreg->getdstE()->getOutput();
    uint64_t W_dstE1 = wreg->getdstE()->getOutput();
    uint64_t W_dstM1 = wreg->getdstM()->getOutput();


    if (d_srcB1 == e_dstE1) {
        return e->gete_valE();
    }
    if (d_srcB1 == m_dstM1) {
        return m->get_valM();
    }

    if (d_srcB1 == M_dstE1) {
        return mreg->getvalE()->getOutput();
    }

    if (d_srcB1 == W_dstM1) {
        return wreg->getvalM()->getOutput();
    }

    if (d_srcB1 == W_dstE1) {
        return wreg->getvalE()->getOutput();
    }
    //printf("rB: %X\n", regInstance->readRegister(rB, error))
    return regInstance->readRegister(d_srcB1, error);//value from register file
}

uint64_t DecodeStage::getd_srcA() {
    return d_srcA_var;
}

uint64_t DecodeStage::getd_srcB() {
    return d_srcB_var;
}

bool DecodeStage::calculateControlSignals(uint64_t E_icode, uint64_t E_dstM, uint64_t d_srcA, uint64_t d_srcB, uint64_t e_Cnd) {
    //(E_icode == IJXX && !e_Cnd)
    return ((E_icode == IJXX && !e_Cnd) || 
    ((E_icode == IMRMOVQ || E_icode == IPOPQ) && (E_dstM == d_srcA || E_dstM == d_srcB)));
}