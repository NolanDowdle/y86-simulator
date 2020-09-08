#define LONGSIZE 8

class Tools
{
   public:
      static uint64_t buildLong(uint8_t bytes[LONGSIZE]);
      static uint64_t getByte(uint64_t ul, int32_t byteNum);
      static uint64_t getBits(uint64_t source, int32_t low, int32_t high);
      static uint64_t clearBits(uint64_t source, int32_t low, int32_t high);
      static uint64_t setBits(uint64_t source, int32_t low, int32_t high);
      static uint64_t setByte(uint64_t ul, int32_t byteNum);
      static uint64_t copyBits(uint64_t source, uint64_t dest,
                               int32_t srclow, int32_t dstlow, int32_t length);
      static uint8_t sign(uint64_t op);
      static bool addOverflow(uint64_t op1, uint64_t op2);
      static bool subOverflow(uint64_t op1, uint64_t op2);
};
