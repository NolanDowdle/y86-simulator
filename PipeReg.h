
//these can be used as indices into an array of PipeReg
#define FREG 0
#define DREG 1
#define EREG 2
#define MREG 3
#define WREG 4

//number of PipeRegisters
#define NUMPIPEREGS 5

//base class for the F, D, E, M, W pipeline registers
class PipeReg
{
   public:
      //dump method is implemented in the classes that descend
      //from PipeReg
      //
      //dump is abstract
      //virtual makes it polymorphic 
      virtual void dump() = 0;
   protected:
      void dumpField(std::string label, int width, uint64_t value, bool nl);
};

