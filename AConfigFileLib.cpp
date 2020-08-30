#include "AConfigFileLib.hpp"

ConfigFile::ConfigFile(std::string directory, FILE_MODE WRITE_OR_READ)
{
	// Update MODE
	m_WRITE_OR_READ = WRITE_OR_READ;

	// Init File Directory
	m_sFileDirectory = directory;

	// Load File
	m_ConfigFileStream.open(m_sFileDirectory, FILE_READ);
}

std::string ConfigFile::getValueOfAttribute(std::string attribute)
{
	// Go to the Top of File
	m_ConfigFileStream.clear();
	m_ConfigFileStream.seekg(std::ios::beg);

	// Check Line
	char lineC[999];
	std::string line;
	while (m_ConfigFileStream.getline(lineC, 999))
	{
		// Set Line
		line = lineC;

		// Blank Line?
		if (line.size() != 0)
		{
			// Remove spaces
			this->remove(line, ' ');

			// Remove all Comments in Line
			this->removeCommentsInLine(line);

			// Find Attribute
			if (line.find(attribute) != std::string::npos)
			{
				this->remove(line, attribute);
				return line;
			}
			else
				return "ERROR: ATTRIBUTE NOT FOUND";
		}
	}
}

bool ConfigFile::setValueOfAttribute(std::string attribute, std::string value)
{
	// Get Old Value
	std::string oldValue = getValueOfAttribute(attribute);

	// Go to the Top of File
	m_ConfigFileStream.clear();
	m_ConfigFileStream.seekg(std::ios::beg);

	// Open new File to write
	std::fstream writeStream("configWRITE.ini", std::ios::out);

	// File contains Attribute?
	std::string line;
	while (std::getline(m_ConfigFileStream, line))
	{
		// Find Attribute
		if (line.find(oldValue) != std::string::npos)
		{
			if (value.size() > oldValue.size())
				line.replace(line.find(oldValue), value.size(), value);
			else
				line.replace(line.find(oldValue), oldValue.size(), value);
			writeStream.write((line.append("\n")).c_str(), line.append("\n").size());
		}
		else
			writeStream << line << std::endl;
	}

	// Close readStream
	this->close();
	// Close writeStream
	writeStream.close();

	// Delete old Config
	std::remove(m_sFileDirectory.c_str());
	// Rename new Config
	std::rename("configWRITE.ini", m_sFileDirectory.c_str());

	return true;
}

bool ConfigFile::createAttributeWithValue(std::string attribute, char dot, std::string value)
{
	// Go to the Top of File
	m_ConfigFileStream.clear();
	m_ConfigFileStream.seekg(std::ios::beg);

	// Close Read Stream
	m_ConfigFileStream.close();

	// Open a write Stream
	std::fstream writeStream(m_sFileDirectory, std::ios::app);

	// Go to bottom of File
	writeStream.seekg(std::ios::end);
	writeStream.clear();

	// Write line to File
	writeStream << attribute << dot << value;

	// Close Write Stream
	writeStream.close();

	// Reopen Read Stream
	this->open(m_sFileDirectory);

	return true;
}

void ConfigFile::remove(std::string& string, char remove)
{
	// remove every character in the string
	for (int i = 0; i < string.size(); i++)
		if (string.at(i) == remove)
			string.erase(i, 1);
}

void ConfigFile::remove(std::string& string, std::string remove)
{
	// Vector of chars to remove
	std::vector<int> toRemovePositions;

	// Iterator for remove charArray
	int iteratorCharA = 0;
	// Loop through every letter
	for (int i = 0; i < string.size(); i++)
	{
		// Letter i is a letter in remove? 
		if (string.at(i) == remove.c_str()[iteratorCharA])
		{
			// Increase CharA Iterator
			iteratorCharA++;
			// Save Position
			toRemovePositions.push_back(i);
		}
		// Word finished?
		else if (iteratorCharA == remove.size())
		{
			// Erase Word
			string.erase(toRemovePositions.at(0), remove.size());
			string.erase(0, 1);
			i = string.size();
		}
		// Not the word we search for? Start from the Beginning
		else
		{
			iteratorCharA = 0;
			toRemovePositions.clear();
		}
	}
}

void ConfigFile::removeCommentsInLine(std::string& string)
{
	// Remove everything behind "#"
	for (int i = 0; i < string.size(); i++)
		if (string.at(i) == '#')
			string.erase(i, string.size());
}

bool ConfigFile::open(std::string file_directory)
{
	// Load new file
	if (!m_ConfigFileStream.is_open())
		m_ConfigFileStream.open(file_directory);
	else
		return false;

	if (m_ConfigFileStream.is_open())
		return true;
	else
		return false;
}

void ConfigFile::close()
{
	// Close Stream
	m_ConfigFileStream.close();
}
