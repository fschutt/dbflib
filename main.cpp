#include <iostream>
#include "DBaseFile.h"

using namespace std;

int main()
{
    DBaseFile dbf;
    //See SHBrowseForFolder, GetOpenFileName
    dbf.openFile("./test/ADRESSEN.dbf");

    return 0;
}
