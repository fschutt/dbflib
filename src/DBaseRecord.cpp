#include "DBaseRecord.h"
#include <iostream>
#include <string>

DBaseRecord::DBaseRecord()
{
    //ctor
}

DBaseRecord::DBaseRecord(std::string& recordStr, std::vector<DBaseColDef>& iFileColDef)
{
    std::cerr << recordStr << std::endl << std::endl;
}

DBaseRecord::~DBaseRecord()
{
    //dtor
}
