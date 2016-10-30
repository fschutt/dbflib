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

    //get file size
    std::ifstream iFile(fileName, std::ifstream::ate | std::ifstream::binary);
    m_fileSize = (unsigned long long)iFile.tellg();
    iFile.seekg(0, iFile.beg);
    iFile.close();

    //check memory
    m_memAvailable = getAvailableMemory();
    if(m_fileSize <= 0){ throw unexpectedHeaderEndEx("ERR_001 : File is empty", m_fileSize); }
    if(m_fileSize > m_memAvailable){ throw noMemoryAvailableEx();};

    //Read file contents into heap memory
    readHeader(iFile);
    m_colDefBlockSize = calculateBlockSize(m_colDefBlockSize, m_colDefLength);
    if(!(m_headerData.empty())){ m_header.parse(m_headerData);}

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
	std::stringstream tempHeaderSS;

    if(!(tempHeaderSS.good())){ throw noMemoryAvailableEx(); };
    iFile.exceptions(std::ios::failbit | std::ios::badbit);

	try{
        //copy into tempHeaderSS
        while(true){
            char cur = iFile.get();
            if(cur != (char)0x0D){
                break;
            }else{
                tempHeaderSS << iFile.get();
            }
        }

	}catch(const ios_base::failure& e){
        throw badFileEx(e.what());
	}

	std::string tempHeader = tempHeaderSS.str();
	m_header.parse(tempHeader);
}

void DBaseFile::readColumns(std::ifstream& iFile, DBaseHeader& iFileHeader){
    if(!iFile){ throw fileNotFoundEx(); }

    //go to end of header (usually 32 Bytes from file beginning)
    iFile.seekg(m_fileHeaderLength, iFile.beg);
    char columnDefinitionBuf[m_colDefBlockSize];
    unsigned int i = 0;

    while(true){
        char cur = iFile.get();
        if(cur != (char)0x0D){
            break;
        }else{
            columnDefinitionBuf[((i-m_colDefBlockSize) % m_colDefBlockSize)] = iFile.get();
            ++i;

            //one block is full
            if((i - m_colDefBlockSize) % m_colDefBlockSize == m_colDefBlockSize - 1){
                m_fieldDescriptors.push_back(DBaseColDef(columnDefinitionBuf));
            }
        }
    }
}

void DBaseFile::readRecords(ifstream& iFile, DBaseHeader& iFileHeader)
{
    if(!iFile){ throw fileNotFoundEx(); }

    //go to end of header (usually 32 Bytes from file beginning)
    iFile.seekg(m_totalHeaderLength, iFile.beg);
    std::string recordBuf;
    unsigned int i = 0;

    while(true){
        char cur = iFile.get();
        if(cur != (char)0x0D){
            break;
        }else{
            recordBuf.push_back(cur);
            ++i;

            //one block is full
            if((i - iFileHeader.m_numBytesInRecord) % iFileHeader.m_numBytesInRecord == iFileHeader.m_numBytesInRecord - 1){

                DBaseRecord* record = new DBaseRecord(recordBuf);
                m_records.push_back(record);
            }
        }
    }
}

unsigned long long DBaseFile::getAvailableMemory()
{
    #if defined(_WIN32)
        MEMORYSTATUSEX status;
        status.dwLength = sizeof(status);
        GlobalMemoryStatusEx(&status);
        return status.ullTotalPhys;
    #elif defined(__linux__)
        long pages = sysconf(_SC_PHYS_PAGES);
        long page_size = sysconf(_SC_PAGE_SIZE);
        return pages * page_size;
    #endif // defined
}

int DBaseFile::calculateBlockSize(int prev, int totalStringSize)
{
    unsigned int calcBlockSize = prev;
    while(totalStringSize % prev != 0){
        if(prev >= 0){ throw unexpectedHeaderEndEx("Header has an unknown number of columns");};
        calcBlockSize -= 16;
    }

    return prev;
}
