/**
 * Names: Nolan Dowdle & Chase Faine
 * Team: Nolan & Chase
*/
#include <iostream>
#include <fstream>
#include <string.h>
#include <ctype.h>

#include "Loader.h"
#include "Memory.h"
//#include "Tools.h"

//first column in file is assumed to be 0
#define ADDRBEGIN 2   //starting column of 3 digit hex address 
#define ADDREND 4     //ending column of 3 digit hext address
#define DATABEGIN 7   //starting column of data bytes
#define COMMENT 28    //location of the '|' character 

/**
 * Loader constructor
 * Opens the .yo file named in the command line arguments, reads the contents of the file
 * line by line and loads the program into memory.  If no file is given or the file doesn't
 * exist or the file doesn't end with a .yo extension or the .yo file contains errors then
 * loaded is set to false.  Otherwise loaded is set to true.
 *
 * @param argc is the number of command line arguments passed to the main; should
 *        be 2
 * @param argv[0] is the name of the executable
 *        argv[1] is the name of the .yo file
 */
Loader::Loader(int argc, char * argv[])
{
   loaded = false;
   
   //Start by writing a method that opens the file (checks whether it ends 
   //with a .yo and whether the file successfully opens; if not, return without 
   //loading) 
    
    //printf("%d", Loader::checkFile("asumr.yo"));
    //printf("%d", Loader::checkFile("nolan.yo"));
    if (argc != 2) {
        return;
    }
    if (Loader::checkFile(argv[1]) == false) {
        return;
    }
   

   //The file handle is declared in Loader.h.  You should use that and
   //not declare another one in this file.
   
   //Next write a simple loop that reads the file line by line and prints it out
    
    std::string line;
    int lineNumber = 0;
    while (std::getline(inf, line)) {
        Loader::loadline(line);
        lineNumber = lineNumber + 1;
        if (Loader::hasErrors(line)) {
            std::cout << "Error on line " << std::dec << lineNumber
                 << ": " << line << std::endl;
            return;
        }
    }
         
   //Next, add a method that will write the data in the line to memory 
   //(call that from within your loop)

   //Finally, add code to check for errors in the input line.
   //When your code finds an error, you need to print an error message and return.
   //Since your output has to be identical to your instructor's, use this cout to print the
   //error message.  Change the variable names if you use different ones.
   //  std::cout << "Error on line " << std::dec << lineNumber
   //       << ": " << line << std::endl;

    /*int lineNumber = 0;
    if (Loader::hasErrors(line)) {
        lineNumber = lineNumber + 1;
        std::cout << "Error on line " << std::dec << lineNumber
            << ": " << line << std::endl;
        return;
    }*/

   //If control reaches here then no error was found and the program
   //was loaded into memory.
   loaded = true;  
  
}

/**
 * isLoaded
 * returns the value of the loaded data member; loaded is set by the constructor
 *
 * @return value of loaded (true or false)
 */
bool Loader::isLoaded()
{
   return loaded;
}


//You'll need to add more helper methods to this file.  Don't put all of your code in the
//Loader constructor.  When you add a method here, add the prototype to Loader.h in the private
//section.

bool Loader::checkFile(std::string fileName)
{ 
    if (fileName.substr(fileName.find_last_of(".") + 1) == "yo") 
    {   
        inf.open(fileName);
        if (!inf.fail()) 
        {
            return true;
        }
    }
    return false;
}

void Loader::loadline(std::string line) {
    if(line[0] == '0') {
        //has address in column 0

        if(line[DATABEGIN] != ' ') {
            //has data begin in column 7
            
            int32_t addr = Loader::convert(line, ADDRBEGIN, ADDREND);
            std::string temp;
            int32_t a;
            bool error;
            Memory * memInstance = Memory::getInstance();
            int counter = 0;
            for(int i = 7; line.at(i) != ' ' && line.at(i) != '|'; i = i + 2) {
                temp = line.at(i);
                temp += line.at(i + 1);
                a = Loader::convert(temp, 0, 2);
                memInstance->putByte(a, addr + counter, error);
                counter = counter + 1;
            }
            return;
        } else {
            return;
        }
    } else {
        return;
    }
}

int32_t Loader::convert(std::string line, int a, int b) {
    std::string temp = line.substr(a, b);
    return stoul(temp, NULL, 16);
}

bool Loader::hasErrors(std::string line) {
    if(Loader::hasEmptyLine(line) && Loader::hasPipe(line)) {
        return false;
    }
    if(Loader::correctAddress(line) 
        //&& Loader::correctData(line)
        && Loader::hasPipe(line) 
        && Loader::noOverflow(line)) 
    {
        return false;
    }
    return true;
}

bool Loader::hasEmptyLine(std::string line) {
    return (line.substr(0, 28) == "                            ");
}

bool Loader::hasEmptyData(std::string line) {
    return (line.substr(7, 28) == "                     ");
}

bool Loader::hasPipe(std::string line) {
    if(line.at(28) == '|') {
        return true;
    }
    return false;
}

bool Loader::correctAddress(std::string line) {
    if(line.substr(0, 2) == "0x") {
            int32_t x = Loader::convert(line, ADDRBEGIN, ADDREND);
            if(x >= 0 && x < 0x1000 && line.substr(5, 7) == ": ") {
                return true;
            }
    }
    return false;
}

bool Loader::correctData(std::string line) {
      if(Loader::hasEmptyData(line) || Loader::bytesDivisibleByTwo(line)) {
          return true;
      }

        return false;
}

bool Loader::bytesDivisibleByTwo(std::string line) {
    int bytes = 0;
        for (int i = 7; line.substr(i, i + 1) != " "; i++) {
            bytes = bytes + 1;
        }
        if(bytes % 2 == 1) {
            return false;
        }
        return true;
}

bool Loader::noOverflow(std::string line) {
    int address = Loader::convert(line, 2, 4);
    int bytes = 0;
    for (int i = 7; line.substr(i, i + 1) != " "; i++) {
        bytes = bytes + 1;
    }
    bytes = bytes / 2;
    int sum = address + bytes;
    if (sum >= 0 && sum <= 0x1000) {
        return true; 
    }  
    return false; 
}