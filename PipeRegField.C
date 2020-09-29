#include <cstdint>
#include "PipeRegField.h"

/* 
 * PipeRegField constructor
 *
 * initialize the PipeRegField
 * default value is 0 if no parameter is given
*/
PipeRegField::PipeRegField(int state)
{
   input = 0;
   this->state = state;
}

/*
 * setInput
 *
 * @param value to be stored in input data field
 *
*/
void PipeRegField::setInput(uint64_t input)
{
   this->input = input;
}

/*
 * getOutput
 *
 * @return the state data field
*/
uint64_t PipeRegField::getOutput()
{
   return state;
}

/*
 * normal
 *
 * simulates the normal control signal applied
 * to a pipelined register by setting state
 * to the input
*/
void PipeRegField::normal()
{
   state = input;
} 

/*
 * stall
 *
 * simulates a stall by not changing the value
 * of the pipelined register
 *
*/
void PipeRegField::stall()
{
   //do nothing
} 

/*
 * bubble
 *
 * simulates a bubble by changing the pipeline
 * register field to the appropriate value for
 * a nop instruction (SAOK, RNONE, INOP)
 *
 * @param value to set state to (SAOK, RNONE, or INOP)
*/  
void PipeRegField::bubble(int state)
{
   this->state = state;
}

