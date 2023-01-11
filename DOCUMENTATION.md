##  The methods and what they do

```
/*
 * Hint:
 * There are no attributes without a group. You cand find them in the [default] group.
 * But you dont need to care about this, just handle them like they dont have a group.
 * The lib does the job there!
 */

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
    /// Saves everything in the file. But doesnt clean it up
    void save();

    /// close
    /// Saves the file, cleans it up and closes the stream.
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

    /// write_list
    /// Writes the attribute 'name' in the group 'group' and assigns the list 'value'.
    /// Creates a new group if 'group' does not exist.
    /// Returns 'true' if successful.
    bool write_list(const std::string& name, const std::vector<std::string>& value);

    /// write
    /// Writes the attribute 'name' in the group default and assigns the list 'value'.
    /// Returns 'true' if successful.
    bool write_list(const std::string& name, const std::vector<std::string>& value, const std::string& group);



    /// get
    /// Returns the value of the attribute 'name' in the group 'group'
    /// as std::string. Have fun with casting! :)
    /// Returns std::nullopt when not successful.
    [[nodiscard]] std::optional<std::string> get(const std::string& name, const std::string& group) const;

    /// get
    /// Returns the value of the attribute 'name' in the group default
    /// as std::string. Have fun with casting! :)
    /// Returns std::nullopt when not successful.
    [[nodiscard]] std::optional<std::string> get(const std::string& name) const;

    /// get_list
    /// Returns the list-value of the attribute 'name' in the group 'group'
    /// as std::vector.
    /// Returns empty vector when not successful.
    [[nodiscard]] std::vector<std::string> get_list(const std::string& name, const std::string& group) const;

    /// get_list
    /// Returns the list-value of the attribute 'name' in the group 'group'
    /// as std::vector.
    /// Returns empty vector when not successful.
    [[nodiscard]] std::vector<std::string> get_list(const std::string& name) const;

    /// get_raw_list
    /// Returns the list-value as string of the attribute 'name' in the group 'group'
    /// Returns std::nullopt when not successful.
    [[nodiscard]] std::optional<std::string> get_raw_list(const std::string& name) const;

    /// get_raw_list
    /// Returns the list-value as string of the attribute 'name' in the group default
    /// Returns std::nullopt when not successful.
    [[nodiscard]] std::optional<std::string> get_raw_list(const std::string& name, const std::string& group) const;



    /// remove
    /// Removes the attribute 'name' from the group 'group'.
    void remove(const std::string& name, const std::string& group);

    /// remove
    /// Removes the attribute 'name' from the group default.
    void remove(const std::string& name);

    /// remove
    /// Removes the group 'group'. 'move' indicates if the attributes in the group
    /// should get moved to the default group.
    void remove(const std::string& group, bool move);

    /// remove_from_group
    /// Removes the attribute 'name' from the group 'group'.
    void remove_from_group(const std::string& name, const std::string& old_group);



    /// move
    /// Moves the attribute 'name' from the group 'old_group' to the group 'new_group'.
    void move(const std::string& name, const std::string& old_group, const std::string& new_group);

    /// move
    /// Moves the attribute 'name' from the group default to the group 'new_group'.
    void move(const std::string& name, const std::string& new_group);



    /// find
    /// Checks if the group 'group' exists.
    /// Returns the number of the line or '-1' if it does not exist.
    int find(const std::string& group) const;

    /// find
    /// Checks if the attribute 'name' in the group 'group' exists.
    /// Returns the number of the line or '-1' if it does not exist.
    int find(const std::string& name, const std::string& group) const;
```
