#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include "RegisterFile.h"
#include "Instructions.h"
#include "PipeRegField.h"
#include "PipeReg.h"
#include "E.h"
#include "Status.h"
/*
 *  E constructor
 *  
 *  initialize the E pipeline register
 *    
*/
E::E()
{
   stat = new PipeRegField(SAOK);
   icode = new PipeRegField(INOP);
   ifun = new PipeRegField(FNONE);
   valC = new PipeRegField();
   valA = new PipeRegField();
   valB = new PipeRegField();
   dstE = new PipeRegField(RNONE);
   dstM = new PipeRegField(RNONE);
   srcA = new PipeRegField();
   srcB = new PipeRegField();
}

/* return the stat pipeline register field */
PipeRegField * E::getstat()
{
   return stat;
}

/* return the icode pipeline register field */
PipeRegField * E::geticode()
{
   return icode;
}


/* return the ifun pipeline register field */
PipeRegField * E::getifun()
{
   return ifun;
}

/* return the valC pipeline register field */
PipeRegField * E::getvalC()
{
   return valC;
}

/* return the valA pipeline register field */
PipeRegField * E::getvalA()
{
   return valA;
}

/* return the valB pipeline register field */
PipeRegField * E::getvalB()
{
   return valB;
}

/* return the dstE pipeline register field */
PipeRegField * E::getdstE()
{
   return dstE;
}

/* return the dstM pipeline register field */
PipeRegField * E::getdstM()
{
   return dstM;
}

/* return the srcA pipeline register field */
PipeRegField * E::getsrcA()
{
   return srcA;
}

/* return the srcB pipeline register field */
PipeRegField * E::getsrcB()
{
   return srcB;
}

/* 
 * dump
 *
 * outputs the current values of the E pipeline register
*/
void E::dump()
{
   dumpField("E: stat: ", 1, stat->getOutput(), false);
   dumpField(" icode: ", 1, icode->getOutput(), false);
   dumpField(" ifun: ", 1, ifun->getOutput(), false);
   dumpField(" valC: ", 16, valC->getOutput(), false);
   dumpField(" valA: ", 16, valA->getOutput(), true);
   dumpField("E: valB: ", 16, valB->getOutput(), false);
   dumpField(" dstE: ", 1, dstE->getOutput(), false);
   dumpField(" dstM: ", 1, dstM->getOutput(), false);
   dumpField(" srcA: ", 1, srcA->getOutput(), false);
   dumpField(" srcB: ", 1, srcB->getOutput(), true);
}
