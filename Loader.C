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
#include "Tools.h"

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
    while (std::getline(inf, line)) {
        //std::cout << line << "\n";
        Loader::loadline(line);
    }
         
   //Next, add a method that will write the data in the line to memory 
   //(call that from within your loop)

   //Finally, add code to check for errors in the input line.
   //When your code finds an error, you need to print an error message and return.
   //Since your output has to be identical to your instructor's, use this cout to print the
   //error message.  Change the variable names if you use different ones.
   //  std::cout << "Error on line " << std::dec << lineNumber
   //       << ": " << line << std::endl;


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
            
            int32_t addr = Loader::convert(line, 2, 4);
            printf("%X\n", addr);
            return;
        } else {
            return;
        }
    } else {
        return;
    }
}

int32_t Loader::convert(std::string line, int a, int b) {
    std::string temp;
    temp += line.at(2);
    temp += line.at(3);
    temp += line.at(4);
    return stoul(temp, NULL, 16);
}
