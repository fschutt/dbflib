#include "DBaseFile.h"
#include <iostream>

int main() {

    DBaseFile dbf = DBaseFile();
/*
    try {
        dbf->openFile("./test/ANSPRECH.DBF");
    } catch(std::runtime_error& e) {
        std::cout << e.what() << std::endl;
    }
*/
    dbf->stat();
    return 0;
}
