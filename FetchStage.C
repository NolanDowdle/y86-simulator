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
   //printf("icode: %X\n", icode);
   //printf("ifun: %X\n", ifun);
   //The lab assignment describes what methods need to be
   //written.

   bool needRegIds = FetchStage::needRegIds(icode);
   bool needValC = FetchStage::needValC(icode);

   //The value passed to setInput below will need to be changed
   uint64_t valP = FetchStage::PCincrement(f_pc, needRegIds, needValC);

   if (needRegIds == true) {
      uint8_t regIds = getRegIds(f_pc);
      rA = Tools::getBits(regIds, 4, 7);
      rB = Tools::getBits(regIds, 0, 3);
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

   freg->getpredPC()->setInput(FetchStage::predictPC(icode, valC, valP));

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
void FetchStage::doClockHigh(PipeReg ** pregs)
{
   F * freg = (F *) pregs[FREG];
   D * dreg = (D *) pregs[DREG];

   freg->getpredPC()->normal();
   dreg->getstat()->normal();
   dreg->geticode()->normal();
   dreg->getifun()->normal();
   dreg->getrA()->normal();
   dreg->getrB()->normal();
   dreg->getvalC()->normal();
   dreg->getvalP()->normal();
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

uint8_t FetchStage::getRegIds(uint64_t f_pc) {
   Memory * memInstance = Memory::getInstance();
   bool error;
   return memInstance->getByte(f_pc + 1, error);
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