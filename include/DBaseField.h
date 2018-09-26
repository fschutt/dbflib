#ifndef DBASEFIELD_H
#define DBASEFIELD_H


struct DBaseField
{
        DBaseField();
        ~DBaseField();

    private:
        char fieldNameASCII[11];        // name of the field (?)
        char fieldType;                 // Field type of DB field
        char fieldLength;               // Max: 254 (0xFE)
        short fieldDecimalCount;


};

#endif // DBASEFIELD_H
