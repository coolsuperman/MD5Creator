#include"MD5.h"

MD5::MD5 ():_chunkByte(N) {
	Init ();
	memset (_chunk , 0 , _chunkByte);
	_lastByte = _totalByte = 0;
}

void MD5::Init () {
	//初始化A,B,C,D
	_a = 0x67452301;
	_b = 0xefcdab89;
	_c = 0x98badcfe;
	_d = 0x10325476;

	size_t s[N]= { 7 , 12 , 17 , 22 , 7 , 12 , 17 , 22 , 7 , 12 , 17 , 22 , 7 , 12 , 17 , 22 ,
		5 , 9 , 14 , 20 , 5 , 9 , 14 , 20 , 5 , 9 , 14 , 20 , 5 , 9 , 14 , 20 ,
		4 , 11 , 16 , 23 , 4 , 11 , 16 , 23 , 4 , 11 , 16 , 23 , 4 , 11 , 16 , 23 ,
		6 , 10 , 15 , 21 , 6 , 10 , 15 , 21 , 6 , 10 , 15 , 21 , 6 , 10 , 15 , 21 };
	memcpy (_sft , s , sizeof (s));
	for ( int i = 0; i < 64; ++i ) {
		_k [i] = (size_t)(pow (2.0 , 32)*(abs (sin (i + 1.0))));
	}
}

void MD5::calculateMD5 (size_t* chunk) {
	size_t a = _a , b = _b , c = _c , d = _d;
	size_t f , g;//chunk[g],哈希函数返回值--f;
	//64次变换，4轮操作，每轮16个子操作；
	for ( size_t i = 0; i < 64; ++i ) {
		if ( 0 <= i &&i < 16 ) {
			f = F (b , c , d);
			g = i;
		}
		else if ( 16 <=i&& i < 32 ) {
			f = G (b , c , d);
			g = (5 * i + 1) % 16;
		}
		else if ( 32 <=i&& i < 48 ) {
			f = H (b , c , d);
			g = (3 * i + 5) % 16;
		}
		else {
			f = I (b , c , d);
			g = (7 * i) % 16;
		}
		size_t dret = d;
		d = c;
		c = b;
		b = b + shiftLeftRotate (a + f + _k [i] + chunk [g] , _sft [i]);
		a = dret;
	}

	_a += a;
	_b += b;
	_c += c;
	_d += d;
}

void MD5::calculateMD5Final () {
	//_lastByte:<64byte, 表示最后一块数据大小;
	unsigned char*p=_chunk+ _lastByte;//偏移到有效字节的下一个字节
	//填充的前八bit位:1000 0000 0x80
	*p++ = 0x80;
	size_t remainFillByte = _chunkByte - _lastByte - 1;
	if ( remainFillByte < 8 ) {
		memset (p , 0 , remainFillByte);
		calculateMD5 ((size_t*)_chunk);
		memset (_chunk , 0 , _chunkByte);//造出最后一块，最后64位存放原始文档的bit长度
	} else {
		memset (p , 0 , remainFillByte);
	}
	((unsigned long long*)_chunk)[7] = _totalByte * 8;
	calculateMD5 ((size_t*)_chunk);
}

std::string MD5::getFileMd5 (const char* filename) {
	std::ifstream fin (filename , std::ifstream::binary);
	if ( fin.is_open () ) {
		while ( !fin.eof () ) {
			fin.read ((char*)_chunk , _chunkByte);
			if ( _chunkByte != fin.gcount () )
				break;
			_totalByte += _chunkByte;
			calculateMD5 ((size_t*)_chunk);
		}
		_lastByte = fin.gcount ();
		_totalByte += _lastByte;
		calculateMD5Final ();
	}
	std::string A (changeHex (_a)) , B (changeHex (_b)) , C (changeHex (_c)) , D (changeHex (_d)) , back = A + B + C + D;
	return back;
}

std::string MD5::getStringMd5 (const std::string& s) {
	if ( s.empty () )
		return "";
	else {
		unsigned char* ps = (unsigned char*)s.c_str ();
		size_t numChunk = s.size () / _chunkByte;
		for ( size_t i = 0; i < numChunk; ++i ) {
			_totalByte += _chunkByte;
			calculateMD5 ((size_t*)(ps + i * _chunkByte));
		}
		_lastByte = s.size ()&_chunkByte;
		memcpy (_chunk , ps + _totalByte , _lastByte);
		calculateMD5Final ();
		std::string A (changeHex (_a)) , B (changeHex (_b)) , C (changeHex (_c)) , D (changeHex (_d)) , back = A + B + C + D;
		return back;
	}
}

std::string changeHex (size_t num) {
	unsigned char* p = (unsigned char*)&num;
	std::string back;
	for ( int i = 0; i < 4; i++ ) {
		size_t get = *(p + i);
		size_t a = get >> 4;
		size_t b = get&(0xf);
		back += ItoS (a) + ItoS (b);
	}
	return back;
}
std::string ItoS (int x) {
	std::string back;
	if ( x < 10 )
		back.push_back (x + '0');
	else
		back.push_back ((x - 10) + 'a');
	return back;
}
