#include <iostream>
#include <fstream>
#include <stdexcept>
#include <bitset>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "DBaseFile.h"
#include "DBaseColDef.h"

#if defined(_WIN32)
    #define WINVER 0x0500
    #include <windows.h>
#elif defined(__linux__)
    #include <unistd.h>
#endif // _WIN32

using namespace std;

/** \brief Constructs dBase structure from given file.
 *  \param Name of the dBase file
 *  \return True if succeded. Otherwise throws exception
 */
bool DBaseFile::openFile(const std::string fileName){

    //open file and get file size
    std::ifstream iFile;
    if(!iFile && iFile.is_open()){ throw fileNotFoundEx("File is already open in another process."); }
    iFile.open(fileName, std::ifstream::ate | std::ifstream::binary);
    m_fileSize = (unsigned long long)iFile.tellg();
    iFile.seekg(0, iFile.beg);

    //check memory
    m_memAvailable = getAvailableMemory();
    if(m_fileSize <= 0){ throw unexpectedHeaderEndEx("File is empty", m_fileSize); }
    if(m_fileSize > m_memAvailable){ throw noMemoryAvailableEx();};

    //Read file contents into heap memory
    readHeader(iFile);

    //Check header
    m_colDefLength = m_header.m_numBytesInHeader - m_totalHeaderLength -1;
    validateBlockSize(m_colDefBlockSize, m_colDefLength);
    if(!(m_headerData.empty())){ m_header.parse(m_headerData);}
    validateBlockSize(m_colDefBlockSize, m_colDefLength);

    //Read rest of file
    readColDef(iFile, m_header);
    readRecords(iFile, m_header);

    iFile.close();

    return true;
}

///Read header
void DBaseFile::readHeader(std::ifstream& iFile){
    iFile.seekg(0, iFile.beg);
    std::string headerBuf(m_fileHeaderLength,' ');
    iFile.read(&headerBuf.at(0), m_fileHeaderLength);
    m_header.parse(headerBuf);
}

///Read column definition
void DBaseFile::readColDef(std::ifstream& iFile, DBaseHeader& iFileHeader){
    //omit terminating byte at header end
    unsigned int headerLengthWOTerminatingChar = iFileHeader.m_numBytesInHeader - 1;
    iFile.seekg((m_fileHeaderLength), iFile.beg);
    std::string colDefBuf((headerLengthWOTerminatingChar - m_fileHeaderLength), ' ');
    iFile.read(&(colDefBuf.at(0)), (headerLengthWOTerminatingChar - m_fileHeaderLength));

    //for each column
    for(unsigned int i = 0; i < colDefBuf.size(); i+=m_colDefBlockSize){
        std::string curColDefStr = colDefBuf.substr(i, m_colDefBlockSize);
        DBaseColDef colDef(curColDefStr);
        m_colDef.push_back(colDef);
    }
}

///Read records (based on header and column definition)
void DBaseFile::readRecords(std::ifstream& iFile, DBaseHeader& iFileHeader){
    iFile.seekg(m_header.m_numBytesInHeader);
    //recordBuf may be huge, allocate on heap
    std::shared_ptr<std::string> recordBuf = std::make_shared<std::string>(std::string((m_fileSize - iFileHeader.m_numBytesInHeader), ' '));
    iFile.read(&(recordBuf->at(0)), (m_fileSize - iFileHeader.m_numBytesInHeader));

    //for each record
    for(unsigned int i = 0; i < recordBuf->size(); i+=iFileHeader.m_numBytesInRecord){
        std::string curRecordStr = recordBuf->substr(i, iFileHeader.m_numBytesInRecord);
        DBaseRecord* record = new DBaseRecord(curRecordStr, m_colDef);
        m_records.push_back(record);
    }
}

///Get free memory from OS
inline unsigned long long DBaseFile::getAvailableMemory(){
    #if defined(_WIN32)
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullAvailPhys;
    #elif defined(__linux__)
        long pages = sysconf(_SC_PHYS_PAGES);
        long page_size = sysconf(_SC_PAGE_SIZE);
        return pages * page_size;
    #endif // defined
}

///Check if sane block size for column definition
inline void DBaseFile::validateBlockSize(unsigned int& blockSize, unsigned int& totalStringSize){
    if(totalStringSize % blockSize != 0){
        throw unexpectedHeaderEndEx("Header has an unknown number of columns");
    }
}

///Nice formatting for console output
void DBaseFile::stat(){
    std::cout << std::endl;
    std::cout << "========== FILE INFORMATION ==========" << std::endl;
    std::cout << std::endl;
    m_header.stat();
    std::cout << std::endl;
    std::cout << "========= COLUMN INFORMATION =========" << std::endl;
    std::cout << std::endl;
    for(DBaseColDef d : m_colDef){
            d.stat();
    }
    std::cout << std::endl;
    std::cout << "========= RECORD INFORMATION =========" << std::endl;
    for(DBaseRecord* r : m_records){
            r->stat();
    }
    std::cout << std::endl;
}
