//#define MEMSIZE 0x1000
class Loader
{
   private:
      bool loaded;        //set to true if a file is successfully loaded into memory
      std::ifstream inf;  //input file handle
      bool checkFile(std::string fileName);
      void loadline(std::string line);
      int32_t convert(std::string line, int a, int b);
      bool hasErrors(std::string line);
      bool hasEmptyLine(std::string line);
      bool correctAddress(std::string line);
      bool hasPipe(std::string Line);
      bool noOverflow(std::string line);
      //uint8_t mem[MEMSIZE];
   public:
      Loader(int argc, char * argv[]);
      bool isLoaded();
};
