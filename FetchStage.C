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
   uint64_t f_pc = 0, icode = 0, ifun = 0, valC = 0, valP = 0;
   uint64_t rA = RNONE, rB = RNONE, stat = SAOK;

   //code missing here to select the value of the PC
   //and fetch the instruction from memory
   //Fetching the instruction will allow the icode, ifun,
   //rA, rB, and valC to be set.
   //The lab assignment describes what methods need to be
   //written.

   //The value passed to setInput below will need to be changed
   freg->getpredPC()->setInput(f_pc + 1);

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
   uint64_t IJXX = 7;
   uint64_t IRET = 9;
   if (m_icode == IJXX && !m_Cnd) {
      return m_valA;
   }
   if (w_icode == IRET) {
      return w_valM;
   }
   return f_predPC;
}

bool FetchStage::needRegIds(uint64_t f_icode) {
   uint64_t IRRMOVQ = 2;
   uint64_t IOPQ = 6;
   uint64_t IPUSHQ = 10;
   uint64_t IPOPQ = 11;
   uint64_t IIRMOVQ = 3;
   uint64_t IRMMOVQ = 4;
   uint64_t IMRMOVQ = 5;
   if (f_icode == IRRMOVQ || f_icode == IOPQ || f_icode == IPUSHQ || f_icode == IPOPQ
      || f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ) {
         return true;
   }
   return false;
}

bool FetchStage::needValC(uint64_t f_icode) {
   uint64_t IIRMOVQ = 3;
   uint64_t IRMMOVQ = 4;
   uint64_t IMRMOVQ = 5;
   uint64_t IJXX = 7;
   uint64_t ICALL = 8;
   if (f_icode == IIRMOVQ || f_icode == IRMMOVQ || f_icode == IMRMOVQ || f_icode == IJXX || f_icode == ICALL) {
      return true;
   }
   return false;
}

uint64_t FetchStage::predictPC(uint64_t f_icode, uint64_t f_valC, uint64_t f_valP) {
   uint64_t IJXX = 7;
   uint64_t ICALL = 8;
   if (f_icode == IJXX || f_icode == ICALL) {
      return f_valC;
   }
   return f_valP;
}