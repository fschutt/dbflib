#include <iostream>
#include <fstream>
#include <stdexcept>
#include <bitset>
#include <ctime>
#include <iomanip>
#include <sstream>

#include "DBaseFile.h"
#include "DBaseFieldDescArray.h"

using namespace std;

/** \brief Constructs dBase structure from given file.
 *  \param Name of the dBase file
 *  \return True if succeded. Otherwise throws exception
 */
bool DBaseFile::openFile(const std::string fileName){

	ifstream iFile;
    iFile.open(fileName.c_str(), ios::binary | ios::in);

    if(!iFile){ throw fileNotFoundEx(); }

    //Get file size
    iFile.seekg(0, iFile.end);
    m_fileSize = iFile.tellg();
    iFile.seekg(m_fileSize, iFile.beg);

    if(m_fileSize <= 0){ throw unexpectedHeaderEndEx(nullptr, m_fileSize); }

    //Read file contents into heap memory
    m_headerData = readHeader(iFile);
    unsigned char* headerData = new unsigned char[m_fileSize];

    //TODO: Check memory
    if(!(m_headerData.empty())){
        header.parse(m_headerData);
    }
    iFile.close();

    //Loop through file
    header.m_fieldDescArrayNum = ((header.m_numBytesInHeader-header.m_blockSize) / header.m_blockSize);
    if(((header.m_numBytesInHeader-header.m_blockSize) % header.m_blockSize) != 0){ throw unexpectedHeaderEndEx(); }

    delete headerData;
    return true;
}

/** \brief Helper function to safely get
 *
 * \param Input File to get File from
 * \throw unexpectedHeaderEx
 * \throw noMemoryAvailableEx
 * \return File contents as std::string
 */
std::string DBaseFile::readHeader(ifstream& iFile){
	std::stringstream tempHeaderFS;

    if(iFile.peek() == std::ifstream::traits_type::eof()){
        throw unexpectedHeaderEndEx();
    }
    iFile.exceptions(std::ios::failbit);
    //copy into tempHeaderFS
    if(iFile.get() != 0x0D){
        tempHeaderFS << iFile.get();
        std::cout << iFile.get();
    }

    //tempHeaderFS << iFile.rdbuf();
    if(!(tempHeaderFS.good())){
        throw noMemoryAvailableEx();
    }

	return tempHeaderFS.str();
}

std::string DBaseFile::readRecords(std::ifstream& iFile, DBaseHeader& iFileHeader)
{
//    m_fieldDescArray[((i-m_blockSize) % m_blockSize)] = m_currentByte;
//    if((i-m_blockSize) % m_blockSize == m_blockSize-1){
//        //One block is full
//        fieldDescriptors.push_back(DBaseFieldDescArray(fieldDescArray));
//    }

    //read into DBaseFieldDescArray
    return "";//TODO
}

int DBaseFile::calculateNextBlockSize(int prev, int totalStringSize, ...)
{
    //TODO
    return 32;
}
