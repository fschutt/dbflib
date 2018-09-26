#ifndef DBaseColDef_H
#define DBaseColDef_H
#include <stdint.h>
#include <string>

enum class DBaseFieldType {
    Character,
    Currency,
    Numeric,
    Float,
    Date,
    DateTime,
    Double,
    Integer,
    Logical,
    Memo,
    General,
    Picture,
    AutoIncrement,
    Timestamp,
};

enum class DBaseFieldFlag {
    SystemColumn,
    NullColumn,
    BinaryColumn,
    NullBinaryColumn,
    AutoIncrementColumn,
    NormalColumn,
};

struct DBaseColDef
{
        DBaseColDef();
        ~DBaseColDef();
        DBaseColDef(std::string& oneColumn);

        void stat();

        std::string     m_fieldName = "";
        DBaseFieldType  m_fieldType = DBaseFieldType::Character;
        uint32_t         m_fieldDisplacement = 0;
        uint16_t         m_fieldLength = 0;
        uint16_t         m_fieldDecCount = 0;
        DBaseFieldFlag  m_fieldFlag = DBaseFieldFlag::NormalColumn;

        uint16_t   m_autoIncrementNext = 0;
        uint16_t   m_autoIncrementStep = 0;

};

#endif // DBaseColDef_H
