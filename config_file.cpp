#include "config_file.h"
#include <fstream>

config_file::config_file(const std::string& file)
{
	file_ = file;

	line_vector_ = {""};
	
	// Config Datei öffnen
	fstream_.open(file, std::fstream::in);

	// Erfolgreich?
	if (fstream_.is_open())
	{
		current_state_ = state::opened;
		this->read_lines();
	}
	else
	{
		current_state_ = state::closed;

		// Config erstellen
		std::fstream tmp;
		tmp.open(file, std::fstream::out);
		tmp << std::flush;
		tmp.close();

		// Erneut versuchen
		this->open(file);
	}
}



std::vector<std::string> config_file::get_line_vector()
{
	return line_vector_;
}



bool config_file::file_valid(const std::string& file)
{
	// Datei temporär öffnen
	config_file tmp(file);

	// Zeilen
	const std::vector<std::string> line_vector = tmp.get_line_vector();

	// Automaten
	std::vector<char> basic_dea;
	
	// Alle Zeilen überprüfen
	for(auto& l : line_vector)
	{
		// Gruppe wird geöffnet
		if (l.find('[') != std::string::npos &&
			l.find('/') == std::string::npos &&
			l.find(']') != std::string::npos)
		{
			basic_dea.push_back('[');
		}

		// Gruppe wird geschlossen
		else if (l.find('[') != std::string::npos &&
			l.find('/') != std::string::npos &&
			l.find(']') != std::string::npos)
		{
			if(basic_dea.empty() || basic_dea.back() == 'E')
				return false;
			basic_dea.pop_back();
		}

		// fehlerhaftes Attribut
		else if(l.find(':') == std::string::npos && !l.empty())
			basic_dea.push_back('E');  

		// Attribut
		else if(l.find(':') != std::string::npos && basic_dea.empty())
			basic_dea.push_back('E');
	}

	// tmp schließen
	tmp.close();

	// Auswertung
	if(basic_dea.empty())
		return true;
	return false;
}



bool config_file::open(const std::string& file)
{
	// Config Datei öffnen
	fstream_.open(file, std::fstream::in);

	// Erfolgreich?
	if (fstream_.is_open())
	{
		// Ja
		current_state_ = state::opened;
		this->read_lines();
		return true;
	}
	else
		// Nein
		current_state_ = state::closed;
	return false;
}

void config_file::save()
{
	// Datei zum schreiben neu öffnen
	fstream_.close();
	fstream_.open(file_, std::ios::out | std::ios::trunc);
	
	// Alle Zeilen schreiben
	for (std::string& s : line_vector_)
	{
		if(line_vector_.back() == s)
			fstream_ << translate_file(s);
		else
			fstream_ << translate_file(s) << std::endl;
	}

	// Schließen
	fstream_.close();
	current_state_ = state::closed; // STATE ändern

	// Neu öffnen
	if(this->open(file_)) current_state_ = state::opened; // Wieder geöffnet
}

void config_file::close()
{
	// smooth
	this->smooth();

	// save
	this->save();

	// m_fstream schließen
	fstream_.close();

	// STATE ändern
	current_state_ = state::closed;
}



bool config_file::write(const std::string& name, const std::string& value, const std::string& group)
{
	// Config offen?
	if (current_state_ == state::closed)
		return false; // Nein
	// Ja

	// Gibt es das Attribut in der Gruppe schon?
	int i = this->exists(name, group);
	if (i != -1)
		// Ja, also Zeile ersetzen
		line_vector_.at(i).replace(name.size() + 3, line_vector_.at(i).size()-(name.size() + 3), value);
	else // Nein
	{
		// Gibt es die Gruppe überhaupt?
		i = this->exists(group);
		const std::string tmp = name + ":/l" + value; // Neue Zeile, zwischengespeichert
		if (i != -1)
		{
			// Neue Zeile einfügen
			line_vector_.insert(line_vector_.begin() + i + 1, tmp);

			return true;
		}
		else
		{
			// Neue Gruppe einfügen
			line_vector_.insert(line_vector_.end(), "[" + group + "]");
			line_vector_.insert(line_vector_.end(), tmp);
			line_vector_.insert(line_vector_.end(), "[/" + group + "]");
			line_vector_.insert(line_vector_.end(), "");

			return true;
		}
	}

	// Hat nicht geklappt
	return false;
}

bool config_file::write(const std::string& name, const std::string& value)
{
	// Config offen?
	if (current_state_ == state::closed)
		return false; // Nein

	// Gebe Rückgabewert von write zurück
	return this->write(name, value, "default");
}



std::string config_file::get(const std::string& name, const std::string& group)
{
	// Gibt es Attribut & Gruppe überhaupt?
	const int i = this->exists(name, group); // Zeile zwischenspeichern
	if (i == -1) return ""; // Nein
	
	// Ja, Value zurückgeben
	return line_vector_.at(i).substr(name.size() + 3, line_vector_.at(i).size() - (name.size() + 3));
}

std::string config_file::get(const std::string& name)
{
	return this->get(name, "default");
}



void config_file::remove(const std::string& name, const std::string& group)
{
	// Exestiert Attribut und Gruppe überhaupt?
	const int i = this->exists(name, group);
	if (i != -1)
		// Line rauslöschen
		line_vector_.erase(line_vector_.begin() + i, line_vector_.begin() + i + 1);
}

void config_file::remove(const std::string& name, const char* group)
{
	// remove
	this->remove(name, std::string(group));
}

void config_file::remove(const std::string& name)
{
	// remove
	this->remove(name, "default");
}

void config_file::remove(const std::string& group, const bool move)
{
	// Exestiert die Gruppe überhaupt?
	int i = this->exists(group);
	int j = i;

	if (i != -1) // Ja
	{
		// Alles in default verschieben?
		if (move) 
		{
			// Wo ist die default Gruppe?
			const int d = this->exists("default");

			// Alle Zeilen verschieben
			i++; // Will mit Attributen starten, nicht mit Gruppen Header
			while (line_vector_.at(i) != ("[/" + group + "]"))
			{
				// Zeile einfügen
				line_vector_.insert(line_vector_.begin() + d + 1, line_vector_.at(i));

				i = this->exists(group) + 1; // Zeilen verschieben sich

				// Zeile löschen
				line_vector_.erase(line_vector_.begin() + i, line_vector_.begin() + i + 1);

				i = this->exists(group) + 1; // Zeilen verschieben sich
			}

			// Gruppe löschen
			this->remove(group, false);
		}
		else
		{
			// Nein
			while (line_vector_.at(j) != ("[/" + group + "]"))
				j++;
			line_vector_.erase(line_vector_.begin() + i, line_vector_.begin() + j + 1); // Zeilen löschen
		}
	}
}

void config_file::remove_from_group(const std::string& name, const std::string& old_group)
{
	// move
	this->move(name, old_group, "default");
}



void config_file::move(const std::string& name, const std::string& old_group, const std::string& new_group)
{
	if (this->exists(name, old_group) != -1)
	{
		// Wert speichern
		const std::string value = this->get(name, old_group);

		// Zeile löschen
		this->remove(name, old_group);

		// Zeile einfügen
		this->write(name, value, new_group);
	}
}

void config_file::move(const std::string& name, const std::string& new_group)
{
	// move
	this->move(name, "default", new_group);
}



int config_file::exists(const std::string& group)
{
	int i = 0;	// Aktuelle Zeile
	for (std::string& s : line_vector_)
	{
		// Exestiert die Gruppe?
		if (s == ("[" + group + "]"))
			return i;
		i++; // Nächste Zeile
	}
	return -1;
}

int config_file::exists(const std::string& name, const std::string& group)
{
	int i = this->exists(group);	// Aktuelle Zeile

	// Exestiert die Gruppe?
	if (i != -1)
	{
		// Ja, exestiert Attribut?
		for (NULL; line_vector_.at(i) != ("[/" + group + "]"); i++)
		{
			if (line_vector_.at(i).find(name) != std::string::npos && line_vector_.at(i).find(":") != std::string::npos)
				return i; // Ja, Zeile zurückgeben
		}
	}

	// Exestiert nicht, Fehler zurückgeben
	return -1;
}



void config_file::read_lines()
{
	// Config offen?
	if (current_state_ == state::opened)
	{
		// Zum Anfang der Datei springen
		fstream_.clear();
		fstream_.seekg(std::fstream::beg);

		// Liste leeren
		line_vector_.clear();

		// Jede Zeile einlesen
		std::string stringGL;			// Braucht getline();
		while (std::getline(fstream_, stringGL))
			// Zu Liste hinzufügen
			line_vector_.push_back(translate_code(stringGL));
	}
}



std::string config_file::translate_code(const std::string& line)
{
	std::string end; // Finaler String

	/* 
	* Leerzeichen  : /l
	* '#'          : rauslöschen
	* ':'		   : bleibt
	*/
	for (const char c : line)
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

std::string config_file::translate_file(const std::string& line)
{
	/*
	* /l  : " "
	* ':' : bleibt
	*/
	std::string tmp = line;
	const int i = static_cast<int>(tmp.find("/l")); // Char wo '/' steht
	if (i != static_cast<int>(std::string::npos))
		tmp.replace(i, 2, " "); // '/l' durch " " ersetzen

	// String zurückgeben
	return tmp;
}



void config_file::smooth()
{
	// Alle leeren zeilen löschen
	for (int i = 0; i < static_cast<int>(line_vector_.size()); i++)
		if (line_vector_.at(i).empty())
		{
			line_vector_.erase(line_vector_.begin() + i, line_vector_.begin() + i + 1);
			i--;
		}

	// Nach Abschluss jeder Gruppe Zeile einfügen
	for (int i = 0; i < static_cast<int>(line_vector_.size()) && line_vector_.at(i) != line_vector_.back(); i++)
		if (line_vector_.at(i).find("[/") != std::string::npos)
		{
			line_vector_.insert(line_vector_.begin() + i + 1, "");
			i += 1;
		}

	// Save
	this->save();
}