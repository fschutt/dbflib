#include <iostream>
#include "DBaseFile.h"

using namespace std;

int main()
{
    DBaseFile dbf;
    dbf.open("../res/somefile.dbf");

    cout << "Hello world!" << endl;
    return 0;
}
