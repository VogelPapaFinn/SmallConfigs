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

bool config_file::write_list(const std::string &name, const std::vector<std::string> &value)
{
    // Is the config file opened?
    if (current_state_ == state::closed)
        return false; // No

    // return
    return this->write_list(name, value, "default");
}

bool config_file::write_list(const std::string& name, const std::vector<std::string>& value, const std::string& group)
{
    // Is the config file opened?
    if (current_state_ == state::closed)
        return false; // No

    // Does the attribute in the group exist?
    int line = this->find(name, group);
    if (line != -1)
        // Yes, replace line
        line_vector_.at(line).replace(name.size() + 3, line_vector_.at(line).size() - (name.size() + 3), vector_to_string(value));
    else // No
    {
        // Does the group exist?
        line = this->find(group);
        const std::string tmp = name + ":/l" + vector_to_string(value);
        if (line != -1)
        {
            // Insert new line
            line_vector_.insert(line_vector_.begin() + line + 1, tmp);

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


std::optional<std::string> config_file::get(const std::string& name) const
{
	return get(name, "default").value_or("");
}

std::optional<std::string> config_file::get(const std::string& name, const std::string& group) const
{
	// Does attribute and group exist?
	const int i = this->find(name, group);
	if (i == -1) return std::nullopt; // No

	// Yes return value
	return line_vector_.at(i).substr(name.size() + 3, line_vector_.at(i).size() - (name.size() + 3));
}

std::vector<std::string> config_file::get_list(const std::string& name) const
{
    return get_list(name, "default");
}

std::vector<std::string> config_file::get_list(const std::string& name, const std::string& group) const
{
    // Does attribute and group exist?
    const int line = this->find(name, group);
    if (line == -1) return std::vector<std::string>(); // No

    // Save string to parse
    std::string unparsed_list = line_vector_.at(line).substr(name.size() + 3, line_vector_.at(line).size() - (name.size() + 3));

    // Parse string
    // { list head
    // } list tail
    // , new list element
    std::vector<std::string> output;
    std::string element = "";
    bool list_opened = false;
    for(auto& c : unparsed_list)
    {
        switch(c)
        {
            case '{': list_opened = true; break;
            case '}': list_opened = false; output.push_back(element); break;
            case ',': output.push_back(element); element = ""; break;
            default: element += c;
        }
    }

    return output;
}

std::optional<std::string> config_file::get_raw_list(const std::string& name) const
{
    return get_raw_list(name, "default");
}

std::optional<std::string> config_file::get_raw_list(const std::string& name, const std::string& group) const
{
    // Does attribute and group exist?
    const int line = this->find(name, group);
    if (line == -1) return std::nullopt;

    // Save string to parse
    std::string unparsed_list = line_vector_.at(line).substr(name.size() + 3, line_vector_.at(line).size() - (name.size() + 3));

    return unparsed_list;
}




void config_file::remove(const std::string& name, const std::string& group)
{
	// Does attribute and group exist?
	const int i = this->find(name, group);
	if (i != -1)
		// Delete line
		line_vector_.erase(line_vector_.begin() + i, line_vector_.begin() + i + 1);
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
		const std::string value = this->get(name, old_group).value_or("");

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



int config_file::find(const std::string& group) const
{
    // Search all lines
    for(int l = 0; l < line_vector_.size(); l++)
        if(line_vector_[l].front() == '[' && line_vector_[l].back() == ']' && line_vector_[l].substr(1, line_vector_[l].size() - 2) == group)
            return l;   // Found

	return -1;	// Not found, return nothing
}

int config_file::find(const std::string& name, const std::string& group) const
{
	auto i = this->find(group);	// Current line

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



std::string config_file::vector_to_string(const std::vector<std::string>& vector)
{
    std::string output = "{";

    for(auto& l : vector)
        output += l + ",";

    return output.substr(0, output.size() - 1) + "}";
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
	const int i = tmp.find("/l");
	if (i != std::string::npos)
		tmp.replace(i, 2, " "); // replace '/l' with " "

	// return
	return tmp;
}



void config_file::smooth()
{
	// Delete all empty lines
	for(auto itr = line_vector_.begin(); itr != line_vector_.end(); itr++)
        if(*itr == "")
            itr = line_vector_.erase(itr);

	// Insert empty line after every group [/XXX]
	for (int i = 0; i < line_vector_.size() && line_vector_.at(i) != line_vector_.back(); i++)
		if (line_vector_.at(i).find("[/") != std::string::npos)
		{
			line_vector_.insert(line_vector_.begin() + i + 1, "");
			i += 1;
		}

	// Save
	this->save();
}
