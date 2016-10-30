#include "DBaseColDef.h"
#include <iostream>
#include <string>
#include <cstring>

using namespace std;

DBaseColDef::DBaseColDef(){
	//ctor
}

DBaseColDef::DBaseColDef(std::string& rawData){
	for(unsigned int i = 0; i < 32; i++){
		//Field name with a maximum of 10 characters
		if(i <= 10 && rawData[i] != 0x00){
			cout << rawData[i];
		}else{
			cout << " ";
		}
	}

	//Data type
	switch(rawData[11]){
        case 'C':	cout << "Character";		break;
        case 'Y':	cout << "Currency";			break;
        case 'N':	cout << "Numeric";			break;
        case 'F':	cout << "Float";			break;
        case 'D':	cout << "Date";				break;
        case 'T':	cout << "DateTime";			break;
        case 'B':	cout << "Double";			break;
        case 'I':	cout << "Integer";			break;
        case 'L':	cout << "Logical";			break;
        case 'M':	cout << "Memo";				break;
        case 'G':	cout << "General";			break;
        case 'P':	cout << "Picture";			break;
        case '+':	cout << "Autoincrement";	break;
        case 'O':	cout << "Double";			break;
        case '@':	cout << "Timestamp";		break;
	default: cout << "\t" << std::endl;
	}

	cout << " ";

	//Displacement of field in record
	for(unsigned int k = 12; k <=15; k++){
		cout << (long)rawData[k];
	}

	cout << "\t";

	//Length of field in bytes
	cout << (int)rawData[16];

	cout << "\t";

	//Number of decimal places
	cout << (int)rawData[17];

	cout << " ";

	//Field flags
	switch(rawData[18]){
		case 0x01: cout << "System column"; break;
		case 0x02: cout << "Can be NULL"; 	break;
		case 0x04: cout << "Binary column"; break;
		case 0x06: cout << "Can be NULL and binary!"; break;
		case 0x0C: cout << "Autoincrementing column"; break;
		default:   cout << "Normal column"; break;
	}


	cout << std::endl;

}

DBaseColDef::~DBaseColDef()
{
	//dtor
}
