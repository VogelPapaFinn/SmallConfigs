#pragma once
#include "AConfigFileLib.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>

int main()
{

	ConfigFile file("config.ini", FILE_READ);
	std::string s = file.getValueOfAttribute("sina");	
	std::cout << s << std::endl;
	file.close();

	ConfigFile write("config.ini", FILE_WRITE);
	write.setValueOfAttribute("sina", "divers");
	write.close();
	
	ConfigFile file2("config.ini", FILE_READ);
	s = file2.getValueOfAttribute("sina");
	std::cout << s << std::endl;
	file2.close();


	return 0;
}