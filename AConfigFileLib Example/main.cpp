#pragma once
#include "AConfigFileLib.hpp"
#include <fstream>
#include <iostream>
#include <cstdio>

int main()
{

	ConfigFile file("config.ini", FILE_READ);
	std::string s = file.getValueOfAttribute("resolution");	
	std::cout << s << std::endl;
	file.close();

	ConfigFile write("config.ini", FILE_WRITE);
	write.setValueOfAttribute("resolution", "antialiasing");
	write.createAttributeWithValue("neuesAtt", ':', "96");
	write.close();
	
	ConfigFile file2("config.ini", FILE_READ);
	s = file2.getValueOfAttribute("resolution");
	std::cout << s << std::endl;
	file2.close();

	return 0;
}