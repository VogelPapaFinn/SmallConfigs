# Small Configs

With this C++ lib you can use basic configuration files. It is really easy to use and doesnt need to be compiled.

### Installation

To use this lib just follow these really easy instructions:

```
Download or clone the repo
$ git clone https://github.com/VogelPapaFinn/SmallConfigs

Now include these two files to your project:
- config_file.h
- config_file.cpp

Include the header file where you want to use the lib:
$ #include "config_file.h"
```

### Usage

The lib is simple to use and all of the functions are self-explanatory. Look at the documentation file for further information.

```
$ config_file c("config.ini")

$ c.write("att1", "value", "group")
$ c.write("foo", "bar", "root")

$ c.write_list("List1", std::array<std::string, 3>{"1", "2", "3"})
$ c.write_list("List1", std::vector<std::string>{"1", "2", "3"})
$ c.write_list("List1", std::list<std::string>{"1", "2", "3"})

$ c.get("att1")
$ c.get("att1", "root")

$ c.get_list_as_vector("List1")
$ c.get_raw_list("List1")

$ c.file_valid("config.ini")
```

### Features

* Choose your own filename and datatype e.g. ``.ini`` or ``.cfg``
* Group-System - currently everything belongs to a group
* Lists are supported [basic atm]
* Basic operations
  * adding attributes and values
  * changing & deleting them
  * creating & deleting groups
  * moving attributes between groups

### Planned Features

- Improved support for lists.

If you have any ideas to improve the lib create a issue or send me a message on discord.

### Social Media - Contact

* Discord: ``VogelPapa | Finn#0442``
