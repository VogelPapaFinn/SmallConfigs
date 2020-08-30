#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cstdio>

enum FILE_MODE
{
	FILE_READ = std::ios::in,
	FILE_WRITE = std::ios::out
};

class ConfigFile
{
	public:
		ConfigFile(std::string directory, FILE_MODE WRITE_OR_READ);
		std::string getValueOfAttribute(std::string attribute);
		bool setValueOfAttribute(std::string attribute, std::string value);
		void createAttributeWithValue(std::string attribute, char dot, std::string value);
		bool open(std::string file_directory);
		void close();

	private:
		void remove(std::string& string, char remove);
		void removeCommentsInLine(std::string& string);
		void remove(std::string& string, std::string);

		std::string m_sFileDirectory;
		std::fstream m_ConfigFileStream;
		FILE_MODE m_WRITE_OR_READ;
};
