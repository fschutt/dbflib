#include "DBaseHeader.h"
#include <fstream>

DBaseHeader::DBaseHeader()
{
    //ctor
}

DBaseHeader::~DBaseHeader()
{
    //dtor
}

bool DBaseHeader::parse(std::string& headerData){

    //Temporary variables
    unsigned char currentByte;
    char fieldDescArray[m_blockSize-1];
    struct tm fileLastUpdated = {0,0,0,0,0,0,0,0,0};

    //TODO: read file into m_headerData
    for(unsigned int i = 0; i <= headerData.size(); i++){

        currentByte = headerData.at(i);

        //Read file header bit by bit. Spec of DBF files available at:
        //http://www.dbf2002.com/dbf-file-format.html
        if(currentByte == 0x0D){break;}

        if(i < m_blockSize){
            switch(i){
                case 0:{
                    std::bitset<8> firstBitset = toBits(currentByte);

                    m_versionNr = (firstBitset[0] | (firstBitset[1] << 1));
                    m_memoFilePresent = (firstBitset[3] == 1 ? true : false);
                    m_sqlFilePresent = ((firstBitset[4] == 1 || firstBitset[5] == 1 || firstBitset[6] == 1) ? true : false);
                    m_anyMemoFilePresent = (firstBitset[6] == 1 ? true : false);

                    //There is some system to this (the memo and SQL part), but the version numbers are rather
                    //random. DO NOT BELIEVE the "official" dBase documentation. This information is based on
                    //http://stackoverflow.com/questions/3391525/which-header-format-can-be-assumed-by-reading-an-initial-dbf-byte
                    if(currentByte == 0x01){m_fileType = "not used";};
                    if(currentByte == 0x02){m_fileType = "FoxBASE";};
                    if(currentByte == 0x03){m_fileType = "FoxBASE+/dBASE III PLUS, no memo";};
                    if(currentByte == 0x04){m_fileType = "dBASE 7";};
                    if(currentByte == 0x05){m_fileType = "dBASE 5, no memo";};
                    if(currentByte == 0x30){m_fileType = "Visual FoxPro";};
                    if(currentByte == 0x31){m_fileType = "Visual FoxPro, autoincrement enabled";};
                    if(currentByte == 0x32){m_fileType = "Visual FoxPro, Varchar, Varbinary, or Blob-enabled";};
                    if(currentByte == 0x43){m_fileType = "dBASE IV SQL table files, no memo";};
                    if(currentByte == 0x63){m_fileType = "dBASE IV SQL system files, no memo";};
                    if(currentByte == 0x7B){m_fileType = "dBASE IV, with memo";};
                    if(currentByte == 0x83){m_fileType = "FoxBASE+/dBASE III PLUS, with memo";};
                    if(currentByte == 0x8B){m_fileType = "dBASE IV, with memo";};
                    if(currentByte == 0x8E){m_fileType = "dBASE IV with SQL table";};
                    if(currentByte == 0xCB){m_fileType = "dBASE IV SQL table files, with memo";};
                    if(currentByte == 0xE5){m_fileType = "Clipper SIX driver, with SMT memo";};
                    if(currentByte == 0xF5){m_fileType = "FoxPro 2.x (or earlier) with memo";};
                    if(currentByte == 0xFB){m_fileType = "FoxBASE (with memo?)";};
                    break;
                }
                case 1:{	//Last opened: year
                    fileLastUpdated.tm_year = ((int)currentByte + 100);
                    break;
                }case 2:{	//Last opened: month
                    fileLastUpdated.tm_mon = (((int)currentByte) - 1);
                    break;
                }case 3:{	//Last opened: day
                    fileLastUpdated.tm_mday = (int)currentByte;
                    break;
                }case 4: case 5: case 6: case 7:{ //Number of records in table
                    m_numRecordsInDB += (currentByte << (8*(i-4)));
                    break;
                }case 8: case 9:{ //Position of first data record
                    m_numBytesInHeader += (currentByte << (8*(i-8)));
                    break;
                }case 10: case 11:{ //Length of one data record incl. deleting flag
                    m_numBytesInRecord += (currentByte << (8*(i-10)));
                    break;
                }case 14:{
                    m_incompleteTransaction = (currentByte == 1 ? true : false);
                }case 15:{
                    m_encrypted = (currentByte == 1 ? true : false);
                }case 28:{
                    std::bitset<8> bit28 = toBits(currentByte);
                    if(bit28[0]){m_hasStructuralCDX = true;};
                    if(bit28[1]){m_hasMemoField = true;};
                    if(bit28[2]){m_isDatabase = true;};
                }case 29:{
                    m_codePageMark = (uint8_t)currentByte;
                }
            }

        }
    }//end for loop read header

    //Postfix: convert fileLastUpdated to lastUpdated time type
    m_lastUpdated = mktime(&fileLastUpdated);
}

/** \brief Outputs debug information to cout
 */
void DBaseHeader::stat()
{
    ///DEBUG STATEMENTS BEGIN
    cout << endl;
    cout << m_fileType << endl;
    cout << "Header with length " << m_numBytesInHeader << " bytes contains " << m_fieldDescArrayNum << " field descriptor arrays!"  << endl;

    cout << "Has memo field:\t\t\t\t";
    cout << (m_hasMemoField ? "YES" : "NO") << endl;
    cout << "Has structural .cdx file:\t\t";
    cout << (m_hasStructuralCDX ? "YES" : "NO") << endl;
    cout << "File is a database:\t\t\t";
    cout << (m_isDatabase ? "YES" : "NO") << endl;
    cout << "SQL file is present:\t\t\t";
    cout << (m_sqlFilePresent ? "YES" : "NO") << endl;
    cout << "Any memo file present:\t\t\t";
    cout << (m_anyMemoFilePresent ? "YES" : "NO") << endl;
    cout << "Has code page mark:\t\t\t";
    cout << ((m_codePageMark != 0) ? "YES" : "NO") << endl;

    cout << "Last updated:\t\t\t\t" << ctime(&m_lastUpdated);
    cout << "Number of records:\t\t\t" << (int)m_numRecordsInDB << endl;
    cout << "Number of bytes in header:\t\t" << (int)m_numBytesInHeader << endl;
    cout << "Number of bytes per record:\t\t" << (int)m_numBytesInRecord << endl;
}

/** \brief Helper function: convert byte to bitset
 * \param {char} 8-Bit char
 * \return {bitset} exploded byte into arrays
 */
bitset<8> DBaseFile::toBits(const char& byte){
    return bitset<8>(byte);
}
