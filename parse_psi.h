#ifndef __PARSEPSI__
#define __PARSEPSI__
#include"stdint.h"

class ParsePsi {
    public:
    ParsePsi() {}
    ~ParsePsi(){}
    void decode(uint8_t*section_data,int length);
};
#endif