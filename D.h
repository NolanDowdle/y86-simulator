//class to hold the D pipeline registers
class D : public PipeReg
{
   private:
      PipeRegField * stat;
      PipeRegField * icode;
      PipeRegField * ifun;
      PipeRegField * rA;
      PipeRegField * rB;
      PipeRegField * valC;
      PipeRegField * valP;
   public:
      D();
      PipeRegField * getstat();
      PipeRegField * geticode();
      PipeRegField * getifun();
      PipeRegField * getrA();
      PipeRegField * getrB();
      PipeRegField * getvalC();
      PipeRegField * getvalP();
      void dump();
};
