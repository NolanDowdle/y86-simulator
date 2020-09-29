//class to hold the W pipeline registers
class W : public PipeReg
{
   private:
      PipeRegField * stat;
      PipeRegField * icode;
      PipeRegField * valE;
      PipeRegField * valM;
      PipeRegField * dstE;
      PipeRegField * dstM;
   public:
      W();
      PipeRegField * getstat();
      PipeRegField * geticode();
      PipeRegField * getvalE();
      PipeRegField * getvalM();
      PipeRegField * getdstE();
      PipeRegField * getdstM();
      void dump();
};
