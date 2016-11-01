#include "DBaseRecord.h"
#include <iostream>
#include <string>

DBaseRecord::DBaseRecord()
{
    //ctor
}

DBaseRecord::DBaseRecord(std::string& recordStr, std::vector<DBaseColDef>& colDef)
{
    //in one record
    unsigned int curPos = 0;
    for(DBaseColDef column : colDef){
        std::cout << recordStr.substr(curPos, column.m_fieldLength) << std::endl;
        curPos += column.m_fieldLength;
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

