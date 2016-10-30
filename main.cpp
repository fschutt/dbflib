#include <iostream>
#include <stdexcept>

#include "DBaseFile.h"

using namespace std;

int main()
{
    DBaseFile dbf;
    //See SHBrowseForFolder, GetOpenFileName
    try{
        dbf.openFile("./test/ANSPRECH.DBF");
    }catch(std::runtime_error& e){
        std::cout << e.what() << std::endl;
    }

    dbf.stat();
    return 0;
}
