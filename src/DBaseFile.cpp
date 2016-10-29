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
    m_headerData = getHeader(iFile);
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
std::string DBaseFile::getHeader(ifstream& iFile){
	std::stringstream tempContentsFileStream;

		if(iFile.peek() == std::ifstream::traits_type::eof()){
			//throw unexpectedHeaderEndEx("Empty file", 0);
		}

		iFile.exceptions(std::ios::failbit);
		//operator << returns an ostream&, but in reality it's a stringstream&
		tempContentsFileStream << iFile.rdbuf();

		if(!(tempContentsFileStream.good())){
			//throw noMemoryAvailableEx();
		}

	return tempContentsFileStream.str();
}

std::string DBaseFile::readRecords(std::ifstream& iFile)
{
    //memalloc

        m_fieldDescArray[((i-m_blockSize) % m_blockSize)] = m_currentByte;
        if((i-m_blockSize) % m_blockSize == m_blockSize-1){
            //One block is full
            fieldDescriptors.push_back(DBaseFieldDescArray(fieldDescArray));
        }

    //read into DBaseFieldDescArray
    return "";//TODO
}

int DBaseFile::calculateNextBlockSize(int prev, int totalStringSize, ...)
{

}
