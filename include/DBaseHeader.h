#ifndef DBASEHEADER_H
#define DBASEHEADER_H

#include <fstream>
#include <stdint.h>
#include <bitset>
#include <ctime>

class DBaseHeader
{
    public:
        void parse(std::string& headerData);
        void stat();

    private:
        //--------------------------------------------------
        //Level 7 DOS Header for dBase III - dBase VII files
        //--------------------------------------------------
        unsigned int m_blockSize = 32;
        bool m_hasStructuralCDX = false;
		bool m_hasMemoField = false;
		bool m_isDatabase =  false;
		uint8_t m_codePageMark = 0;

        std::string m_fileType = "";
        char m_firstByte = 0;                   ///BYPTE 0 temporary storage, c++ can only read bytes
            uint16_t m_versionNr = 3;           ///BYTE 0 bit 0-2 version number of file
            bool m_memoFilePresent = false;     ///BYTE 0 bit 3 is a memo present or not
            bool m_sqlFilePresent = false;      ///BYTE 0 bit 4-6 is a .sql file present or not
            bool m_anyMemoFilePresent = false;  ///BYTE 0 bit 7 is one of the above true
        time_t m_lastUpdated = time(0);         ///BYTE 1-3 YYMMDD format
public:
        unsigned int m_numRecordsInDB = 0;          ///BYTE 4-7 32-bit nr of records
        unsigned int m_numBytesInHeader = 0;        ///BYTE 8-9 16 bit nr of bytes in header
        unsigned int m_numBytesInRecord = 0;        ///BYTE 10-11 16 bit nr of bytes in record
                                                ///BYTE 12-13: 0, ignore
private:
        bool m_incompleteTransaction = false;   ///BYTE 14, set by BEGIN TRANSACTION
        bool m_encrypted = false;               ///BYTE 15, dBase IV encryption flag
                                                ///BYTE 16-27 dBase DOS in multi-user environment
        bool m_productionMDX = false;           ///BYTE 28 0x01 = production .mdx available, else not
                                                ///BYTE 30-31:0, ignore
                                                ///BYTE 64-67:0, ignore
                                                ///BYTE n+1: 0x0D (13) field descriptor terminator
                                                ///BYTE n+2 start of the field properties
    private:
        /**< Helper function: converts char to bitset */
    	std::bitset<8> toBits(const char& byte);
};

#endif // DBASEHEADER_H
