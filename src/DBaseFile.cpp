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
    if(iFile.is_open()){ throw fileNotFoundEx("File is already open in another process."); }
    iFile.open(fileName, std::ifstream::ate | std::ifstream::binary);
    m_fileSize = (unsigned long long)iFile.tellg();
    iFile.seekg(0, iFile.beg);

    //check memory
    m_memAvailable = getAvailableMemory();
    if(m_fileSize <= 0){ throw unexpectedHeaderEndEx("File is empty", m_fileSize); }
    if(m_fileSize > m_memAvailable){ throw noMemoryAvailableEx();};

    //Read file contents into heap memory
    readHeader(iFile);
    //figure out block size for column definition
    cout << "Number bytes in header from m_header" << m_header.m_numBytesInHeader << endl;
    cout << "Length of file header from DBaseFile" << m_fileHeaderLength << endl;
    cout << "Column definition length from DBaseFile" << m_colDefLength << endl;
    cout << "Column block size from DBaseFile" << m_colDefBlockSize << endl;

    m_colDefLength = m_header.m_numBytesInHeader - m_totalHeaderLength;
    m_colDefBlockSize = calculateBlockSize(m_colDefBlockSize, m_colDefLength);

    if(!(m_headerData.empty())){ m_header.parse(m_headerData);}
    m_colDefBlockSize = calculateBlockSize(m_colDefBlockSize, m_colDefLength);

    readColumns(iFile, m_header);
    readRecords(iFile, m_header);

    std::cout << "Here all all column definitions:\n\n" << std::endl;
    for(DBaseColDef& i : m_colDef){
        std::cout << i.fieldName << std::endl;
        std::cout << i.fieldDecCount << std::endl;
        std::cout << i.fieldLength << std::endl;
        std::cout << i.fieldType << std::endl;
    }
    std::cout << std::endl;

    iFile.close();

    return true;
}

/** \brief Helper function to safely get
 *
 * \param Input File to get File from
 * \throw unexpectedHeaderEx
 * \throw noMemoryAvailableEx
 * \throw badFileEx
 * \return File contents as std::string
 */
void DBaseFile::readHeader(std::ifstream& iFile){
	std::string tempHeader;

    //if(!(tempHeaderSS.good())){ throw noMemoryAvailableEx(); };

	try{
        iFile.exceptions(std::ios::badbit);
        //copy into tempHeaderSS
        for(unsigned int i = 0; i < m_fileHeaderLength; i++){
            tempHeader.append(1, iFile.get());
        }

	}catch(const ios_base::failure& e){
        throw badFileEx(e.what());
	}

	std::cout << tempHeader << std::endl;
	m_header.parse(tempHeader);
}

///Read Columns
void DBaseFile::readColumns(std::ifstream& iFile, DBaseHeader& iFileHeader){
    std::cout << "Reading columns..." << std::endl;
    //go to end of header (usually 32 Bytes from file beginning)
    std::cout << "Now at position" << iFile.tellg() << std::endl;
    std::string* recordBuf = new std::string((m_fileSize + iFileHeader.m_numBytesInHeader), ' ');
    iFile.read(&(recordBuf->at(0)), (m_fileSize - iFileHeader.m_numBytesInHeader));

    //for each record
    for(unsigned int i = 0; i < recordBuf->size(); i+=iFileHeader.m_numBytesInRecord){
        std::string curRecordStr = recordBuf->substr(i, iFileHeader.m_numBytesInRecord);
        DBaseRecord* record = new DBaseRecord(curRecordStr, m_colDef);
        m_records.push_back(record);
    }
}

///Read Records
void DBaseFile::readRecords(std::ifstream& iFile, DBaseHeader& iFileHeader){
    std::cout << "Reading records..." << std::endl;
    //go to end of header (usually 32 Bytes from file beginning)
    iFile.seekg(m_header.m_numBytesInHeader);
    std::cout << "Now at position" << iFile.tellg() << std::endl;
    std::string* recordBuf = new std::string((m_fileSize + iFileHeader.m_numBytesInHeader), ' ');
    iFile.read(&(recordBuf->at(0)), (m_fileSize - iFileHeader.m_numBytesInHeader));

    //for each record
    for(unsigned int i = 0; i < recordBuf->size(); i+=iFileHeader.m_numBytesInRecord){
        std::string curRecordStr = recordBuf->substr(i, iFileHeader.m_numBytesInRecord);
        DBaseRecord* record = new DBaseRecord(curRecordStr, m_colDef);
        m_records.push_back(record);
    }
}

unsigned long long DBaseFile::getAvailableMemory()
{
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

//NOTE: Function will probably return the wrong result on an even number of columns
unsigned int DBaseFile::calculateBlockSize(unsigned int& prev, unsigned int& totalStringSize){
    if(prev >= 16){
        unsigned int next = prev - 16;
        if(totalStringSize % prev != 0){
            //lines up
            return prev;
        }else{
            return calculateBlockSize(next, totalStringSize);
        }
    }else{
        throw unexpectedHeaderEndEx("Header has an unknown number of columns");
    }
}
