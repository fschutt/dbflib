#ifndef DBASEFILE_H
#define DBASEFILE_H

#include <string>
#include <vector>

#include "DBaseRecord.h"
#include "DBaseColDef.h"
#include "DBaseHeader.h"
#include "DBaseRecord.h"

class DBaseFile
{
    public:
        /**< Open file and get contents */
        bool openFile(const std::string fileName);
        /**< Open file and get contents */
		void stat();

    private:
        /**< Read file header safely into std::string */
		void readHeader(std::ifstream& iFile);
        /**< Read field columns */
        void readColDef(std::ifstream& iFile, DBaseHeader& iFileHeader);
        /**< Read records of file */
        void readRecords(std::ifstream& iFile, DBaseHeader& iFileHeader);
        /**< Set block size depending on found block size: Either 48 Byte, 32 Byte or 16 Byte */
        inline void validateBlockSize(unsigned int& prev, unsigned int& totalStringSize);
        /**< Get available memory from system */
        inline unsigned long long getAvailableMemory();

		/**< File size of .dbf file */
		unsigned long long m_fileSize = 0;
		/**< Memory available on system*/
		unsigned long long m_memAvailable = 0;
		/**< File header length in bytes */
		unsigned int m_fileHeaderLength = 32;
		/**< Column definition structure length in bytes */
		unsigned int m_colDefLength = 0;
		/**< Column definition block size (per column definition) length in bytes */
		unsigned int m_colDefBlockSize = 32;
		/**< Total header size (fileHeader + column definition definition) */
		unsigned int m_totalHeaderLength = 0;
        /**< Header contents (read raw from disk)*/
		std::string m_headerData = "";

		/**< \section Member variables */
		/**< Header structure */
		DBaseHeader m_header;
		/**< Column defintion / field descriptors / subrecord structure */
        std::vector<DBaseColDef> m_colDef;
		/**< Data records in the file */
        std::vector<DBaseRecord*> m_records;
};

/**< \section   Exceptions */
/**< \brief     There is not enough memory available on the target computer */
class noMemoryAvailableEx : public std::exception{
    std::string m_defaultErrorStr;
public:
    noMemoryAvailableEx(std::string errorStr = "Out of memory") : m_defaultErrorStr(errorStr){};
    virtual ~noMemoryAvailableEx(){};
    virtual const char* what() const noexcept{ return (m_defaultErrorStr.c_str());};
};

/**< \brief     File could not be found or user has no access to file */
class fileNotFoundEx : public std::exception{
    std::string m_defaultErrorStr;
public:
    fileNotFoundEx(std::string errorStr = "File could not be opened") : m_defaultErrorStr(errorStr){};
    virtual ~fileNotFoundEx(){};
    virtual const char* what() const noexcept{ return (m_defaultErrorStr.c_str());};
};

/**< \brief     Header is corrupt, 0x0D encountered too early*/
class unexpectedHeaderEndEx : public std::exception{
protected:
	std::string m_defaultErrorStr;
	unsigned int m_byteHeaderFailed = 0;
	bool m_isFoxBaseHeader = false;
public:
    unexpectedHeaderEndEx(std::string errorStr = "Unexpected header termination",
                          unsigned int byteWhenFailed = 0,
                          bool isFoxBaseHeader = false)
                          : m_defaultErrorStr(errorStr),
                            m_byteHeaderFailed(byteWhenFailed),
                            m_isFoxBaseHeader(isFoxBaseHeader){};
    virtual ~unexpectedHeaderEndEx(){};
    virtual const char* what() const noexcept{ return (m_defaultErrorStr.c_str());};
};

/**< \brief     File could not be opened*/
class badFileEx : public std::exception{
protected:
	std::string m_defaultErrorStr;
public:
    badFileEx(std::string errorStr = "Unexpected error when opening file")
              : m_defaultErrorStr(errorStr){};
    virtual ~badFileEx(){};
    virtual const char* what() const noexcept{ return (m_defaultErrorStr.c_str());};
};

/**< \brief     Transaction is incomplete, meaning database is possibly corrupt.*/
class incompleteTransactionEx : public badFileEx{
public:
    incompleteTransactionEx(std::string errorStr = "File transaction is incomplete")
                            : badFileEx(errorStr){};
    ~incompleteTransactionEx(){};
    virtual const char* what() const noexcept{ return (m_defaultErrorStr.c_str());};
};

#endif // DBASEFILE_H
