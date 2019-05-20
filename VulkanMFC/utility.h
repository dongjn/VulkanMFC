
//
//  utility.h
//  MathCore
//
//  Created by seraph on 22/7/18.
//  Copyright © 2018年 seraph. All rights reserved.
//

#ifndef utility_h
#define utility_h
//#include"math_core.hpp"
#include<string>
#include<memory>
#include<sstream>
#include<thread>
#include<assert.h>
#ifdef ANDROID
#include<android/log.h>
#include <iostream>
#elif  _WINDOWS
#include<Windows.h>
extern HANDLE hFile;// = INVALID_HANDLE_VALUE;
inline void windo_log(const char* TAG,std::stringstream& ss) {
	if (hFile == INVALID_HANDLE_VALUE) {
		hFile = CreateFileA("d:/log/vulkan_debug.log", GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, 0, INVALID_HANDLE_VALUE);
		assert(hFile != INVALID_HANDLE_VALUE);
	}
	ss <<"\r\n"<< std::endl;
	std::string msg = ss.str();
	DWORD dwWrite;
	WriteFile(hFile, TAG, strlen(TAG), &dwWrite, nullptr);
	WriteFile(hFile, msg.c_str(), msg.length(), &dwWrite, nullptr);
	FlushFileBuffers(hFile);
	
}



#else
#include<iostream>
#endif

namespace seraphim{
using sstream=std::stringstream;
using std::shared_ptr;
using std::unique_ptr;
using std::string;
using std::wstring;
extern wstring string2wstring(const string& str);
extern string  wstring2string(const wstring& wstr);
//extern const char* LOG_TAG;// ="MC_JNI";
extern const char* TAG;
    
extern void fill_bmp_head_32(int w,int h,void* bmp);
//extern int fill_bmp_head_32(int w,int h,int color,long cbBuf,void* bmp,uint32_t* colors);
    
    
static std::ostream& operator<<(std::ostream& o,wstring s){
        string cs = wstring2string(s);
        o<<cs;
        return o;
    }
    
#ifdef ANDROID
    constexpr  int default_level = ANDROID_LOG_DEBUG;
    constexpr int INFO_LEVEL =  ANDROID_LOG_INFO;
    constexpr int DEBUG_LEVEL = ANDROID_LOG_DEBUG;
    constexpr int ERROR_LEVEL = ANDROID_LOG_ERROR;
#else
    constexpr int DEBUG_LEVEL = 0;
    constexpr int INFO_LEVEL = 1;
    constexpr int ERROR_LEVEL = 2;
    constexpr int  default_level = DEBUG_LEVEL;

#endif
    inline void _SLog(sstream& stream,int lever,const char* tag ){

#ifdef ANDROID
        int default_level = ANDROID_LOG_INFO;
        if(tag==nullptr || string(tag).empty()){
            int code = 0;
        }
        auto str = stream.str();
        __android_log_write(lever,tag,stream.str().c_str());
#elif _WINDOWS
		//OutputDebugStringA(stream.str().c_str());
		windo_log(tag,stream);
		
#else
        std::cout<< tag<<stream.str()<<std::endl;
#endif 
        return;
    }

    template<typename T,typename ...Args>
    inline void _SLog(sstream& stream ,int level,const char* TAG, T t,Args ...args ){
        stream<<t;
        _SLog(stream,level,TAG,args...);
    };

    template<typename ...Args>
    inline void slog(int level,const char* tag,Args ...arg){
        if(level >= INFO_LEVEL){
            sstream s ;//= new std::stringstream();
            s<<std::hex;
            s<<"  ";
            _SLog(s,level,tag,arg...);
        }
      
    };
    

    
    template<typename... Args>
    void retract_log(int retract, int level,const char* tag,Args... args){
        sstream s ;
        s<<std::hex;
        for(int i=0;i<retract;i++)
            s<<"  ";
        _SLog(s,level,tag,args...);
    }
    
    class ByteOStream {
    private:
        uint8_t*buf_;
        const size_t  max_size_;
        size_t    offset_;
    public:
        ByteOStream(uint8_t* buf, size_t len) :buf_(buf), max_size_(len), offset_(0) {
            
        }
        size_t offset(){
            return offset_;
        }
        template <typename T>
        void put(T t) {
            size_t cbType = sizeof(T);
            //assert((cbType + offset_) <= max_size_);
            if ((cbType + offset_) > max_size_) {
                //LaOG_ERROR<<
            }
//            uint8_t* src = reinterpret_cast<uint8_t*>(&t);
            memcpy(buf_ + offset_, &t, cbType);
            offset_ += cbType;
        }
        template<typename T>
        void putN(T t) {
            size_t cbType = sizeof(T);
            //assert((cbType + offset_) <= max_size_);
            uint8_t* src = reinterpret_cast<uint8_t*>(&t);
            for (int i = static_cast<int>(cbType - 1); i >= 0; i--) {
                buf_[offset_++] = src[i];
            }
        }
        void put_arry(uint8_t* src, size_t len) {
            //assert(len + offset_ <= max_size_);
            memcpy(&buf_[offset_], src, len);
            offset_ += len;
            
        }
        void reset() {
            memset(buf_, 0, max_size_);
            offset_ = 0;
        }
    };
    
    
    
    
    
    class ByteIStream {
    private:
        uint8_t *buf_;
        size_t   offset_;
        size_t max_len;
    public:
        ByteIStream(uint8_t* buf, size_t len) :buf_(buf), offset_(0), max_len(len)
        {
        }
        
        template<typename T>
        T get() {
            T t;
            size_t cbType = sizeof(T);
            //assert((max_len - offset_ - cbType) >= 0);
            uint8_t*  p = reinterpret_cast<uint8_t*>(&t);
            for (size_t i = 0; i < cbType; i++) {
                p[i] = buf_[offset_++];
            }
            return t;
        }
        template<typename T>
        T getN() {
            T t;
            size_t cbType = sizeof(T);
            //assert((max_len - offset_ - cbType) >= 0);
            uint8_t*  p = reinterpret_cast<uint8_t*>(&t);
            ;
            for (int i = static_cast<int>(cbType-1); i >= 0; i--) {
                p[i] = buf_[offset_++];
            }
            return t;
        }
        
        template<typename T>
        void get_array(T* dst, size_t len) {
            memcpy(&buf_[offset_], dst, len);
            offset_ += len;
        }
        void reset() {
            memset(buf_, 0, max_len);
            offset_ = 0;
        }
    };
    
    class NotOut : public std::ostream {
    public:
        template<typename U>
        NotOut&  operator<<(U& u) {
            return *this;
        }
    };
};
#endif /* utility_h */
