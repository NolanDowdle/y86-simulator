class DecodeStage: public Stage
{
    private:
        uint64_t d_srcA_var;
        uint64_t d_srcB_var;
        bool E_bubble_var;
        void setEInput(E * ereg, uint64_t stat, uint64_t icode,
                            uint64_t ifun, uint64_t valC, uint64_t valA,
                            uint64_t valB, uint64_t dstE, uint64_t dstM,
                            uint64_t srcA, uint64_t srcB);
        uint64_t d_srcA(uint64_t icode, uint64_t rA);
        uint64_t d_srcB(uint64_t icode, uint64_t rB);
        uint64_t d_dstE(uint64_t icode, uint64_t rB);
        uint64_t d_dstM(uint64_t icode, uint64_t rA);
        uint64_t d_valA(uint64_t icode, uint64_t rA, PipeReg ** pregs, Stage ** stages);
        uint64_t d_valB(uint64_t icode, uint64_t rB, PipeReg ** pregs, Stage ** stages);
        bool calculateControlSignals(uint64_t E_icode, uint64_t E_dstM, uint64_t d_srcA, uint64_t d_srcB, uint64_t e_Cnd);
        void bubbleE(PipeReg ** pregs);
        void normalE(PipeReg ** pregs);
    public:
        bool doClockLow(PipeReg ** pregs, Stage ** stages);
        void doClockHigh(PipeReg ** pregs);
        uint64_t getd_srcA();
        uint64_t getd_srcB();
};