#include"bitmap.h"
#include<fstream>
#include<string>
#define re(a) reinterpret_cast<char*> (&(a))
using std::fstream;
Status Blend(std::string const &FilenameA,std::string const &MixRate,std::string const &FilenameB,std::string const &FilenameRes){
    BitmapHeader bitmapA;
    BitmapHeader bitmapB;
    std::vector<BYTE> rgbA;
    std::vector<BYTE> rgbB;
    float rate=GetMixRate(MixRate);
    ReadInfo(bitmapA,FilenameA,rgbA);
    ReadInfo(bitmapB,FilenameB,rgbB);


    BYTE ch;
    size_t size=sizeof ch;
    std::fstream in(FilenameA);
    std::ofstream out(FilenameRes);
    for (size_t i = 0; i < bitmapA.bfOffBits; i++){
        in.read(re(ch),sizeof ch);
        out.write(re(ch),size);
    }
    out.seekp(bitmapA.bfOffBits+1,std::ios::beg);
    size_t len =rgbA.size();
    for (size_t i = 0; i < len; i++)
    {
        ch=(BYTE)(rgbA[i]*rate)+(BYTE)(rgbB[i]*(1-rate));
        out.write(re(ch), size);
    }

    return OK;

}
float GetMixRate(std::string const &MixRate){
    std::string Rate(MixRate);
    for(auto &p:Rate)
        if(p=='%')p=' ';
    float rate =(float )std::stoi(Rate)/100;
    return rate;


}
Status ReadInfo(BitmapHeader &p,std::string const &FileName,std::vector<BYTE> &rgb){
    std::ifstream in;
    in.open(FileName);
    if(in.is_open()) {
        in.seekg(0);
        in.read(re(p.bfType), sizeof p.bfType);
        in.read(re(p.bfType), sizeof p.bfType);
        in.read(re(p.bfSize), sizeof p.bfSize);
        in.read(re(p.bfReserved), sizeof p.bfReserved);
        in.read(re(p.bfOffBits), sizeof p.bfOffBits);
        in.read(re(p.biSize), sizeof p.biSize);
        in.read(re(p.biWidth), sizeof p.biWidth);
        in.read(re(p.biHeight), sizeof p.biHeight);
        in.seekg(p.bfOffBits + 1, std::ios::beg);
        BYTE ch;
        size_t size = sizeof ch;

        for (size_t i = 0; i < p.biWidth * p.biHeight*3; i++) {
            in.read(re(ch), size);
            rgb.push_back(ch);
        }
        in.close();
    }

    return OK;


}
