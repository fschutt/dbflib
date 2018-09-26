#include "DBaseColDef.h"
#include <iostream>

DBaseColDef::DBaseColDef() {
    //ctor
}

DBaseColDef::DBaseColDef(std::string& oneColumn) {
    // Parse field name
    m_fieldName = oneColumn.substr(0, 10);

    // Remove whitespace from field name
    m_fieldName = m_fieldName.data();

    // Parse field data type
    switch(oneColumn.at(11)) {
        case 'C':   m_fieldType = DBaseFieldType::Character;          break;
        case 'Y':   m_fieldType = DBaseFieldType::Currency;           break;
        case 'N':   m_fieldType = DBaseFieldType::Numeric;            break;
        case 'F':   m_fieldType = DBaseFieldType::Float;              break;
        case 'D':   m_fieldType = DBaseFieldType::Date;               break;
        case 'T':   m_fieldType = DBaseFieldType::DateTime;           break;
        case 'B':   m_fieldType = DBaseFieldType::Double;             break;
        case 'I':   m_fieldType = DBaseFieldType::Integer;            break;
        case 'L':   m_fieldType = DBaseFieldType::Logical;            break;
        case 'M':   m_fieldType = DBaseFieldType::Memo;               break;
        case 'G':   m_fieldType = DBaseFieldType::General;            break;
        case 'P':   m_fieldType = DBaseFieldType::Picture;            break;
        case '+':   m_fieldType = DBaseFieldType::AutoIncrement;      break;
        case 'O':   m_fieldType = DBaseFieldType::Double;             break;
        case '@':   m_fieldType = DBaseFieldType::Timestamp;          break;
    }

    // Displacement of field in record
    for(unsigned int k = 12; k <=15; k++) {
        m_fieldDisplacement += (oneColumn.at(k) << (8*(k-12)));
    }

    // Length of field in bytes
    m_fieldLength = (int)oneColumn.at(16);

    // Number of decimal places
    m_fieldDecCount = (int)oneColumn.at(17);

    // Parse field flags
    switch(oneColumn.at(18)) {
        case 0x01: m_fieldFlag = DBaseFieldFlag::SystemColumn;          break;
        case 0x02: m_fieldFlag = DBaseFieldFlag::NullColumn;            break;
        case 0x04: m_fieldFlag = DBaseFieldFlag::BinaryColumn;          break;
        case 0x06: m_fieldFlag = DBaseFieldFlag::NullBinaryColumn;      break;
        case 0x0C: m_fieldFlag = DBaseFieldFlag::AutoIncrementColumn;   break;
    }

    // Only if column is marked as auto increment
    if(m_fieldFlag == DBaseFieldFlag::AutoIncrementColumn) {

        for(unsigned int k = 19; k <=22; k++) {
            m_autoIncrementNext += (oneColumn.at(k) << (8*(k-19)));
        }

        m_autoIncrementStep = oneColumn.at(23);
    }
}

void DBaseColDef::stat() {
        std::cout << "FELDNAME:" << "\t" <<m_fieldName <<             std::endl;
        std::cout << "FELDTYP:" << "\t" <<  (int)m_fieldType <<       std::endl;
        std::cout << "FELD DISPL:" << "\t"<< m_fieldDisplacement <<     std::endl;
        std::cout << "FELDLAENGE:" << "\t"<< m_fieldLength <<           std::endl;
        std::cout << "DECCOUNT:" << "\t" << m_fieldDecCount <<         std::endl;
        std::cout << "FIELDFLAG:" << "\t" << (int)m_fieldFlag <<        std::endl;
        if(m_autoIncrementNext != 0) {std::cout << "AUTOINCNEXT:" << m_autoIncrementNext << std::endl;};
        if(m_autoIncrementStep != 0) {std::cout << "AUTOINCSTEP:" << m_autoIncrementStep << std::endl;};
        std::cout << std::endl;
}

DBaseColDef::~DBaseColDef() {
    //dtor
}

