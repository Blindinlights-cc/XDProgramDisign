# 位图混合

首先要了解位图的基本信息。

### **1. 位图(Bitmap)**

> 位图又称点阵图或光栅图，它使用我们称为**像素(Pixel)**的一格一格的小点来描述图像。

### **2. 位图文件结构**

1. 位图文件头
2. 位图信息头
3. 颜色表（调色板）
4. 颜色点阵数据

#### 2.1 位图文件头

| 名称          | 占用空间 | 内容                                           |
| :------------ | -------- | ---------------------------------------------- |
| bfType        | 2字节    | 标识，就是“BM”二字                             |
| bfSize        | 4字节    | 整个BMP文件的大小                              |
| bfReserved1/2 | 4字节    | 保留字                                         |
| bfOffBits     | 4字节    | 偏移数，即 位图文件头+位图信息头+调色板 的大小 |

#### 2.2 位图信息头

| **名称**        | **占用空间** | **内容**                                              |
| --------------- | ------------ | ----------------------------------------------------- |
| **biSize**      | 4字节        | 位图信息头的大小，为40                                |
| **biWidth**     | 4字节        | 位图的宽度，单位是像素                                |
| **biHeight**    | 4字节        | 位图的高度，单位是像素                                |
| **biPlanes**    | 2字节        | 固定值1                                               |
| **biBitCount**  | 2字节        | 每个像素的位数 1-黑白图，4-16色，8-256色，24-真彩色   |
| biCompression   | 4字节        | 压缩方式，BI_RGB(0)为不压缩                           |
| biSizeImage     | 4字节        | 位图全部像素占用的字节数，BI_RGB时可设为0             |
| biXPelsPerMeter | 4字节        | 水平分辨率(像素/米)                                   |
| biYPelsPerMeter | 4字节        | 垂直分辨率(像素/米)                                   |
| biClrUsed       | 4字节        | 位图使用的颜色数 如果为0，则颜色数为2的biBitCount次方 |
| biClrImportant  | 4字节        | 重要的颜色数，0代表所有颜色都重要                     |

#### 2.3 位图调色板

24位真彩色图没有调色板不讨论

#### 2.4 颜色点阵数据

位图全部的像素，是按照自下向上，自左向右的顺序排列的。

24位真彩色图有三个通道：R,G,B每个通道占用1字节

**行补位**

将每行补齐为4的倍数，用0补位便于计算机处理。

可以看到24位真彩色图的偏移量为54。

题目中要求处理两个尺寸相同的图片，那么两个图片所占用的位数应该是相同的，即偏移量+（高度\*宽度）\*3

### **3. 处理图片** 

首先是文件信息部分可用一个结构体储存重要信息

````c++
class BitmapHeader{
public:
    BYTE bfType;
    QBYTE bfSize;
    QBYTE bfReserved;
    QBYTE bfOffBits;
    
    QBYTE biSize;
    QBYTE biWidth;
    QBYTE biHeight;
};

````

让目标图片的信息图和第一个图片相同即可，主要获取0宽，高信息；

**读取文件信息：**

首先读取结构体中的信息，读取完后，将指针转移到bfOffBits后的一个位，读取颜色信息，并将亮度信息储存到vetcor中

````c++
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
````

读取两个突变的文件信息后即可开始写入信息

保持目标文件的信息头和第一个图片相同，在用线性叠加的方式写入亮度数据

````c++
    size_t size=sizeof ch;
    std::fstream in(FilenameA);
    std::ofstream out(FilenameRes);
    for (size_t i = 0; i < bitmapA.bfOffBits; i++)
    {
        in.read(re(ch),sizeof ch);
        out.write(re(ch),size);
    }
    out.seekp(bitmapA.bfOffBits+1,std::ios::beg);
    size_t len =rgbA.size();
    for (size_t i = 0; i < rgbA.size(); i++)
    {
        ch=(BYTE)(rgbA[i]*rate)+(BYTE)(rgbB[i]*(1-rate));
        out.write(re(ch), size);
    }
````

混合比例最初是一个字符串，需要将其转化为浮点数据

````c++
float GetMixRate(std::string const &MixRate){
    std::string Rate(MixRate);
    for(auto &p:Rate)
        if(p=='%')p=' ';
    float rate =(float )std::stoi(Rate)/100;
    return rate;
}
````

将功能整合处理，构建函数Blend()

````c++
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
    for (size_t i = 0; i < bitmapA.bfOffBits; i++)
    {
        in.read(re(ch),sizeof ch);
        out.write(re(ch),size);
    }
    out.seekp(bitmapA.bfOffBits+1,std::ios::beg);
    size_t len =rgbA.size();
    for (size_t i = 0; i < rgbA.size(); i++)
    {
        ch=(BYTE)(rgbA[i]*rate)+(BYTE)(rgbB[i]*(1-rate));
        out.write(re(ch), size);
    }

    return OK;

}
````

主函数初始化参数，并调用Blend()

````c++
int main(int argc,char *argv[])
{
    std::string FilenameA(argv[1]);
    std::string MixRate(argv[2]);
    std::string FilenameB(argv[3]);
    std::string FilenameRes(argv[4]);
    Blend(FilenameA,MixRate,FilenameB,FilenameRes);

    return 0;
}
````

![](https://raw.githubusercontent.com/Blindinlights-cc/XDProgramDisign/master/bitmap/a.bmp)

![](https://raw.githubusercontent.com/Blindinlights-cc/XDProgramDisign/master/bitmap/b.bmp)

两幅图片按4:1混合效果

![](https://raw.githubusercontent.com/Blindinlights-cc/XDProgramDisign/master/bitmap/c.bmp)

> 关于第二幅图片
>
> 第二幅图片是用程序生成的
>
> 源码来自StackExchange上的一个项目[ Tweetable Mathematical Art ](https://codegolf.stackexchange.com/questions/35569/tweetable-mathematical-art)
>
> 参加者提供三个函数生成分别R,G,B数据原文有很多好看的由程序生成的图片
