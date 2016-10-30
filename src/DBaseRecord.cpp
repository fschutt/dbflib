#include "DBaseRecord.h"
#include <iostream>
#include <string>

DBaseRecord::DBaseRecord()
{
    //ctor
}

DBaseRecord::DBaseRecord(std::string& recordStr)
{
    std::cout << "Adding record: " << recordStr << std::endl;
}

DBaseRecord::~DBaseRecord()
{
    //dtor
}
