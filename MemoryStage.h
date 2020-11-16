class MemoryStage: public Stage
{
    private:
        void setWInput(W * wreg, uint64_t stat, uint64_t icode,
            uint64_t valM, uint64_t valE,
            uint64_t dstE, uint64_t dstM);
    public:
        bool doClockLow(PipeReg ** pregs, Stage ** stages);
        void doClockHigh(PipeReg ** pregs);
        bool mem_write(uint64_t icode);
        bool mem_read(uint64_t icode);
        uint64_t mem_addr(uint64_t icode, uint64_t valA, uint64_t valE);
        uint64_t get_dstM();
        uint64_t get_valM();
};