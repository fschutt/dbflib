#ifndef DBASEFIELD_H
#define DBASEFIELD_H


class DBaseField
{
    public:
        DBaseField();
        ~DBaseField();

        //Following is only for remebering the data types in dBase
        //stored in fieldType
//        enum DBASE_DATA_TYPE{
//            CHARACTER = C;
//            DATE = D;
//            FLOATING_POINT = F;
//            LOGICAL = L;
//            MEMO = M;
//            NUMERIC = N;
//        };

    protected:

    private:
        char fieldNameASCII[11];        //name of the field (?)
        char fieldType;                 //Field type of DB field
        char fieldLength;               //Max: 254 (0xFE)
        short fieldDecimalCount;


};

#endif // DBASEFIELD_H
