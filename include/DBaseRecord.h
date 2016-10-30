#ifndef DBASERECORD_H
#define DBASERECORD_H

#include <string>

class DBaseRecord
{
    public:
        DBaseRecord();
        DBaseRecord(std::string& recordStr);
        ~DBaseRecord();
    protected:

    private:
        bool isRecordDeleted = false;
};

#endif // DBASERECORD_H
