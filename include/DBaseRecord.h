#ifndef DBASERECORD_H
#define DBASERECORD_H

#include <string>
#include <vector>
#include <memory>

#include "DBaseColDef.h"

//todo make template out of this class
//for int, char, double, etc.
struct DBaseRecord
{
    DBaseRecord();
    DBaseRecord(std::string& recordStr, std::vector<DBaseColDef>& iFileColDef);
    ~DBaseRecord();
    void stat();

    private:
        std::vector<std::shared_ptr<std::string>> m_recordData;
};

#endif // DBASERECORD_H
