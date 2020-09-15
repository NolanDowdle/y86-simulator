
class Loader
{
   private:
      bool loaded;        //set to true if a file is successfully loaded into memory
      std::ifstream inf;  //input file handle
      bool checkFile(std::string fileName);
   public:
      Loader(int argc, char * argv[]);
      bool isLoaded();
};
