#ifndef DBASERECORD_H
#define DBASERECORD_H

#include <string>
#include <vector>
#include "DBaseColDef.h"

//todo make template out of this class
//for int, char, double, etc.
class DBaseRecord
{
    public:
        DBaseRecord();
        DBaseRecord(std::string& recordStr, std::vector<DBaseColDef>& iFileColDef);
        ~DBaseRecord();
        void stat();
    protected:

    private:
        std::string m_recordData = "";
        bool m_recordDeleted = false;
};

#endif // DBASERECORD_H
