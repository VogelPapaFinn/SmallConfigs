#include "CConfigFile.hpp"
/* 
* author:  VogelPapaFinn
* version: 2.0-unstable
*/

// Wenn einem Attribut keine Gruppe zugewiesen wird, wird es automatisch der Gruppe "default" zugewiesen.
// Genau genommen gibt es keine gruppenlose Attribute. Sie k�nnen vom Anwender aber so behandelt werden.


// Constructor
//  �ffnet die Datei 'pFile' oder legt eine an, falls noch keine exestiert.
CConfigFile::CConfigFile(std::string pFile)
{
	m_File = pFile;

	// Config Datei �ffnen
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



// open
// �ffnet die Datei 'pFile', falls diese noch nicht exestiert wird KEINE angelegt.
// Gibt 'true' zur�ck, wenn erfolgreich
bool CConfigFile::open(std::string pFile)
{
	// Config Datei �ffnen
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

// save
// Speichert alle �nderungen in der Datei
void CConfigFile::save()
{
	// Datei zum schreiben neu �ffnen
	m_fstream.close();
	m_fstream.open(m_File, std::ios::out | std::ios::trunc);

	// Alle Zeilen schreiben
	for (std::string& s : m_Lines)
		m_fstream << this->translateFile(s) << std::endl;

	// Schlie�en
	m_fstream.close();
	m_CurrentState = STATE::CLOSED; // STATE �ndern

	// Neu �ffnen
	if(this->open(m_File)) m_CurrentState = STATE::OPENED; // Wieder ge�ffnet
}

// close
// L�scht alle unn�tigen leeren Zeilen, Speichert die Datei und schlie�t sie
void CConfigFile::close()
{
	// smooth
	this->smooth();

	// save
	this->save();

	// m_fstream schlie�en
	m_fstream.close();

	// STATE �ndern
	m_CurrentState = STATE::CLOSED;
}



// write
// Schreibt das Attribut 'pName' mit dem Wert 'pValue' in die Gruppe 'pGroup'.
// Falls es noch keine exestiert wird eine erstellt.
// Gibt 'true' zur�ck, wenn erfolgreich
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
		// Gibt es die Gruppe �berhaupt?
		i = this->exists(pGroup);
		std::string tmp = pName + ":/l" + pValue; // Neue Zeile, zwischengespeichert
		if (i != -1)
		{
			// Neue Zeile einf�gen
			m_Lines.insert(m_Lines.begin() + i + 1, tmp);

			return true;
		}
		else
		{
			// Neue Gruppe einf�gen
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

// write
// Schreibt das Attribut 'pName' mit dem Wert 'pValue' in die Gruppe "default"
// Gibt 'true' zur�ck, wenn erfolgreich
bool CConfigFile::write(std::string pName, std::string pValue)
{
	// Config offen?
	if (m_CurrentState == STATE::CLOSED)
		return false; // Nein

	// Gebe R�ckgabewert von write zur�ck
	return this->write(pName, pValue, "default");;
}



// get
// Liest Wert von Attribut 'pName' in der Gruppe 'pGroup' aus
// Gibt Wert als 'std::string' zur�ck
std::string CConfigFile::get(std::string pName, std::string pGroup)
{
	// Gibt es Attribut & Gruppe �berhaupt?
	int i = this->exists(pName, pGroup); // Zeile zwischenspeichern
	if (i == -1) return ""; // Nein
	
	// Ja, Value zur�ckgeben
	return m_Lines.at(i).substr(pName.size() + 3, m_Lines.at(i).size() - (pName.size() + 3));
}

// get
// Liest Wert von Attribut 'pName' in der Gruppe "default" aus
// Gibt Wert als 'std::string' zur�ck
std::string CConfigFile::get(std::string pName)
{
	return this->get(pName, "default");
}



// remove
// L�scht Attribut 'pName' aus Gruppe 'pGroup'
void CConfigFile::remove(std::string pName, std::string pGroup)
{
	// Exestiert Attribut und Gruppe �berhaupt?
	int i = this->exists(pName, pGroup);
	if (i != -1)
		// Line rausl�schen
		m_Lines.erase(m_Lines.begin() + i, m_Lines.begin() + i + 1);
}

// remove
// L�scht Attribut 'pName' aus Gruppe 'pGroup'
void CConfigFile::remove(std::string pName, const char* pGroup)
{
	// remove
	this->remove(pName, std::string(pGroup));
}

// remove
// L�scht Attribut 'pName' aus Gruppe "default"
void CConfigFile::remove(std::string pName)
{
	// remove
	this->remove(pName, "default");
}

// remove
// L�scht Gruppe 'pGroup'. Wenn 'pMove' true ist, werden alle Attribute der Gruppe in "default" verschoben
void CConfigFile::remove(std::string pGroup, bool pMove)
{
	// Exestiert die Gruppe �berhaupt?
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
				// Zeile einf�gen
				m_Lines.insert(m_Lines.begin() + d + 1, m_Lines.at(i));

				i = this->exists(pGroup) + 1; // Zeilen verschieben sich

				// Zeile l�schen
				m_Lines.erase(m_Lines.begin() + i, m_Lines.begin() + i + 1);

				i = this->exists(pGroup) + 1; // Zeilen verschieben sich
			}

			// Gruppe l�schen
			this->remove(pGroup, false);
		}
		else
		{
			// Nein
			while (m_Lines.at(j) != ("[/" + pGroup + "]"))
				j++;
			m_Lines.erase(m_Lines.begin() + i, m_Lines.begin() + j + 1); // Zeilen l�schen
		}
	}
}

// remove
// L�scht Attribut 'pName' aus Gruppe 'pGroup' und f�gt es in die Gruppe "default" ein
void CConfigFile::removeFromGroup(std::string pName, std::string pOldGroup)
{
	// move
	this->move(pName, pOldGroup, "default");
}



// move
// Verschiebt Attribut 'pName' aus der Gruppe 'pOldGroup' in die Gruppe 'pNewGroup'
void CConfigFile::move(std::string pName, std::string pOldGroup, std::string pNewGroup)
{
	if (this->exists(pName, pOldGroup) != -1)
	{
		// Wert speichern
		std::string value = this->get(pName, pOldGroup);

		// Zeile l�schen
		this->remove(pName, pOldGroup);

		// Zeile einf�gen
		this->write(pName, value, pNewGroup);
	}
}

// move
// Verschiebt Attribut 'pName' aus der Gruppe "default" in die Gruppe 'pNewGroup'
void CConfigFile::move(std::string pName, std::string pNewGroup)
{
	// move
	this->move(pName, "default", pNewGroup);
}


// exists
// �berpr�ft ob die Gruppe 'pGroup' bereits exestiert
// Wenn ja wird die Zeile zur�ckgegeben, ansonsten -1
int CConfigFile::exists(std::string pGroup)
{
	int i = 0;	// Aktuelle Zeile
	for (std::string& s : m_Lines)
	{
		// Exestiert die Gruppe?
		if (s == ("[" + pGroup + "]"))
			return i;
		i++; // N�chste Zeile
	}
	return -1;
}

// exists
// �berpr�ft ob das Attribut 'pName' in der Gruppe 'pGroup' bereits exestiert
// Wenn ja wird die Zeile zur�ckgegeben, ansonsten -1
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
				return i; // Ja, Zeile zur�ckgeben
		}
	}

	// Exestiert nicht, Fehler zur�ckgeben
	return -1;
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
			// Zu Liste hinzuf�gen
			m_Lines.push_back(this->translateCode(stringGL));
	}
}



std::string CConfigFile::translateCode(std::string pLine)
{
	std::string end = ""; // Finaler String

	/* 
	* Leerzeichen  : /l
	* '#'          : rausl�schen
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

	// Zur�ckgeben
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

	// String zur�ckgeben
	return tmp;
}



void CConfigFile::smooth()
{
	// Save
	this->save();

	// Alle leeren zeilen l�schen
	for (int i = 0; i < m_Lines.size(); i++)
		if (m_Lines.at(i) == "")
		{
			m_Lines.erase(m_Lines.begin() + i, m_Lines.begin() + i + 1);
			i--;
		}

	// Nach Abschluss jeder Gruppe Zeile einf�gen
	for (int i = 0; i < m_Lines.size(); i++)
		if (m_Lines.at(i).find("[/") != std::string::npos)
		{
			m_Lines.insert(m_Lines.begin() + i + 1, "");
			i += 1;
		}
}