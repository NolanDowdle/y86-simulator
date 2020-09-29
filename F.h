//class to hold the F pipeline registers
class F : public PipeReg
{
   private:
      PipeRegField * predPC;
   public:
      F();
      PipeRegField * getpredPC();
      void dump();
};
