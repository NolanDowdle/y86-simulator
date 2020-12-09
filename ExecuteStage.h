//class to perform the combinational logic of
//the Fetch stage
class ExecuteStage: public Stage
{
   private:
      uint64_t dstE;
      uint64_t valE;
      bool M_bubble;
      void setMInput(M * mreg, uint64_t stat, uint64_t icode,
         uint64_t Cnd, uint64_t valA, uint64_t valE,
         uint64_t dstE, uint64_t dstM);
         uint64_t aluA(uint64_t icode, uint64_t valA, uint64_t valC);
         uint64_t aluB(uint64_t icode, uint64_t valB);
         uint64_t alufun(uint64_t icode, uint64_t ifun);
         bool set_cc(uint64_t icode, uint64_t m_stat, uint64_t W_stat);
         uint64_t e_dstE(uint64_t icode, uint64_t dstE, uint64_t e_Cnd);
         void CC(uint64_t icode, uint64_t ifun, uint64_t op1, uint64_t op2, uint64_t m_stat, uint64_t W_stat);
         uint64_t ALU(uint64_t icode, uint64_t ifun, uint64_t aluA, uint64_t aluB, uint64_t m_stat, uint64_t W_stat, bool * of);
         uint64_t cond(uint64_t icode, uint64_t ifun);
         bool calculateControlSignals(uint64_t m_stat, uint64_t W_stat);
         void normalM(PipeReg ** pregs);
         void bubbleM(PipeReg ** pregs);

   public:
      bool doClockLow(PipeReg ** pregs, Stage ** stages);
      void doClockHigh(PipeReg ** pregs);
      uint64_t gete_dstE();
      uint64_t gete_valE();

};