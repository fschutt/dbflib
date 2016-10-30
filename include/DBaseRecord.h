#ifndef DBASERECORD_H
#define DBASERECORD_H

#include <string>
#include <vector>
#include "DBaseColDef.h"

class DBaseRecord
{
    public:
        DBaseRecord();
        DBaseRecord(std::string& recordStr, std::vector<DBaseColDef>& iFileColDef);
        ~DBaseRecord();
    protected:

    private:
        bool isRecordDeleted = false;
};

#endif // DBASERECORD_H
