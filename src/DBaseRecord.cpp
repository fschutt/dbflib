#include "DBaseRecord.h"
#include <iostream>
#include <string>

DBaseRecord::DBaseRecord()
{
    //ctor
}

DBaseRecord::DBaseRecord(std::string& recordStr, std::vector<DBaseColDef>& colDef)
{
    for(DBaseColDef column : colDef){
        std::cout << recordStr.substr(column.m_fieldLength) << std::endl;
    }
    //std::cout << recordStr << std::endl;
}

DBaseRecord::~DBaseRecord()
{
    //dtor
}

void DBaseRecord::stat()
{
    if(!m_recordDeleted){
        std::cout << m_recordData << std::endl;
    }
}

