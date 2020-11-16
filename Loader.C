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

int32_t holdAddress = 0;
int holdBytes = 0;

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
        lineNumber = lineNumber + 1;
        if (Loader::hasErrors(line)) {
            std::cout << "Error on line " << std::dec << lineNumber
                 << ": " << line << std::endl;

            return;
        }
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
    //printf("\n\nNEW LINE\n");
    if(Loader::hasEmptyLine(line) && Loader::hasPipe(line)) {
        holdAddress = 0;
        holdBytes = 0;
        return false;
    }

    if(Loader::correctAddress(line) 
        && Loader::correctData(line)
        && Loader::hasPipe(line) 
        && Loader::noOverflow(line)
        && Loader::correctNext(line)) 
    {
        return false;
    }

    if(Loader::correctAddress(line) && Loader::hasEmptyData(line) && Loader::hasPipe(line)) {
        holdAddress = Loader::convert(line, ADDRBEGIN, ADDREND);
        holdAddress -= 1;
        return false;
    }
    
    //printf("NO ERRORS\n");
    return true;
}

bool Loader::hasEmptyLine(std::string line) {
    //std::string str = line.substr(0, 28);
    //std::cout << str << str.length() << "\n";
    return (line.substr(0, 28) == "                            ");
}

bool Loader::hasEmptyData(std::string line) {
    //std::string str = line.substr(6, 22);
    //std::cout << str << str.length() << "\n";
    return (line.substr(6, 22) == "                      ");
}

bool Loader::hasPipe(std::string line) {
    if(line.substr(28, 1) == "|") {
        //printf("HASPIPE PASSES\n");
        return true;
    }
    //printf("HASPIPE FAILS\n");
    return false;
}

bool Loader::correctAddress(std::string line) {
    if(line.substr(0, 2) == "0x") {
            int32_t x = Loader::convert(line, ADDRBEGIN, ADDREND);
            if(x >= 0 && x <= 0x1000 && line.substr(5, 2) == ": ") {
                //printf("X: %X\n", x);
                return true;
            }
    }
    //printf("CORRECTADDRESS FAILS\n");
    return false;
}

bool Loader::correctData(std::string line) {
    if(Loader::hasEmptyData(line) || (Loader::bytesDivisibleByTwo(line))) {
        //printf("CORRECT DATA PASSES\n");
        if (!Loader::hasEmptyData(line)) {
            if(line.substr(7, 1) == " " || line.substr(8, 1) == " ") {
                return false;
            }


            std::string str;
            unsigned temp = 0;
            for (unsigned i = 7; line.substr(i, 1) != " " &&  i < line.length(); i++) {
                temp = i;
                str = line.substr(i, 1);
                if(!Loader::validCharacter(str)) {
                    //std::cout << str << str.length() << "\n";
                    
                    return false;
                }
            }
            temp = temp + 1;

            if(line.substr(temp, 1) == " ") {
                for(unsigned i = temp; i < 28; i++) {
                    if(line.substr(i, 1) != " ") {
                        return false;
                    }
                }
            }

            
        }
        return true;
    }
    //printf("CORRECT DATA FAILS\n");
    return false;
}

bool Loader::correctNext(std::string line) {
    int32_t address = Loader::convert(line, ADDRBEGIN, ADDREND);
    int bytes = Loader::getByteNumbers(line);
    
    //printf("%X\n", (holdAddress + holdBytes));
    //printf("%X\n", address);

    if (holdAddress + holdBytes == address || holdAddress + holdBytes == 0) {
        holdAddress = address;
        holdBytes = bytes;
        return true;
    }

    return false;
}

bool Loader::validCharacter(std::string str) {
    if (str == "a" || str == "A"
        || str == "b" || str == "B"
        || str == "c" || str == "C"
        || str == "d" || str == "D"
        || str == "e" || str == "E"
        || str == "f" || str == "F"
        || str == "1" || str == "2"
        || str == "3" || str == "4"
        || str == "5" || str == "6"
        || str == "7" || str == "8"
        || str == "9" || str == "0")
    {
        return true;
    } 
    //std::cout << str << "\n";
    //printf("VALID CHARACTER FAILS\n");
    return false;
}

bool Loader::bytesDivisibleByTwo(std::string line) {
    int bytes = 0;
    for (int i = 7; line.substr(i, 1) != " "; i++) {
        bytes = bytes + 1;
    }
    if(bytes % 2 == 0) {
        //printf("BYTES: %d\n", bytes);
        return true;
    }
    //printf("BYTES FAILS\n");
    return false;
}

int Loader::getByteNumbers(std::string line) {
    int bytes = 0;
    for (unsigned i = 7; line.substr(i, 1) != " " && i < line.length(); i++) {
        bytes = bytes + 1;
    }
    return (bytes / 2);
}

bool Loader::noOverflow(std::string line) {
    int address = Loader::convert(line, 2, 4);
    int bytes = 0;
    for (unsigned i = 7; line.substr(i, 1) != " " && i < line.length(); i++) {
        bytes = bytes + 1;
    }
    bytes = bytes / 2;
    int sum = address + bytes;
    if (sum >= 0 && sum <= 0x1000) {
        //printf("NO OVERFLOW PASSES\n");
        return true; 
    }
    //printf("NO OVERFLOW FAILS\n");  
    return false; 
}
/*
int main() {
    std::string fuckingwork = "0x138:                      |       .align 8";
    bool ok = hasErrors(fuckingwork);
    if (ok == false) {
        if (correctAddress(fuckingwork) == true) {
            if (correctData(fuckingwork) == true) {
                //(fill the rest in for other methods)
            } else {
                //(fill the rest in for other methods)
                printf("Maybe correctData");
            }
        } else {
            printf("Maybe correctAddress?");
        }
    } else {
        printf("Maybe hasErrors");
    }
}*/