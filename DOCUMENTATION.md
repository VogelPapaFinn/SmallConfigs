<h2><font style="font-size:80px">How do I get this lib running?</font></h2>

<p>
  This is quite easy. Just download the <i>config_file.h</i> and <i>config_file.cpp</i> file and put them into your solution.
  Then just include the header-file like you always do:
  <br/><code>#include "config_file.h"</code>
</p>

<h2><font style="font-size:80px">How do I use it?</font></h2>

<p>
  It's really simple and self-explanatory. Just take a look at the methods and their name. Is it really that hard?
  
  <ul>
  
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
  </ul>

  Still not enough? Then scroll further and take a look at the descriptions. *Hint* They are included in the source file.

  <h2><font style="font-size:80px">The methods and what they do</font></h2>

  <br/>

  <ul>

	/* 
	* author:  VogelPapaFinn
	* version: 2.1-unstable
	*/

	/*
	 * Hint:
	 * There are no attributes without a group. You cand find them in the [default] group.
	 * But you dont need to care about this, just handle them like they dont have a group.
	 * The lib does the job there!
	 */


	// Constructor
	// Opens the file 'file' or creates it, if it does not exists.
	config_file(const std::string& file);



	// get_line_vector
	// Returns the vector that contains the lines of the file you just opened.
	std::vector<std::string> get_line_vector();



	// file_valid
	// Checks if the 'file' is working.
	// Returns 'true' if it the file doesnt contain any errors.
	static bool file_valid(const std::string& file);



	// open
	// Opens the file 'file' but does NOT create it if it doesnt exist.
	// Returns 'true' if successful.
	bool open(const std::string& file);

	// save
	// Saves everything in the file.
	void save();

	// close
	// Saves the file and closes the stream.
	void close();



	// write
	// Writes the attribute 'name' in the group 'group' and assigns the value 'value'.
	// Creates a new group if 'group' does not exist.
	// Returns 'true' if successful.
	bool write(const std::string& name, const std::string& value);

	// write
	// Writes the attribute 'name' in the group default and assigns the value 'value'.
	// Returns 'true' if successful.
	bool write(const std::string& name, const std::string& value, const std::string& group);



	// get
	// Returns the value of the attribute 'name' in the group 'group'
	// as std::string. Have fun with casting! :)
	std::string get(const std::string& name, const std::string& group);

	// get
	// Returns the value of the attribute 'name' in the group default
	// as std::string. Have fun with casting! :)
	std::string get(const std::string& name);



	// remove
	// Removes the attribute 'name' from the group 'group'.
	void remove(const std::string& name, const std::string& group);

	// remove
	// Removes the attribute 'name' from the group 'group'.
	void remove(const std::string& name, const char* group);

	// remove
	// Removes the attribute 'name' from the group default.
	void remove(const std::string& name);

	// remove
	// Removes the group 'group'. 'move' indicates if the attributes in the group
	// should get moved to the default group.
	void remove(const std::string& group, bool move);

	// remove_from_group
	// Removes the attribute 'name' from the group 'group'.
	void remove_from_group(const std::string& name, const std::string& old_group);



	// move
	// Moves the attribute 'name' from the group 'old_group' to the group 'new_group'.
	void move(const std::string& name, const std::string& old_group, const std::string& new_group);

	// move
	// Moves the attribute 'name' from the group default to the group 'new_group'.
	void move(const std::string& name, const std::string& new_group);



	// exists
	// Checks if the group 'group' exists.
	// Returns the number of the line or '-1' if it does not exist.
	int exists(const std::string& group);

	// exists
	// Checks if the attribute 'name' in the group 'group' exists.
	// Returns the number of the line or '-1' if it does not exist.
	int exists(const std::string& name, const std::string& group);
  </ul>

