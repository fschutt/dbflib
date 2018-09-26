#include "DBaseRecord.h"
#include <iostream>
#include <string>
#include <memory>

DBaseRecord::DBaseRecord() {
    //ctor
}

DBaseRecord::DBaseRecord(std::string& recordStr, std::vector<DBaseColDef>& colDef) {
    //Record has space (0x20) if not deleted
    if(recordStr.at(0) == '\x20') {
        //in one record
        unsigned int curPos = 1;
        for(DBaseColDef column : colDef) {
            std::shared_ptr<std::string> fieldStr = std::make_shared<std::string>(
                                     recordStr.substr(curPos, column.m_fieldLength));
            if(!fieldStr->empty()) {
                m_recordData.push_back(fieldStr);
            }
            curPos += column.m_fieldLength;
        }
    }
}

void DBaseRecord::stat()
{
    for(std::shared_ptr<std::string> s : m_recordData) {
        std::cout << *s << std::endl;
    }
}

DBaseRecord::~DBaseRecord() {
}
