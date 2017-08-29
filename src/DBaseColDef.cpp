#include "DBaseColDef.h"
#include <iostream>
#include <string>
#include <cstring>
#include <algorithm>

using namespace std;

DBaseColDef::DBaseColDef(){
    //ctor
}

DBaseColDef::DBaseColDef(std::string& oneColumn){
    //field name
    m_fieldName = oneColumn.substr(0, 10);

    //remove whitespace from field name
    m_fieldName = m_fieldName.data();

    //field data type
    switch(oneColumn.at(11)){
        case 'C':   m_fieldType = DBASE_CHARACTER;          break;
        case 'Y':   m_fieldType = DBASE_CURRENCY;           break;
        case 'N':   m_fieldType = DBASE_NUMERIC;            break;
        case 'F':   m_fieldType = DBASE_FLOAT;              break;
        case 'D':   m_fieldType = DBASE_DATE;               break;
        case 'T':   m_fieldType = DBASE_DATE;               break;
        case 'B':   m_fieldType = DBASE_DOUBLE;             break;
        case 'I':   m_fieldType = DBASE_INTEGER;            break;
        case 'L':   m_fieldType = DBASE_LOGICAL;            break;
        case 'M':   m_fieldType = DBASE_MEMO;               break;
        case 'G':   m_fieldType = DBASE_GENERAL;            break;
        case 'P':   m_fieldType = DBASE_PICTURE;            break;
        case '+':   m_fieldType = DBASE_AUTOINCREMENT;      break;
        case 'O':   m_fieldType = DBASE_DOUBLE;             break;
        case '@':   m_fieldType = DBASE_TIMESTAMP;          break;
    }

    //Displacement of field in record
    for(unsigned int k = 12; k <=15; k++){
        m_fieldDisplacement += (oneColumn.at(k) << (8*(k-12)));
    }

    //Length of field in bytes
    m_fieldLength = (int)oneColumn.at(16);

    //Number of decimal places
    m_fieldDecCount = (int)oneColumn.at(17);

    //Field flags
    switch(oneColumn.at(18)){
        case 0x01: m_fieldFlag = DBASE_SYSTEM_COLUMN; break;
        case 0x02: m_fieldFlag = DBASE_NULL_COLUMN;     break;
        case 0x04: m_fieldFlag = DBASE_BINARY_COLUMN;
        case 0x06: m_fieldFlag = DBASE_NULL_BINARY_COLUMN; break;
        case 0x0C: m_fieldFlag = DBASE_AUTOINCREMENT_COLUMN; break;
    }

    //only if column is marked as auto increment
    if(m_fieldFlag == DBASE_AUTOINCREMENT_COLUMN){

        for(unsigned int k = 19; k <=22; k++){
            m_autoIncrementNext += (oneColumn.at(k) << (8*(k-19)));
        }
        m_autoIncrementStep = oneColumn.at(23);
    }
}

void DBaseColDef::stat(){
        std::cout << "FELDNAME:" << "\t" <<m_fieldName <<             std::endl;
        std::cout << "FELDTYP:" << "\t" <<  (int)m_fieldType <<       std::endl;
        std::cout << "FELD DISPL:" << "\t"<< m_fieldDisplacement <<     std::endl;
        std::cout << "FELDLAENGE:" << "\t"<< m_fieldLength <<           std::endl;
        std::cout << "DECCOUNT:" << "\t" << m_fieldDecCount <<         std::endl;
        std::cout << "FIELDFLAG:" << "\t" << (int)m_fieldFlag <<        std::endl;
        if(m_autoIncrementNext != 0){std::cout << "AUTOINCNEXT:" << m_autoIncrementNext << std::endl;};
        if(m_autoIncrementStep != 0){std::cout << "AUTOINCSTEP:" << m_autoIncrementStep << std::endl;};
        std::cout << std::endl;
}

DBaseColDef::~DBaseColDef()
{
    //dtor
}

