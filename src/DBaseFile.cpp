#include <iostream>
#include <fstream>
#include <stdexcept>
#include <bitset>
#include <ctime>
#include <iomanip>
#include "DBaseFile.h"
#include "DBaseFieldDescArray.h"

using namespace std;

DBaseFile::DBaseFile(){}
DBaseFile::~DBaseFile(){}
//Helper function: convert byte to bitset
bitset<8> DBaseFile::ToBits(char* byte){return bitset<8>(*byte);}

//Open and read dBase File into memory
bool DBaseFile::openFile(const std::string fileName)
{
	fstream iFile;
	//iFile.exceptions(ifstream::failbit | ifstream::badbit);

	try{
		iFile.open(fileName.c_str(), ios::binary | ios::in);
		if(!iFile){
			throw std::runtime_error("File not found.");
		}

		//Temporary variables
		char* currentByte = new char[1];
		unsigned int i = 0;
		struct tm fileLastUpdated = {0,0,0,0,0,0,0,0,0};
		char fieldDescArray[31];

		while(!iFile.eof()){
			//Read 1 byte at a time
			iFile.read(currentByte, 1);
			//Reached end of table file header, exit loop
			if(*currentByte == 0x0D){break;}

			if(i < 32){
				//Read file header bit by bit. Spec of DBF files available at:
				//http://www.dbf2002.com/dbf-file-format.html

				switch(i){
					case 0:{
						bitset<8> firstBitset = ToBits(currentByte);

						versionNr = (firstBitset[0] | (firstBitset[1] << 1));
						firstBitset[3] == 1 ? memoFilePresent = true : memoFilePresent = false;
						firstBitset[4] == 1 ? sqlFilePresent = true : sqlFilePresent = false;
						firstBitset[5] == 1 ? sqlFilePresent = true : sqlFilePresent = false;
						firstBitset[6] == 1 ? sqlFilePresent = true : sqlFilePresent = false;
						firstBitset[6] == 1 ? anyMemoFilePresent = true : anyMemoFilePresent = false;

						//There is some system to this (the memo and SQL part), but the version numbers are rather
						//random. DO NOT BELIEVE the "official" dBase documentation. This information is based on
						//http://stackoverflow.com/questions/3391525/which-header-format-can-be-assumed-by-reading-an-initial-dbf-byte
						if(*currentByte == 0x01){fileType = "not used";};
						if(*currentByte == 0x02){fileType = "FoxBASE";};
						if(*currentByte == 0x03){fileType = "FoxBASE+/dBASE III PLUS, no memo";};
						if(*currentByte == 0x04){fileType = "dBASE 7";};
						if(*currentByte == 0x05){fileType = "dBASE 5, no memo";};
						if(*currentByte == 0x30){fileType = "Visual FoxPro";};
						if(*currentByte == 0x31){fileType = "Visual FoxPro, autoincrement enabled";};
						if(*currentByte == 0x32){fileType = "Visual FoxPro, Varchar, Varbinary, or Blob-enabled";};
						if(*currentByte == 0x43){fileType = "dBASE IV SQL table files, no memo";};
						if(*currentByte == 0x63){fileType = "dBASE IV SQL system files, no memo";};
						if(*currentByte == 0x7B){fileType = "dBASE IV, with memo";};
						if(*currentByte == 0x83){fileType = "FoxBASE+/dBASE III PLUS, with memo";};
						if(*currentByte == 0x8B){fileType = "dBASE IV, with memo";};
						if(*currentByte == 0x8E){fileType = "dBASE IV with SQL table";};
						if(*currentByte == 0xCB){fileType = "dBASE IV SQL table files, with memo";};
						if(*currentByte == 0xE5){fileType = "Clipper SIX driver, with SMT memo";};
						if(*currentByte == 0xF5){fileType = "FoxPro 2.x (or earlier) with memo";};
						if(*currentByte == 0xFB){fileType = "FoxBASE (with memo?)";};
						break;
					}
					case 1:{	//Last opened: year
						fileLastUpdated.tm_year = ((int)*currentByte + 100);
						break;
					}case 2:{	//Last opened: month
						fileLastUpdated.tm_mon = (((int)*currentByte) - 1);
						break;
					}case 3:{	//Last opened: day
						fileLastUpdated.tm_mday = (int)*currentByte;
						break;
					}case 4: case 5: case 6: case 7:{ //Number of records in table
						numRecordsInDB += (*currentByte << (8*(i-4)));
						break;
					}case 8: case 9:{ //Position of first data record
						numBytesInHeader += (*currentByte << ((8*(i-8))));
						break;
					}case 10: case 11:{ //Length of one data record incl. deleting flag
						numBytesInRecord += (*currentByte << (8*(i-10)));
						break;
					}case 14:{
						*currentByte = 1 ? incompleteTransaction = true : incompleteTransaction = false;
					}case 15:{
						*currentByte = 1 ? encrypted = true : encrypted = false;
					}case 28:{
						//FUCKING HELL, WHAT IS IT WITH ALL THIS BIT SHIFTING
						bitset<8> bit28 = ToBits(currentByte);
						if(bit28[0]){hasStructuralCDX = true;};
						if(bit28[1]){hasMemoField = true;};
						if(bit28[2]){isDatabase = true;};
					}case 29:{
						codePageMark = *currentByte;
						cout << hex << *currentByte << endl;
					}
				}

			}else if(i > 32){
				fieldDescArray[((i-32) % 32)] = *currentByte;
				if((i-32) % 32 == 0){
					cout << fieldDescArray << endl;
				}
			}

			i++;
		}//end read header loop

		//Test: get buffer of 32-bit chars
		iFile.seekg(0, iFile.end);
		int j = iFile.tellg();
		iFile.seekg(i, iFile.beg);

		//How many fields (columns) are there (always fixed width of 32 bytes, beginning at byte 32)
		//DO NOT BELIEVE the "official" documentation here. dBase III files do not have a
		int fieldDescArrayNum = ((i-32) / 32);
		if(((i-32) % 32) != 0){
			throw runtime_error("Field descriptors do not line up with header byte alignment!");
		}

		//Read field properties structure
//		for(unsigned int k = 64; k <= i; k= k+48){
//			cout << "Here comes the header at: " << k << endl;
//		}

		//Postfix: convert fileLastUpdated to lastUpdated time type
		lastUpdated = mktime(&fileLastUpdated);

		///DEBUG STATEMENTS BEGIN
			cout << fileType << endl;

			cout << "Header with length " << i << " bytes contains " << fieldDescArrayNum << " field descriptor arrays!"  << endl;

			cout << "Has memo field:\t\t\t\t";
			cout << (hasMemoField ? "YES" : "NO") << endl;

			cout << "Has structural .cdx file:\t\t";
			cout << (hasStructuralCDX ? "YES" : "NO") << endl;

			cout << "File is a database:\t\t\t";
			cout << (isDatabase ? "YES" : "NO") << endl;

			cout << "SQL file is present:\t\t\t";
			cout << (sqlFilePresent ? "YES" : "NO") << endl;

			cout << "Any memo file present:\t\t\t";
			cout << (anyMemoFilePresent ? "YES" : "NO") << endl;

			cout << "Last updated:\t\t\t\t" << ctime(&lastUpdated) << endl;
			cout << "Number of records:\t\t\t" << (int)numRecordsInDB << endl;
			cout << "Number of bytes in header:\t\t" << (int)numBytesInHeader << endl;
			cout << "Number of bytes per record:\t\t" << (int)numBytesInRecord << endl;
			cout << "Code page mark:\t\t" << hex << codePageMark << endl;
		///DEBUG STATEMENTS END

	}catch(const std::runtime_error& e){
		cout << "Exeption opening file: " << e.what() << endl;
		return false;
	}catch(const std::ifstream::failure& f){
		cout << "Exception when reading file: " << f.what() << endl;
		return false;
	}

	iFile.close();
    return true;
}

