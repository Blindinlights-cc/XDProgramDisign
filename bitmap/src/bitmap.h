
//
// Created by @BlindingLight on 4/3/22.
//

#ifndef BITMAP_BITMAP_H
#define BITMAP_BITMAP_H
#include <cstdint>
#include <string>
#include<vector>
#define Status bool
#define OK true
#define ERR false
#define BYTE std::uint8_t
#define DBYTE std::uint16_t
#define QBYTE std::uint32_t


class BitmapHeader{
public:
//file header
    BYTE bfType;
    QBYTE bfSize;
    QBYTE bfReserved;
    QBYTE bfOffBits;
//information header
    QBYTE biSize;
    QBYTE biWidth;
    QBYTE biHeight;
};

Status Blend(std::string const &FilenameA,std::string const &MixRate,std::string const &FilenameB,std::string const &FilenameRes);
float GetMixRate(std::string const &MixRate);
Status ReadInfo(BitmapHeader &p,std::string const &FileName,std::vector<BYTE> &rgb);

#endif //BITMAP_BITMAP_H
