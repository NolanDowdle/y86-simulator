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
#include "FetchStage.h"
#include "Status.h"
#include "Debug.h"
#include "Memory.h"
#include "Tools.h"
#include "Instructions.h"
#include "ExecuteStage.h"
#include "DecodeStage.h"


/*
 * doClockLow:
 * Performs the Fetch stage combinational logic that is performed when
 * the clock edge is low.
 *
 * @param: pregs - array of the pipeline register sets (F, D, E, M, W instances)
 * @param: stages - array of stages (FetchStage, DecodeStage, ExecuteStage,
 *         MemoryStage, WritebackStage instances)
 */
bool FetchStage::doClockLow(PipeReg ** pregs, Stage ** stages)
{
   F * freg = (F *) pregs[FREG];
   D * dreg = (D *) pregs[DREG];
   M * mreg = (M *) pregs[MREG];
   W * wreg = (W *) pregs[WREG];
   uint64_t valC = 0;
   uint64_t rA = RNONE, rB = RNONE, stat = SAOK;

   //code missing here to select the value of the PC
   uint64_t f_pc = FetchStage::selectPC(freg, mreg, wreg);
   //and fetch the instruction from memory
   Memory * memInstance = Memory::getInstance();
   bool error;
   uint8_t word = memInstance->getByte(f_pc, error);
   //printf("Word: %X\n", word);
   //printf("Address: %X\n", f_pc);
   //Fetching the instruction will allow the icode, ifun,
   //rA, rB, and valC to be set.
   uint8_t ifun = Tools::getBits(word, 0, 3);
   uint8_t icode = Tools::getBits(word, 4, 7);

   if (error) {
      icode = INOP;
      ifun = FNONE;
   }

   //printf("icode: %X\n", icode);
   //printf("ifun: %X\n", ifun);
   //The lab assignment describes what methods need to be
   //written.

   bool needRegIds = FetchStage::needRegIds(icode);
   bool needValC = FetchStage::needValC(icode);

   //The value passed to setInput below will need to be changed
   uint64_t valP = FetchStage::PCincrement(f_pc, needRegIds, needValC);

   if (needRegIds == true) {
      getRegIds(rA, rB, f_pc);
      if (needValC == true) {
         valC = buildValC(f_pc, needRegIds);
      } else {
         valC = 0;
      }
   } else {
      if (needValC == true) {
         valC = buildValC(f_pc, needRegIds);
      } else {
         valC = 0;
      }
      rA = RNONE;
      rB = RNONE;
   }

   bool instrValid = instr_valid(icode);
   stat = f_stat(icode, error, instrValid);

   //calculateControlSignals(pregs, stages);
   freg->getpredPC()->setInput(FetchStage::predictPC(icode, valC, valP));

   calculateControlSignals(pregs, stages);

   //provide the input values for the D register
   setDInput(dreg, stat, icode, ifun, rA, rB, valC, valP);
   return false;
}

/* doClockHigh
 * applies the appropriate control signal to the F
 * and D register intances
 *
 * @param: pregs - array of the pipeline register (F, D, E, M, W instances)
 */
void FetchStage::doClockHigh(PipeReg ** pregs) {
   F * freg = (F *) pregs[FREG];
   D * dreg = (D *) pregs[DREG];
   
   if (!F_stall_var) {
      freg->getpredPC()->normal();
   }
   if (D_bubble_var)
   {
      bubbleD(pregs);
   }
   else if (!D_stall_var) {
      normalD(pregs);
   }
}

void FetchStage::normalD(PipeReg ** pregs) {
    D * dreg = (D *) pregs[DREG];

    dreg->getstat()->normal();
    dreg->geticode()->normal();
    dreg->getifun()->normal();
    dreg->getrA()->normal();
    dreg->getrB()->normal();
    dreg->getvalC()->normal();
    dreg->getvalP()->normal();

}

void FetchStage::bubbleD(PipeReg ** pregs) {
    D * dreg = (D *) pregs[DREG];

    dreg->getstat()->bubble(SAOK);
    dreg->geticode()->bubble(INOP);
    dreg->getifun()->bubble();
    dreg->getrA()->bubble(RNONE);
    dreg->getrB()->bubble(RNONE);
    dreg->getvalC()->bubble();
    dreg->getvalP()->bubble();
       
}

/* setDInput
 * provides the input to potentially be stored in the D register
 * during doClockHigh
 *
 * @param: dreg - pointer to the D register instance
 * @param: stat - value to be stored in the stat pipeline register within D
 * @param: icode - value to be stored in the icode pipeline register within D
 * @param: ifun - value to be stored in the ifun pipeline register within D
 * @param: rA - value to be stored in the rA pipeline register within D
 * @param: rB - value to be stored in the rB pipeline register within D
 * @param: valC - value to be stored in the valC pipeline register within D
 * @param: valP - value to be stored in the valP pipeline register within D
*/
void FetchStage::setDInput(D * dreg, uint64_t stat, uint64_t icode, 
                           uint64_t ifun, uint64_t rA, uint64_t rB,
                           uint64_t valC, uint64_t valP)
{
   dreg->getstat()->setInput(stat);
   dreg->geticode()->setInput(icode);
   dreg->getifun()->setInput(ifun);
   dreg->getrA()->setInput(rA);
   dreg->getrB()->setInput(rB);
   dreg->getvalC()->setInput(valC);
   dreg->getvalP()->setInput(valP);
}
     
uint64_t FetchStage::selectPC(F * freg, M * mreg, W * wreg) {
   uint64_t f_predPC = freg->getpredPC()->getOutput();
   uint64_t m_Cnd = mreg->getCnd()->getOutput();
   uint64_t m_valA = mreg->getvalA()->getOutput();
   uint64_t m_icode = mreg->geticode()->getOutput();
   uint64_t w_icode = wreg->geticode()->getOutput();
   uint64_t w_valM = wreg->getvalM()->getOutput();
   if (m_icode == IJXX && !m_Cnd) {
      return m_valA;
   }
   if (w_icode == IRET) {
      return w_valM;
   }
   return f_predPC;
}

bool FetchStage::needRegIds(uint64_t f_icode) {
   if (f_icode == IRRMOVQ || f_icode == IOPQ || f_icode == IPUSHQ || f_icode == IPOPQ
      || f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ) {
         return true;
   }
   return false;
}

bool FetchStage::needValC(uint64_t f_icode) {
   if (f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ || f_icode == IJXX || f_icode == ICALL) {
      return true;
   }
   return false;
}

uint64_t FetchStage::predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP) {
   if (f_icode == IJXX || f_icode == ICALL) {
      return f_valC;
   }
   return f_valP;
}

uint64_t FetchStage::PCincrement(uint64_t f_pc, bool needRegIds, bool needValC) {
   if (needValC) {
      if(needRegIds) {
         return f_pc + 10;
      }
      return f_pc + 9;
   } else {
      if(needRegIds) {
         return f_pc + 2;
      }
   }
   return f_pc + 1;
}

void FetchStage::getRegIds(uint64_t &rA, uint64_t &rB, uint64_t f_pc) {
   Memory * memInstance = Memory::getInstance();
   bool error = false;

   rA = memInstance->getByte(f_pc + 1, error);
   rA = rA >> 4;
    
   rB = memInstance->getByte(f_pc + 1, error);
   rB = rB & 0x0F;  
}

uint64_t FetchStage::buildValC(uint64_t f_pc, bool needRegIds) {
   Memory * memInstance = Memory::getInstance();
   uint64_t word = 0;
   bool error;

   if (needRegIds) {
      for (unsigned int i = 9; i >= 2; i--) {
         word += memInstance->getByte(f_pc + i, error);
         if(i != 2) {
            word = word << 8;
         }
      }
   } else {
      for (unsigned int i = 8; i >= 1; i--) {
         word += memInstance->getByte(f_pc + i, error);
         if(i != 1) {
            word = word << 8;
         }
      }
   }
   return word;
}

bool FetchStage::instr_valid(uint64_t icode) {
   if (icode == INOP || icode == IHALT || icode == IRRMOVQ 
    || icode == IIRMOVQ || icode == IRMMOVQ || icode == IMRMOVQ 
    || icode == IOPQ || icode == IJXX || icode == ICALL 
    || icode == IRET || icode == IPUSHQ || icode == IPOPQ) {
       return true;
    }
    return false;
}

uint64_t FetchStage::f_stat(uint64_t icode, bool mem_error, bool instr_valid) {
   if (mem_error) {
      return SADR;
   }
   if (!instr_valid) {
      return SINS;
   }
   if (icode == IHALT) {
      return SHLT;
   }
   return SAOK;
}

bool FetchStage::F_stall(uint64_t D_icode, uint64_t M_icode, uint64_t E_icode, uint64_t E_dstM, uint64_t d_srcA, uint64_t d_srcB) {
   if (((E_icode == IMRMOVQ || E_icode == IPOPQ) && (E_dstM == d_srcA || E_dstM == d_srcB)) ||
      (D_icode == IRET || E_icode == IRET || M_icode == IRET)) {
      return true;
   }
   return false;
}

bool FetchStage::D_stall(uint64_t E_icode, uint64_t E_dstM, uint64_t d_srcA, uint64_t d_srcB) {
   if ((E_icode == IMRMOVQ || E_icode == IPOPQ) && (E_dstM == d_srcA || E_dstM == d_srcB)) {
      return true;
   }
   return false;
}

void FetchStage::calculateControlSignals(PipeReg ** pregs, Stage ** stages) {
   ExecuteStage * es = (ExecuteStage *) stages[ESTAGE];
   DecodeStage * ds = (DecodeStage *) stages[DSTAGE];
   E * ereg = (E *) pregs[EREG];
   M * mreg = (M *) pregs[MREG];
    D * dreg = (D *) pregs[DREG];

   uint64_t E_icode = ereg->geticode()->getOutput();
   uint64_t M_icode = mreg->geticode()->getOutput();
   uint64_t D_icode = dreg->geticode()->getOutput();
   uint64_t E_dstM = ereg->getdstM()->getOutput();
   uint64_t d_srcA = ds->getd_srcA();
   uint64_t d_srcB = ds->getd_srcB();
   uint64_t e_Cnd = es->gete_Cnd();

   F_stall_var = F_stall( D_icode, M_icode, E_icode, E_dstM, d_srcA, d_srcB);
   D_stall_var = D_stall(E_icode, E_dstM, d_srcA, d_srcB);
   D_bubble_var = D_bubble(D_icode, M_icode, E_icode, e_Cnd, d_srcA, d_srcB, E_dstM);
}

bool FetchStage::D_bubble(uint64_t D_icode, uint64_t M_icode, uint64_t E_icode, uint64_t e_Cnd, uint64_t d_srcA, uint64_t d_srcB, uint64_t E_dstM) {
   return (E_icode == IJXX && !e_Cnd) ||
   (!((E_icode == IMRMOVQ || E_icode == IPOPQ) && (E_dstM == d_srcA || E_dstM == d_srcB)) &&
   (D_icode == IRET || E_icode == IRET || M_icode == IRET));
}