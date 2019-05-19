//
//  utility.cpp
//  MathCore
//
//  Created by seraph on 22/7/18.
//  Copyright © 2018年 seraph. All rights reserved.
//

#include"utility.h"
#include <cstdlib>
#include <string.h>
#include <string>

//#ifndef _WINDOWS
//#include<locale.h>
//#endif


#ifdef _WINDOWS
HANDLE hFile INVALID_HANDLE_VALUE;
#endif

namespace seraphim{
    const char* TAG = "MATH_CORE_JNI";
//    const char* LOG_TAG = "MC_JNI";
    //将wstring转换成string
    string wstring2string(const wstring& ws)
    {
        std::string strLocale = setlocale(LC_ALL, "");
        const wchar_t* wchSrc = ws.c_str();
        size_t nDestSize = wcstombs(NULL, wchSrc, 0) + 1;
        char *chDest = new char[nDestSize];
        memset(chDest,0,nDestSize);
        wcstombs(chDest,wchSrc,nDestSize);
        std::string strResult = chDest;
        delete []chDest;
        setlocale(LC_ALL, strLocale.c_str());
        return strResult;
    }
    
    void fill_bmp_head_32(int w,int h,void* bmp){
        const int cbFileHeader = 14;//sizeof(BITMAPFILEHEADER);
        const int cbInfoHeader = 40;//sizeof(BITMAPINFOHEADER);
        uint16_t bm = 'M'<< 8 | 'B';
        uint32_t cbData = (w* h *4);
        uint32_t bfSize = cbData+ cbFileHeader+cbInfoHeader;
        //        COLOR32 color0={255,0,0,255};
        uint8_t* phead = static_cast<uint8_t*>(bmp);
        ByteOStream istream(phead,54);
        istream.put(bm);
        istream.put<uint32_t>(bfSize);
        istream.put<uint16_t>(0);
        istream.put<uint16_t>(0);
        istream.put<uint32_t>(54);
        istream.put<uint32_t>(40);
        istream.put<uint32_t>(w);
        istream.put<uint32_t>(h);
        istream.put<uint16_t>(1);
        istream.put<uint16_t>(32);
        istream.put<uint32_t>(0);
        istream.put<uint32_t>(cbData);
        istream.put<uint32_t>(w);
        istream.put<uint32_t>(h);
        istream.put<uint32_t>(0);
        istream.put<uint32_t>(0);
    }
};
