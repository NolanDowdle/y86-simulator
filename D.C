#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>
#include "Instructions.h"
#include "RegisterFile.h"
#include "PipeReg.h"
#include "PipeRegField.h"
#include "D.h"
#include "Status.h"

/*
 * D constructor
 *
 * initialize the D pipeline register
 */
D::D()
{
   stat = new PipeRegField(SAOK);
   icode = new PipeRegField(INOP);
   ifun = new PipeRegField(FNONE);
   rA = new PipeRegField(RNONE);
   rB = new PipeRegField(RNONE);
   valC = new PipeRegField();
   valP = new PipeRegField();
}

/* return the stat pipeline register */
PipeRegField * D::getstat()
{
   return stat;
}

/* return the icode pipeline register */
PipeRegField * D::geticode()
{
   return icode;
}

/* return the ifun pipeline register */
PipeRegField * D::getifun()
{
   return ifun;
}

/* return the rA pipeline register */
PipeRegField * D::getrA()
{
   return rA;
}

/* return the rB pipeline register */
PipeRegField * D::getrB()
{
   return rB;
}

/* return the valC pipeline register */
PipeRegField * D::getvalC()
{
   return valC;
}

/* return the valP pipeline register */
PipeRegField * D::getvalP()
{
   return valP;
}

/* 
 * dump
 *
 * outputs the current values of the D pipeline register
*/
void D::dump()
{
   dumpField("D: stat: ", 1, stat->getOutput(), false);
   dumpField(" icode: ", 1, icode->getOutput(), false);
   dumpField(" ifun: ", 1, ifun->getOutput(), false);
   dumpField(" rA: ", 1, rA->getOutput(), false);
   dumpField(" rB: ", 1, rB->getOutput(), false);
   dumpField(" valC: ", 16, valC->getOutput(), false);
   dumpField(" valP: ", 3, valP->getOutput(), true);
}
