#include <iostream>
#include <stdexcept>
#include <memory>

#include "DBaseFile.h"

using namespace std;

int main()
{
    std::shared_ptr<DBaseFile> dbf = std::make_shared<DBaseFile>(DBaseFile());
    //See SHBrowseForFolder, GetOpenFileName
    try{
        dbf->openFile("./test/ANSPRECH.DBF");
    }catch(std::runtime_error& e){
        std::cout << e.what() << std::endl;
    }

    dbf->stat();
    return 0;
}
