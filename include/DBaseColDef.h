#ifndef DBaseColDef_H
#define DBaseColDef_H
#include <stdint.h>
#include <string>

class DBaseColDef
{
	public:
		DBaseColDef();
		~DBaseColDef();
		DBaseColDef(std::string& rawData);

		char fieldName[32];
		char fieldType;
		char fieldLength;
		char fieldDecCount;
		bool productionMDX = 0x00;
		uint_fast32_t nextAutoIncrement = 0x00;
};

#endif // DBaseColDef_H
