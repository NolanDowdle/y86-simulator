#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include "RegisterFile.h"
#include "Instructions.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "W.h"
#include "Status.h"

/*
 * W constructor
 *
 * initialize the W pipeline register 
*/
W::W()
{
   stat = new PipeRegField(SAOK);
   icode = new PipeRegField(INOP);
   valE = new PipeRegField();
   valM = new PipeRegField();
   dstE = new PipeRegField(RNONE);
   dstM = new PipeRegField(RNONE);
}

/* return the stat pipeline register field */
PipeRegField * W::getstat()
{
   return stat;
}

/* return the icode pipeline register field */
PipeRegField * W::geticode()
{
   return icode;
}

/* return the valE pipeline register field */
PipeRegField * W::getvalE()
{
   return valE;
}

/* return the valM pipeline register field */
PipeRegField * W::getvalM()
{
   return valM;
}

/* return the dstE pipeline register field */
PipeRegField * W::getdstE()
{
   return dstE;
}

/* return the dstM pipeline register field */
PipeRegField * W::getdstM()
{
   return dstM;
}

/* 
 * dump
 *
 * outputs the current values of the W pipeline register
*/
void W:: dump()
{
   dumpField("W: stat: ", 1, stat->getOutput(), false);
   dumpField(" icode: ", 1, icode->getOutput(), false);
   dumpField(" valE: ", 16, valE->getOutput(), false);
   dumpField(" valM: ", 16, valM->getOutput(), false);
   dumpField(" dstE: ", 1, dstE->getOutput(), false);
   dumpField(" dstM: ", 1, dstM->getOutput(), true);
}

