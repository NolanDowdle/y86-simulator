#include <iostream>
#include <iomanip>
#include "ConditionCodes.h"
#include "Tools.h"

//cc_instance will be initialized to reference the single 
//instance of ConditionCodes
ConditionCodes * ConditionCodes::ccInstance = NULL;

/**
 * ConditionCodes constructor
 * initialize the codes field to 0
 */
ConditionCodes::ConditionCodes()
{
    codes = 0;
}

/**
 * getInstance
 * if ccInstance is NULL then getInstance creates the
 * single ConditionCodes instance and sets ccInstance
 * to that. Otherwise, it returns ccInstance
 *
 * @return ccInstance
 */
ConditionCodes * ConditionCodes::getInstance()
{
   if(ccInstance == NULL) {
       ccInstance = new ConditionCodes;
   }
   return ccInstance;
}

/*
 * getConditionCode
 * accepts a condition code number (OF, SF, or ZF) and returns 
 * the value of the condition code 
 *
 * @param ccNum equal to either OF, SF, or ZF
 * @return the value of bit ccNum out of codes. if ccNum is 
 *         out of range then returns false
 * @return error is set to true if ccNum is out of range and
 *         false otherwise
 */
bool ConditionCodes::getConditionCode(int32_t ccNum, bool & error)
{
   //Use your getBits in Tools.C.
   //Don't use "magic" numbers.
   if (ccNum == OF || ccNum == SF || ccNum == ZF) {
        error = false;    
        return Tools::getBits(codes, ccNum, ccNum);
   }
   else {
        error = true;
   }
   return false;
}

/*
 * setConditionCode
 * accepts a condition code number (OF, SF, or ZF) and value
 * (true or false) and sets the condition code bit to that
 * value (1/true or 0/false). if the ccNum value is out of 
 * range then codes does not get modified. 
 *
 * @param value to set the condition code bit to (true/1 or false/0)
 * @param ccNum condition code number, either OF, SF, or ZF
 * @return error is set to true if ccNum is out of range and
 *         false otherwise
 */
void ConditionCodes::setConditionCode(bool value, int32_t ccNum, 
                                      bool & error)
{
   //Use your setBits and clearBits in Tools.C. 
   //Don't use "magic" numbers in your code.
   if (ccNum == OF || ccNum == SF || ccNum == ZF) {
        error = false;
        if (value) {
            codes = Tools::setBits(codes, ccNum, ccNum);
        }
        else {
            codes = Tools::clearBits(codes, ccNum, ccNum);
        }
   }
   else {
        error = true;
   }
}

/*
 * dump
 * outputs the values of the condition codes
 */
void ConditionCodes::dump()
{
   int32_t zf = Tools::getBits(codes, ZF, ZF);
   int32_t sf = Tools::getBits(codes, SF, SF);
   int32_t of = Tools::getBits(codes, OF, OF);
   std::cout << std::endl;
   std::cout << "ZF: " << std::hex << std::setw(1) << zf << " ";
   std::cout << "SF: " << std::hex << std::setw(1) << sf << " ";
   std::cout << "OF: " << std::hex << std::setw(1) << of << std::endl;
}
