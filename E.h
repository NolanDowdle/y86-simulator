//class to hold the E pipeline registers
class E : public PipeReg
{
   private:
      PipeRegField * stat;
      PipeRegField * icode;
      PipeRegField * ifun;
      PipeRegField * valC;
      PipeRegField * valA;
      PipeRegField * valB;
      PipeRegField * dstE;
      PipeRegField * dstM;
      PipeRegField * srcA;
      PipeRegField * srcB;
   public:
      E();
      PipeRegField * getstat();
      PipeRegField * geticode();
      PipeRegField * getifun();
      PipeRegField * getvalC();
      PipeRegField * getvalA();
      PipeRegField * getvalB();
      PipeRegField * getdstE();
      PipeRegField * getdstM();
      PipeRegField * getsrcA();
      PipeRegField * getsrcB();
      void dump();
};
