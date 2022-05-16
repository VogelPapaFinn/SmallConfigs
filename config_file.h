/* 
* author:  VogelPapaFinn
* version: 2.0-unstable
*/

#pragma once

#include<fstream>
#include<string>
#include<vector>

/*
* Hint:
* There are no attributes without a group. You cand find them in the [default] group.
* But you dont need to care about this, just handle them like they dont have a group.
* The lib does the job there!
*/

enum class state
{
    closed,
    opened
};

class config_file
{
public:
    /// Constructor
    /// Opens the file 'file' or creates it, if it does not exists.
    config_file(const std::string& file);



    /// get_line_vector
    /// Returns the vector that contains the lines of the file you just opened.
    std::vector<std::string> get_line_vector();



    /// file_valid
    /// Checks if the 'file' is working.
    /// Returns 'true' if it the file doesnt contain any errors.
    static bool file_valid(const std::string& file);
	


    /// open
    /// Opens the file 'file' but does NOT create it if it doesnt exist.
    /// Returns 'true' if successful.
    bool open(const std::string& file);
    
    /// save
    /// Saves everything in the file.
    void save();

    /// close
    /// Saves the file and closes the stream.
    void close();



    /// write
    /// Writes the attribute 'name' in the group 'group' and assigns the value 'value'.
    /// Creates a new group if 'group' does not exist.
    /// Returns 'true' if successful.
    bool write(const std::string& name, const std::string& value);
    
    /// write
    /// Writes the attribute 'name' in the group default and assigns the value 'value'.
    /// Returns 'true' if successful.
    bool write(const std::string& name, const std::string& value, const std::string& group);
    


    /// get
    /// Returns the value of the attribute 'name' in the group 'group'
    /// as std::string. Have fun with casting! :)
    std::string get(const std::string& name, const std::string& group);
    
    /// get
    /// Returns the value of the attribute 'name' in the group default
    /// as std::string. Have fun with casting! :)
    std::string get(const std::string& name);

    

    /// remove
    /// Removes the attribute 'name' from the group 'group'.
    void remove(const std::string& name, const std::string& group);
    
    /// remove
    /// Removes the attribute 'name' from the group 'group'.
    void remove(const std::string& name, const char* group);
    
    /// remove
    /// Removes the attribute 'name' from the group default.
    void remove(const std::string& name);
    
    /// remove
    /// Removes the group 'group'. 'move' indicates if the attributes in the group
    /// should get moved to the default group.
    void remove(const std::string& group, bool move);
    
    /// remove_from_group
    /// Removes the attribute 'name' from the group 'group'.
    void remove_from_group(const std::string& name, const std::string& group);



    /// move
    /// Moves the attribute 'name' from the group 'old_group' to the group 'new_group'.
    void move(const std::string& name, const std::string& old_group, const std::string& new_group);

    /// move
    /// Moves the attribute 'name' from the group default to the group 'new_group'.
    void move(const std::string& name, const std::string& new_group);



    /// exists
    /// Checks if the group 'group' exists.
    /// Returns the number of the line or '-1' if it does not exist.
    int exists(const std::string& group);
    
    /// exists
    /// Checks if the attribute 'name' in the group 'group' exists.
    /// Returns the number of the line or '-1' if it does not exist.
    int exists(const std::string& name, const std::string& group);

private:
    void read_lines();												/// read all lines

    static std::string translate_code(const std::string& line);		/// Translates code to file
    static std::string translate_file(const std::string& line);		/// Translates file to code

    void smooth();													/// smoothes the code



    std::string file_;
    std::vector<std::string> line_vector_;							/// Contains all the lines from the file
    state current_state_;											/// Current state
    std::fstream fstream_;
};
