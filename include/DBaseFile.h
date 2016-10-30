#ifndef DBASEFILE_H
#define DBASEFILE_H

#include <string>
#include <vector>

#include "DBaseRecord.h"
#include "DBaseFieldDescArray.h"
#include "DBaseHeader.h"

class DBaseFile
{
    public:
        /**<Open file and get contents */
        bool openFile(const std::string fileName);
        /**<Open file and get contents */
		void stat(){header.stat();};

    private:
        /**< Read file contents safely into std::string */
		std::string readHeader(std::ifstream& iFile);

        std::string readRecords(std::ifstream& iFile, DBaseHeader& iFileHeader);
        /**< Set block size depending on found block size: Either 48 Byte, 32 Byte or 16 Byte */
        int calculateNextBlockSize(int prev, int totalStringSize, ...);

		/**< File size of .dbf file */
		int m_fileSize = 0;
		/**< File header length in Bytes */
		unsigned int m_fileHeaderLength = 32;
		/**< Field subrecords structure */
		unsigned int m_subrecordsLength = 0;
		/**< Total header size (fileHeader + subrecords definition) */
		unsigned int m_totalHeaderLength = 0;
        /**< Header contents (read raw from disk)*/
		std::string m_headerData = "";

		/**< \section Member variables */
		/**< Header structure */
		DBaseHeader header;
		/**< Subrecords (column definitions) */
        std::vector<DBaseFieldDescArray> fieldDescriptors;	//BYTE 68-n Field Descriptor Array, 48 bytes each
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

/**< \brief     Subrecords (column definitions) do not fit neatly in header.
*               Try again with different block size */
class unexpectedBlockSize : protected unexpectedHeaderEndEx{
	unsigned int m_blockSizeDefault = 48;
	unsigned int m_blockSizeCurrent = m_blockSizeDefault;
public:
    unexpectedBlockSize(std::string errorStr = "Unexpected block size",
                        unsigned int byteWhenFailed = 0,
                        unsigned int blockSizeCurrent = 48)
                        : unexpectedHeaderEndEx(errorStr, byteWhenFailed, false){
                              if(blockSizeCurrent != m_blockSizeDefault){
                                    m_blockSizeCurrent = blockSizeCurrent;
                                    }
                          };
    virtual ~unexpectedBlockSize(){};
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
