#include "config_file.h"

config_file::config_file(const std::string& file)
{
	file_ = file;

	line_vector_ = {""};
	
	// Open config file
	fstream_.open(file, std::fstream::in);

	// Did it work?
	if (fstream_.is_open())
	{
		current_state_ = state::opened;
		this->read_lines();
	}
	else
	{
		current_state_ = state::closed;

		// Create config file
		std::fstream tmp;
		tmp.open(file, std::fstream::out);
		tmp << std::flush;
		tmp.close();

		// Retry
		this->open(file);
	}
}



std::vector<std::string> config_file::get_line_vector()
{
	return line_vector_;
}



bool config_file::file_valid(const std::string& file)
{
	// Open file temporary
	config_file tmp(file);

	// Get lines
	const std::vector<std::string> line_vector = tmp.get_line_vector();

	// really basic DEA
	std::vector<char> basic_dea;
	
	// Check all lines
	for(auto& l : line_vector)
	{
		// Group opened
		if (l.find('[') != std::string::npos &&
			l.find('/') == std::string::npos &&
			l.find(']') != std::string::npos)
		{
			basic_dea.push_back('[');
		}

		// Group closed
		else if (l.find('[') != std::string::npos &&
			l.find('/') != std::string::npos &&
			l.find(']') != std::string::npos)
		{
			if(basic_dea.empty() || basic_dea.back() == 'E')
				return false;
			basic_dea.pop_back();
		}

		// Attribute without group - ERROR
		else if(l.find(':') == std::string::npos && !l.empty())
			basic_dea.push_back('E');  

		// Attribute
		else if(l.find(':') != std::string::npos && basic_dea.empty())
			basic_dea.push_back('E');
	}

	// Close temporary config
	tmp.close();

	// return
	if(basic_dea.empty())
		return true;
	return false;
}



bool config_file::open(const std::string& file)
{
	// Open config file
	fstream_.open(file, std::fstream::in);

	// Did it work?
	if (fstream_.is_open())
	{
		// Yes
		current_state_ = state::opened;
		this->read_lines();
		return true;
	}
	else
		// No
		current_state_ = state::closed;
	return false;
}

void config_file::save()
{
	// Reopen config to write
	fstream_.close();
	fstream_.open(file_, std::ios::out | std::ios::trunc);
	
	// Push all lines
	for (std::string& s : line_vector_)
	{
		if(line_vector_.back() == s)
			fstream_ << translate_file(s);
		else
			fstream_ << translate_file(s) << std::endl;
	}

	// Close stream
	fstream_.close();
	current_state_ = state::closed; // change state

	// Reopen file
	if(this->open(file_)) current_state_ = state::opened;
}

void config_file::close()
{
	// smooth
	this->smooth();

	// save
	this->save();

	// close m_fstream
	fstream_.close();

	// change state
	current_state_ = state::closed;
}



bool config_file::write(const std::string& name, const std::string& value)
{
	// Is the config file opened?
	if (current_state_ == state::closed)
		return false; // No

	// return
	return this->write(name, value, "default");
}

bool config_file::write(const std::string& name, const std::string& value, const std::string& group)
{
	// Is the config file opened?
	if (current_state_ == state::closed)
		return false; // No
	// Yes

	// Does the attribute in the group exist?
	int i = this->find(name, group);
	if (i != -1)
		// Yes, replace line
		line_vector_.at(i).replace(name.size() + 3, line_vector_.at(i).size()-(name.size() + 3), value);
	else // No
	{
		// Does the group exist?
		i = this->find(group);
		const std::string tmp = name + ":/l" + value;
		if (i != -1)
		{
			// Insert new line
			line_vector_.insert(line_vector_.begin() + i + 1, tmp);

			return true;
		}
		else
		{
			// Create new group
			line_vector_.insert(line_vector_.end(), "[" + group + "]");
			line_vector_.insert(line_vector_.end(), tmp);
			line_vector_.insert(line_vector_.end(), "[/" + group + "]");
			line_vector_.insert(line_vector_.end(), "");

			return true;
		}
	}

	// Didnt work
	return false;
}



std::string config_file::get(const std::string& name)
{
	return get(name, "default");
}

std::string config_file::get(const std::string& name, const std::string& group)
{
	// Does attribute and group exist?
	const int i = this->find(name, group);
	if (i == -1) return ""; // No
	
	// Yes return value
	return line_vector_.at(i).substr(name.size() + 3, line_vector_.at(i).size() - (name.size() + 3));
}



void config_file::remove(const std::string& name, const std::string& group)
{
	// Does attribute and group exist?
	const int i = this->find(name, group);
	if (i != -1)
		// Delete line
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
	// Does group exist?
	int i = this->find(group);
	int j = i;

	if (i != -1) // Yes
	{
		// Move everything to default?
		if (move) 
		{
			// Where is the default group?
			const int d = this->find("default");

			// Move all lines
			i++; // Otherwise it moves the header of the group [XXX]
			while (line_vector_.at(i) != ("[/" + group + "]"))
			{
				// Insert line
				line_vector_.insert(line_vector_.begin() + d + 1, line_vector_.at(i));

				i = this->find(group) + 1; // Lines moved

				// Delete line
				line_vector_.erase(line_vector_.begin() + i, line_vector_.begin() + i + 1);

				i = this->find(group) + 1; // Lines moved
			}

			// Delete group
			this->remove(group, false);
		}
		else
		{
			// No
			while (line_vector_.at(j) != ("[/" + group + "]"))
				j++;
			line_vector_.erase(line_vector_.begin() + i, line_vector_.begin() + j + 1); // Delete line
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
	if (this->find(name, old_group) != -1)
	{
		// Save value
		const std::string value = this->get(name, old_group);

		// Delete line
		this->remove(name, old_group);

		// Insert line
		this->write(name, value, new_group);
	}
}

void config_file::move(const std::string& name, const std::string& new_group)
{
	// move
	this->move(name, "default", new_group);
}



int config_file::find(const std::string& group)
{
	int i = 0;	// Current line
	for (std::string& s : line_vector_)
	{
		// Does the group exist?
		if (s == ("[" + group + "]"))
			return i;
		i++; // Next line
	}
	return -1;
}

int config_file::find(const std::string& name, const std::string& group)
{
	int i = this->find(group);	// Current line

	// Does the group exist?
	if (i != -1)
	{
		// Yes, does the attribute exist?
		for (NULL; line_vector_.at(i) != ("[/" + group + "]"); i++)
		{
			if (line_vector_.at(i).find(name) != std::string::npos && line_vector_.at(i).find(":") != std::string::npos)
				return i; // Yes, return line
		}
	}

	// No, return error
	return -1;
}



void config_file::read_lines()
{
	// Is the config file opened?
	if (current_state_ == state::opened)
	{
		// Jump to top of file
		fstream_.clear();
		fstream_.seekg(std::fstream::beg);

		// Clear vector
		line_vector_.clear();

		// Read lines
		std::string stringGL;			// needed for getline();
		while (std::getline(fstream_, stringGL))
			// push into vector
			line_vector_.push_back(translate_code(stringGL));
	}
}



std::string config_file::translate_code(const std::string& line)
{
	std::string end; // return string

	/* 
	* space		   : /l
	* '#'          : gets deleted
	* ':'		   : stays
	*/
	for (const char c : line)
	{
		switch (c)
		{
			// space
			case(' '):
				end += "/l";
				break;

			// #
			case('#'):
				break;

			// any other character
			default:
				end += c;
				break;
		}
	}

	// return
	return end;
}

std::string config_file::translate_file(const std::string& line)
{
	/*
	* /l  : " "
	* ':' : stays
	*/
	std::string tmp = line;
	const int i = static_cast<int>(tmp.find("/l"));
	if (i != static_cast<int>(std::string::npos))
		tmp.replace(i, 2, " "); // replace '/l' with " "

	// return
	return tmp;
}



void config_file::smooth()
{
	// Delete all empty lines
	std::erase(line_vector_, "");

	// Insert empty line after every group [/XXX]
	for (int i = 0; i < static_cast<int>(line_vector_.size()) && line_vector_.at(i) != line_vector_.back(); i++)
		if (line_vector_.at(i).find("[/") != std::string::npos)
		{
			line_vector_.insert(line_vector_.begin() + i + 1, "");
			i += 1;
		}

	// Save
	this->save();
}