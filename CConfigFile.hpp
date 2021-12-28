#pragma once

#include <iostream>

#include<fstream>
#include<string>
#include<vector>

enum class STATE
{
	CLOSED,
	OPENED
};

class CConfigFile
{
	public:
		CConfigFile(std::string pFile);

		bool open(std::string pFile);
		void save();
		void close();

		bool write(std::string pName, std::string pValue);
		bool write(std::string pName, std::string pValue, std::string pGroup);

		std::string get(std::string pName, std::string pGroup);
		std::string get(std::string pName);

		void remove(std::string pName, std::string pGroup);
		void remove(std::string pName, const char* pGroup);
		void remove(std::string pName);
		void remove(std::string pGroup, bool pMove);
		void removeFromGroup(std::string pName, std::string pOldGroup);

		void move(std::string pName, std::string pOldGroup, std::string pNewGroup);
		void move(std::string pName, std::string pNewGroup);

		int exists(std::string pGroup);
		int exists(std::string pName, std::string pGroup);

	private:
		void readLines();									// Alle Zeilen einlesen

		std::string translateCode(std::string pLine);		// Übersetzt Zeile zum einfacheren bearbeiten
		std::string translateFile(std::string pLine);		// Übersetzt Zeile zum einfügen in Config

		void smooth();										// Entfernt unnötige leere Zeilen



		std::string m_File;
		std::vector<std::string> m_Lines;					// Speichert alle Zeilen aus der Datei
		STATE m_CurrentState;								// Aktueller Zustand
		std::fstream m_fstream;
};
