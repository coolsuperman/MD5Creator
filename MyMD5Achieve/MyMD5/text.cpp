#define _CRT_SECURE_NO_WARNINGS 1
#include"MD5.h"

int main (int argc,char* argv[]) {
	if ( argc != 3 ) {
		std::cout << argv [0] << "-optional  filename" << std::endl;
		return 0;
	}
	std::string opt (argv [1]);
	MD5 md5;
	if ( opt == "-f" ) {
		std::cout << md5.getFileMd5 (argv [2]) << std::endl;
	}
	else if ( opt == "-s" )
		std::cout << md5.getStringMd5 (argv [2]) << std::endl;
	else
		std::cout << "Unknow Optional! Check your inputation!" << std::endl;
	return 0;
	/*size_t a = 0x1234ab;
	std::cout << changeHex (a) << std::endl;*/
}