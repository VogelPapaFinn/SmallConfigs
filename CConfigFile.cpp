#include "CConfigFile.hpp"

CConfigFile::CConfigFile(std::string pFile)
{
	m_File = pFile;

	// Config Datei öffnen
	m_fstream.open(pFile, std::fstream::in);

	// Erfolgreich?
	if (m_fstream.is_open())
	{
		m_CurrentState = STATE::OPENED;
		this->readLines();
	}
	else
	{
		m_CurrentState = STATE::CLOSED;

		// Config erstellen
		std::fstream tmp;
		tmp.open(pFile, std::fstream::out);
		tmp << std::flush;
		tmp.close();

		// Erneut versuchen
		this->open(pFile);
	}
}



bool CConfigFile::open(std::string pFile)
{
	// Config Datei öffnen
	m_fstream.open(pFile, std::fstream::in);

	// Erfolgreich?
	if (m_fstream.is_open())
	{
		// Ja
		m_CurrentState = STATE::OPENED;
		this->readLines();
		return true;
	}
	else
		// Nein
		m_CurrentState = STATE::CLOSED;
	return false;
}

void CConfigFile::readLines()
{
	// Config offen?
	if (m_CurrentState == STATE::OPENED)
	{
		// Zum Anfang der Datei springen
		m_fstream.clear();
		m_fstream.seekg(std::fstream::beg);

		// Liste leeren
		m_Lines.clear();

		// Jede Zeile einlesen
		std::string stringGL;			// Braucht getline();
		while (std::getline(m_fstream, stringGL))
			// Zu Liste hinzufügen
			m_Lines.push_back(this->translateCode(stringGL));
	}
}

void CConfigFile::save()
{
	// Datei zum schreiben neu öffnen
	m_fstream.close();
	m_fstream.open(m_File, std::ios::out | std::ios::trunc);

	// Alle Zeilen schreiben
	for (std::string& s : m_Lines)
		m_fstream << this->translateFile(s) << std::endl;

	// Schließen
	m_fstream.close();
	m_CurrentState = STATE::CLOSED; // STATE ändern

	// Neu öffnen
	if(this->open(m_File)) m_CurrentState = STATE::OPENED; // Wieder geöffnet
}

void CConfigFile::close()
{
	// smooth
	this->smooth();

	// save
	this->save();

	// m_fstream schließen
	m_fstream.close();

	// STATE ändern
	m_CurrentState = STATE::CLOSED;
}



bool CConfigFile::write(std::string pName, std::string pValue, std::string pGroup)
{
	// Config offen?
	if (m_CurrentState == STATE::CLOSED)
		return false; // Nein
	// Ja

	// Gibt es das Attribut in der Gruppe schon?
	int i = this->exists(pName, pGroup);
	if (i != -1)
		// Ja, also Zeile ersetzen
		m_Lines.at(i).replace(pName.size() + 3, m_Lines.at(i).size()-(pName.size() + 3), pValue);
	else // Nein
	{
		// Gibt es die Gruppe überhaupt?
		i = this->exists(pGroup);
		std::string tmp = pName + ":/l" + pValue; // Neue Zeile, zwischengespeichert
		if (i != -1)
		{
			// Neue Zeile einfügen
			m_Lines.insert(m_Lines.begin() + i + 1, tmp);

			return true;
		}
		else
		{
			// Neue Gruppe einfügen
			m_Lines.insert(m_Lines.end(), "[" + pGroup + "]");
			m_Lines.insert(m_Lines.end(), tmp);
			m_Lines.insert(m_Lines.end(), "[/" + pGroup + "]");
			m_Lines.insert(m_Lines.end(), "");

			return true;
		}
	}

	// Hat nicht geklappt
	return false;
}

bool CConfigFile::write(std::string pName, std::string pValue)
{
	// Config offen?
	if (m_CurrentState == STATE::CLOSED)
		return false; // Nein

	// Gebe Rückgabewert von write() zurück
	return this->write(pName, pValue, "default");;
}



std::string CConfigFile::get(std::string pName, std::string pGroup)
{
	// Gibt es Attribut & Gruppe überhaupt?
	int i = this->exists(pName, pGroup); // Zeile zwischenspeichern
	if (i == -1) return ""; // Nein
	
	// Ja, Value zurückgeben
	return m_Lines.at(i).substr(pName.size() + 3, m_Lines.at(i).size() - (pName.size() + 3));
}

std::string CConfigFile::get(std::string pName)
{
	return this->get(pName, "default");
}



void CConfigFile::remove(std::string pName, std::string pGroup)
{
	// Exestiert Attribut und Gruppe überhaupt?
	int i = this->exists(pName, pGroup);
	if (i != -1)
		// Line rauslöschen
		m_Lines.erase(m_Lines.begin() + i, m_Lines.begin() + i + 1);
}

void CConfigFile::remove(std::string pName, const char* pGroup)
{
	// remove
	this->remove(pName, std::string(pGroup));
}

void CConfigFile::remove(std::string pName)
{
	// remove
	this->remove(pName, "default");
}

void CConfigFile::remove(std::string pGroup, bool pMove)
{
	// Exestiert die Gruppe überhaupt?
	int i = this->exists(pGroup);
	int j = i;

	if (i != -1) // Ja
	{
		// Alles in default verschieben?
		if (pMove) 
		{
			// Wo ist die default Gruppe?
			int d = this->exists("default");

			// Alle Zeilen verschieben
			i++; // Will mit Attributen starten, nicht mit Gruppen Header
			while (m_Lines.at(i) != ("[/" + pGroup + "]"))
			{
				// Zeile einfügen
				m_Lines.insert(m_Lines.begin() + d + 1, m_Lines.at(i));

				i = this->exists(pGroup) + 1; // Zeilen verschieben sich

				// Zeile löschen
				m_Lines.erase(m_Lines.begin() + i, m_Lines.begin() + i + 1);

				i = this->exists(pGroup) + 1; // Zeilen verschieben sich
			}

			// Gruppe löschen
			this->remove(pGroup, false);
		}
		else
		{
			// Nein
			while (m_Lines.at(j) != ("[/" + pGroup + "]"))
				j++;
			m_Lines.erase(m_Lines.begin() + i, m_Lines.begin() + j + 1); // Zeilen löschen
		}
	}
}

void CConfigFile::removeFromGroup(std::string pName, std::string pOldGroup)
{
	// move
	this->move(pName, pOldGroup, "default");
}



void CConfigFile::move(std::string pName, std::string pOldGroup, std::string pNewGroup)
{
	if (this->exists(pName, pOldGroup) != -1)
	{
		// Wert speichern
		std::string value = this->get(pName, pOldGroup);

		// Zeile löschen
		this->remove(pName, pOldGroup);

		// Zeile einfügen
		this->write(pName, value, pNewGroup);
	}
}

void CConfigFile::move(std::string pName, std::string pNewGroup)
{
	// move
	this->move(pName, "default", pNewGroup);
}



std::string CConfigFile::translateCode(std::string pLine)
{
	std::string end = ""; // Finaler String

	/* 
	* Leerzeichen  : /l
	* '#'          : rauslöschen
	* ':'		   : bleibt
	*/
	for (char& c : pLine)
	{
		switch (c)
		{
			// Leerzeichen
			case(' '):
				end += "/l";
				break;

			// #
			case('#'):
				break;

			// Alle anderen Zeichen
			default:
				end += c;
				break;
		}
	}

	// Zurückgeben
	return end;
}

std::string CConfigFile::translateFile(std::string pLine)
{
	/*
	* /l  : " "
	* ':' : bleibt
	*/
	std::string tmp = pLine;
	int i = tmp.find("/l"); // Char wo '/' steht
	if (i != std::string::npos)
		tmp.replace(i, 2, " "); // '/l' durch " " ersetzen

	// String zurückgeben
	return tmp;
}



int CConfigFile::exists(std::string pGroup)
{
	int i = 0;	// Aktuelle Zeile
	for (std::string& s : m_Lines)
	{
		// Exestiert die Gruppe?
		if (s == ("[" + pGroup + "]"))
			return i;
		i++; // Nächste Zeile
	}
	return -1;
}

int CConfigFile::exists(std::string pName, std::string pGroup)
{
	int i = this->exists(pGroup);	// Aktuelle Zeile

	// Exestiert die Gruppe?
	if (i != -1)
	{
		// Ja, exestiert Attribut?
		for (NULL; m_Lines.at(i) != ("[/" + pGroup + "]"); i++)
		{
			if (m_Lines.at(i).find(pName) != std::string::npos && m_Lines.at(i).find(":") != std::string::npos)
				return i; // Ja, Zeile zurückgeben
		}
	}

	// Exestiert nicht, Fehler zurückgeben
	return -1;
}



void CConfigFile::smooth()
{
	// Save
	this->save();

	// Alle leeren zeilen löschen
	for (int i = 0; i < m_Lines.size(); i++)
		if (m_Lines.at(i) == "")
		{
			m_Lines.erase(m_Lines.begin() + i, m_Lines.begin() + i + 1);
			i--;
		}

	// Nach Abschluss jeder Gruppe Zeile einfügen
	for (int i = 0; i < m_Lines.size(); i++)
		if (m_Lines.at(i).find("[/") != std::string::npos)
		{
			m_Lines.insert(m_Lines.begin() + i + 1, "");
			i += 1;
		}
}