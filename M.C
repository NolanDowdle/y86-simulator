#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include "RegisterFile.h"
#include "Instructions.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "M.h"
#include "Status.h"
/*
 * M constructor
 *
 * initialize the M pipeline register
*/
M::M()
{
   stat = new PipeRegField(SAOK);
   icode = new PipeRegField(INOP);
   Cnd = new PipeRegField();
   valE = new PipeRegField();
   valA = new PipeRegField();
   dstE = new PipeRegField(RNONE);
   dstM = new PipeRegField(RNONE);
}

/* return the stat pipeline register field */
PipeRegField * M::getstat()
{
   return stat;
}

/* return the icode pipeline register field */
PipeRegField * M::geticode()
{
   return icode;
}

/* return the Cnd pipeline register field */
PipeRegField * M::getCnd()
{
   return Cnd;
}

/* return the valE pipeline register field */
PipeRegField * M::getvalE()
{
   return valE;
}

/* return the valA pipeline register field */
PipeRegField * M::getvalA()
{
   return valA;
}

/* return the dstE pipeline register field */
PipeRegField * M::getdstE()
{
   return dstE;
}

/* return the dstM pipeline register field */
PipeRegField * M::getdstM()
{
   return dstM;
}

/* 
 * dump
 *
 * outputs the current values of the M pipeline register
*/
void M:: dump()
{
   dumpField("M: stat: ", 1, stat->getOutput(), false);
   dumpField(" icode: ", 1, icode->getOutput(), false);
   dumpField(" Cnd: ", 1, Cnd->getOutput(), false);
   dumpField(" valE: ", 16, valE->getOutput(), false);
   dumpField(" valA: ", 16, valA->getOutput(), false);
   dumpField(" dstE: ", 1, dstE->getOutput(), false);
   dumpField(" dstM: ", 1, dstM->getOutput(), true);
}
