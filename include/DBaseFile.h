#ifndef DBASEFILE_H
#define DBASEFILE_H

#include <string>
#include <vector>
#include <stdint.h>
#include <bitset>
#include <ctime>
#include <exception>
#include <stdexcept>

#include "DBaseRecord.h"
#include "DBaseFieldDescArray.h"

class DBaseFile
{
    public:
        bool openFile(const std::string fileName);              //Open file and get contents
		void stat();                                            //Output debug information

    private:
		std::string getFileContents(std::ifstream& iFile);              //Read file contents safely into std::string
        int calculateNextBlockSize(int prev, int totalStringSize, ...); //On failed block size, calculate next

    	//Helper function: converts char to bitset
    	std::bitset<8> ToBits(const char& byte);
        //File contents
		std::string m_rawData = "";
		int fileSizeBytes = 0;
		unsigned int blockSize = 32;        //max be 16 or 32 or 48 Byte

        //--------------------------------------------------
        //Level 5 DOS Header for dBase III - dBase VII files
        //--------------------------------------------------
        std::string fileType;
        char firstByte;                     //BYPTE 0 temporary storage, c++ can only read bytes
            uint16_t versionNr = 3;         //BYTE 0 bit 0-2 version number of file
            bool memoFilePresent = false;   //BYTE 0 bit 3 is a memo present or not
            bool sqlFilePresent = false;    //BYTE 0 bit 4-6 is a .sql file present or not
            bool anyMemoFilePresent = false;//BYTE 0 bit 7 is one of the above true
        time_t lastUpdated = time(0);       //BYTE 1-3 YYMMDD format
        uint32_t numRecordsInDB = 0;        //BYTE 4-7 32-bit nr of records
        uint16_t numBytesInHeader = 0;      //BYTE 8-9 16 bit nr of bytes in header
        uint16_t numBytesInRecord = 0;      //BYTE 10-11 16 bit nr of bytes in record
                                            //BYTE 12-13: 0, ignore
        bool incompleteTransaction = false; //BYTE 14, set by BEGIN TRANSACTION
        bool encrypted = false;             //BYTE 15, dBase IV encryption flag
                                            //BYTE 16-27 dBase DOS in multi-user environment
        bool productionMDX = false;         //BYTE 28 0x01 = production .mdx available, else not
                                            //BYTE 30-31:0, ignore
											//BYTE 64-67:0, ignore
                                            //BYTE n+1: 0x0D (13) field descriptor terminator
                                            //BYTE n+2 start of the field properties


        std::vector<DBaseFieldDescArray> fieldDescriptors;	//BYTE 68-n Field Descriptor Array, 48 bytes each
        unsigned int endOfHeader = 0;
		bool hasStructuralCDX = false;
		bool hasMemoField = false;
		bool isDatabase =  false;
		uint8_t fieldDescArrayNum = 0;
		uint8_t codePageMark = 0;
};

//Exceptions
//no memory available on system
class noMemoryAvailableEx : public std::exception{
    std::string m_defaultErrorStr;
public:
    noMemoryAvailableEx(std::string errorStr = "Out of memory") : m_defaultErrorStr(errorStr){};
    ~noMemoryAvailableEx();
};

//file could not be read
class fileNotFoundEx : public std::runtime_error{
    static constexpr const char* m_defaultErrorStr = "File not found";
public:
    fileNotFoundEx(const char* errorStr = m_defaultErrorStr);
    ~fileNotFoundEx();
};

//header is not a calculatable, misaligned bytes
class unexpectedHeaderEndEx : public std::runtime_error{
    friend class unexpectedBlockSize;
protected:
	static constexpr int m_byteHeaderFailed = 0;
	static constexpr bool m_isFoxBaseHeader = false;
	static constexpr const char* m_defaultErrorStr = "Unexpected header termination";
public:
    unexpectedHeaderEndEx(const char* errorStr = m_defaultErrorStr,
                          unsigned int byteWhenFailed = m_byteHeaderFailed,
                          bool isFoxBaseHeader = m_isFoxBaseHeader)
                          : runtime_error(m_defaultErrorStr){};
    virtual ~unexpectedHeaderEndEx();
    what(const char* errorStr = m_defaultErrorStr);
};

//blockSize is too short or too long, calculate expected byte layout
class unexpectedBlockSize : protected unexpectedHeaderEndEx{
    static constexpr const char* m_defaultErrorStr = "Unexpected block size";
	static constexpr unsigned int m_unexpectedBlockSizePrev = 32;
public:
    unexpectedBlockSize(const char* errorStr = m_defaultErrorStr,
                        unsigned int byteWhenFailed = m_byteHeaderFailed,
                        unsigned int unexpectedBlockSizePrev = m_unexpectedBlockSizePrev)
                        : unexpectedHeaderEndEx(errorStr){};
    ~unexpectedBlockSize();
};

//base for other file-related exceptions
class badFileEx : public std::runtime_error{
    friend class incompleteTransactionEx;
protected:
	static constexpr const char* m_defaultErrorStr = "Unexpected error when opening file.";
public:
    badFileEx(const char* errorStr = m_defaultErrorStr);
    ~badFileEx();
	what(const char* errorStr = m_defaultErrorStr);
};

//transaction is incomplete
class incompleteTransactionEx : public badFileEx{
private:
    static constexpr const char* m_defaultErrorStr = "File transaction is incomplete.";
public:
    incompleteTransactionEx(const char* errorStr = m_defaultErrorStr)
                            : badFileEx(errorStr){};
    ~incompleteTransactionEx();
};

#endif // DBASEFILE_H
