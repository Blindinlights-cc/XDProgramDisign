#include<iostream>
#include<fstream>
#include<cstdint>
#include<string>
#include"bitmap.h"

int main(int argc,char *argv[])
{
    std::string FilenameA(argv[1]);
    std::string MixRate(argv[2]);
    std::string FilenameB(argv[3]);
    std::string FilenameRes(argv[4]);
    Blend(FilenameA,MixRate,FilenameB,FilenameRes);

    return 0;
}