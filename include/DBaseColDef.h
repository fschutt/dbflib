#ifndef DBaseColDef_H
#define DBaseColDef_H
#include <stdint.h>
#include <string>

class DBaseColDef
{
    public:
        DBaseColDef();
        ~DBaseColDef();
        DBaseColDef(std::string& oneColumn);

        enum DBASE_FIELD_TYPE{
            DBASE_CHARACTER,
            DBASE_CURRENCY,
            DBASE_NUMERIC,
            DBASE_FLOAT,
            DBASE_DATE,
            DBASE_DATETIME,
            DBASE_DOUBLE,
            DBASE_INTEGER,
            DBASE_LOGICAL,
            DBASE_MEMO,
            DBASE_GENERAL,
            DBASE_PICTURE,
            DBASE_AUTOINCREMENT,
            DBASE_TIMESTAMP
        };

        enum DBASE_FIELD_FLAG{
            DBASE_SYSTEM_COLUMN,
            DBASE_NULL_COLUMN,
            DBASE_BINARY_COLUMN,
            DBASE_NULL_BINARY_COLUMN,
            DBASE_AUTOINCREMENT_COLUMN,
            DBASE_NORMAL_COLUMN
        };

        void stat();

        std::string m_fieldName = "";
        DBASE_FIELD_TYPE m_fieldType = DBASE_CHARACTER;
        uint_fast32_t m_fieldDisplacement = 0;
        uint_fast16_t m_fieldLength = 0;
        uint_fast16_t m_fieldDecCount = 0;
        DBASE_FIELD_FLAG m_fieldFlag = DBASE_NORMAL_COLUMN;

        uint_fast16_t m_autoIncrementNext = 0;
        uint_fast16_t m_autoIncrementStep = 0;

};

#endif // DBaseColDef_H
