/* 
* author:  VogelPapaFinn
* version: 2.0-unstable
*/

#pragma once

#include<fstream>
#include<string>
#include<vector>

enum class state
{
    closed,
    opened
};

class config_file
{
public:
    config_file(const std::string& file);

    std::vector<std::string> get_line_vector();

    static bool file_valid(const std::string& file);
	
    bool open(const std::string& file);
    void save();
    void close();

    bool write(const std::string& name, const std::string& value);
    bool write(const std::string& name, const std::string& value, const std::string& group);

    std::string get(const std::string& name, const std::string& group);
    std::string get(const std::string& name);

    void remove(const std::string& name, const std::string& group);
    void remove(const std::string& name, const char* group);
    void remove(const std::string& name);
    void remove(const std::string& group, bool move);
    void remove_from_group(const std::string& name, const std::string& old_group);

    void move(const std::string& name, const std::string& old_group, const std::string& new_group);
    void move(const std::string& name, const std::string& new_group);

    int exists(const std::string& group);
    int exists(const std::string& name, const std::string& group);

private:
    void read_lines();												// Alle Zeilen einlesen

    static std::string translate_code(const std::string& line);		// Übersetzt Zeile zum einfacheren bearbeiten
    static std::string translate_file(const std::string& line);		// Übersetzt Zeile zum einfügen in Config

    void smooth();													// Entfernt unnötige leere Zeilen



    std::string file_;
    std::vector<std::string> line_vector_;							// Speichert alle Zeilen aus der Datei
    state current_state_;											// Aktueller Zustand
    std::fstream fstream_;
};
