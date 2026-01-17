#include <iostream>

extern "C"
{
	void __stdcall writestr(char* str) {
		std::cout << str << std::endl;
	}

	void __stdcall writechar(char ch) {
		std::cout << ch << std::endl;
	}

	void __stdcall writeint(int n) {
		std::cout << n << std::endl;
	}

	void __stdcall writebool(bool b) {
		if (b == false) {
			std::cout << "no" << std::endl;
		}
		else {
			std::cout << "yes" << std::endl;
		}
	}
	int __stdcall strcpr(char* str1, char* str2) {
		if (!str1 && !str2) return 0;
		if (!str1) return -1;
		if (!str2) return 1;

		return std::strcmp(str1, str2);
	}

	int __stdcall atoli(char* str) {
		if (!str) return 0;
		return std::atoi(str);
	}
}