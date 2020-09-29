//class to hold M pipeline registers
class M : public PipeReg
{
   private:
      PipeRegField * stat;
      PipeRegField * icode;
      PipeRegField * Cnd;
      PipeRegField * valE;
      PipeRegField * valA;
      PipeRegField * dstE;
      PipeRegField * dstM;
   public:
      M();
      PipeRegField * getstat();
      PipeRegField * geticode();
      PipeRegField * getCnd();
      PipeRegField * getvalE();
      PipeRegField * getvalA();
      PipeRegField * getdstE();
      PipeRegField * getdstM();
      void dump();
};
