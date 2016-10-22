#include "DBaseFile.h"
#include <iostream>

DBaseFile::DBaseFile()
{
    //ctor
}

DBaseFile::~DBaseFile()
{
    //dtor
}
bool DBaseFile::open(const std::string fileName)
{
    std::cout << fileName << std::endl;
}

