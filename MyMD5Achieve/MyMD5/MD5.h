#include<cstring>
#include<cmath>
#include<fstream>
#include<string>
#include<iostream>

const int N = 64;

class MD5 {
public:
	MD5 ();
	std::string getFileMd5 (const char* filename);
	std::string getStringMd5 (const std::string& s);
private:
	size_t _a;
	size_t _b;
	size_t _c;
	size_t _d;
	size_t _k [N];
	size_t _sft [N];//循环左移数组
	const size_t _chunkByte;
	unsigned char _chunk[N];//64bit
	size_t _lastByte;//最后一块数据大小
	unsigned long long _totalByte;
private:
	void calculateMD5 (size_t* chunk);
	void calculateMD5Final ();//补充最后一块数据到512；
	size_t F (size_t x , size_t y , size_t z) {
		return (x & y) | ((~x) & z);
	}
	size_t G (size_t x , size_t y , size_t z) {
		return (x & z) | (y & (~z));
	}
	size_t H (size_t x , size_t y , size_t z) {
		return x ^ y ^ z;
	}
	size_t I (size_t x , size_t y , size_t z) {
		return	y ^ (x | (~z));
	}
	size_t shiftLeftRotate (size_t num , size_t i) {
		return ((num << i) | (num >> (32 - i)));
	}
	void Init ();
};

std::string changeHex (size_t num);
std::string ItoS (int x);