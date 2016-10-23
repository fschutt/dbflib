#ifndef DBASEFIELDDESCARRAY_H
#define DBASEFIELDDESCARRAY_H
#include <stdint.h>

class DBaseFieldDescArray
{
	public:
		DBaseFieldDescArray();
		~DBaseFieldDescArray();

	private:
		char fieldName[32];
		char fieldType;
		char fieldLength;
		char fieldDecCount;
		bool productionMDX = 0x00;
		uint_fast32_t nextAutoIncrement = 0x00;
};

#endif // DBASEFIELDDESCARRAY_H
