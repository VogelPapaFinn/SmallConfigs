<h2><font style="font-size:80px">Wie genau benutze ich die Library?</font></h2>

<p>
  Das ist eigentlich ziemlich einfach, du musst die <i>CConfigFile.hpp</i> und <i>CConfigFile.cpp</i> in dein Projekt laden.   
  Wenn du sie nun benutzen möchtest, musst du die <i>CConfigFile.hpp</i> in deine Quelldatei einbinden: 
  <br/><code>#include "CConfigFile.hpp"</code> [Als Beispiel]
</p>

<h2><font style="font-size:80px">Wie verwende ich die Library in Code Form?</font></h2>

<p>
  Das ist genau so einfach, denn die <i>CConfigFile.hpp</i> bietet akutell 15 Funktionen (überladene mitgezählt):
  <ul>
	  
    CConfigFile(std::string pFile)
	  
    bool open(std::string pFile)
    void save()
    void close()
	  
    bool write(std::string pName, std::string pValue)
    bool write(std::string pName, std::string pValue, std::string pGroup)
	  
    void remove(std::string pName, std::string pGroup)
    void remove(std::string pName, const char* pGroup)
    void remove(std::string pName)
    void remove(std::string pGroup, bool pMove)
    void removeFromGroup(std::string pName, std::string pOldGroup)
	  
    void move(std::string pName, std::string pOldGroup, std::string pNewGroup)
    void move(std::string pName, std::string pNewGroup)
	  
    std::string get(std::string pName, std::string pGroup)
    std::string get(std::string pName)
	  
    Eine genaue Beschreibung zu den Funktionen findest du weiter unten.
  </ul>
  <h2><font style="font-size:80px">Funktionen Beschreibung - Auffindbar im Sourcecode</font></h2>
  <br/>
  <ul>
	
	/* 
	* author:  VogelPapaFinn
	* version: 2.0-unstable
	*/

	// Wenn einem Attribut keine Gruppe zugewiesen wird, wird es automatisch der Gruppe "default" zugewiesen.
	// Genau genommen gibt es keine gruppenlose Attribute. Sie können vom Anwender aber so behandelt werden.


	// Constructor
	//  Öffnet die Datei 'pFile' oder legt eine an, falls noch keine exestiert.
	CConfigFile::CConfigFile(std::string pFile);
	
	
	
	// open
	// Öffnet die Datei 'pFile', falls diese noch nicht exestiert wird KEINE angelegt.
	// Gibt 'true' zurück, wenn erfolgreich
	bool CConfigFile::open(std::string pFile);
	
	// save
	// Speichert alle Änderungen in der Datei
	void CConfigFile::save();
	
	// close
	// Löscht alle unnötigen leeren Zeilen, Speichert die Datei und schließt sie
	void CConfigFile::close();
	
	
	
	// write
	// Schreibt das Attribut 'pName' mit dem Wert 'pValue' in die Gruppe 'pGroup'.
	// Falls es noch keine exestiert wird eine erstellt.
	// Gibt 'true' zurück, wenn erfolgreich
	bool CConfigFile::write(std::string pName, std::string pValue, std::string pGroup;
	
	// write
	// Schreibt das Attribut 'pName' mit dem Wert 'pValue' in die Gruppe "default"
	// Gibt 'true' zurück, wenn erfolgreich
	bool CConfigFile::write(std::string pName, std::string pValue);
	
	
	
	// get
	// Liest Wert von Attribut 'pName' in der Gruppe 'pGroup' aus
	// Gibt Wert als 'std::string' zurück
	std::string CConfigFile::get(std::string pName, std::string pGroup);
	
	// get
	// Liest Wert von Attribut 'pName' in der Gruppe "default" aus
	// Gibt Wert als 'std::string' zurück
	std::string CConfigFile::get(std::string pName);
	


	// remove
	// Löscht Attribut 'pName' aus Gruppe 'pGroup'
	void CConfigFile::remove(std::string pName, std::string pGroup);
	
	// remove
	// Löscht Attribut 'pName' aus Gruppe 'pGroup'
	void CConfigFile::remove(std::string pName, const char* pGroup);
	
	// remove
	// Löscht Attribut 'pName' aus Gruppe "default"
	void CConfigFile::remove(std::string pName);
	
	// remove
	// Löscht Gruppe 'pGroup'. Wenn 'pMove' true ist, werden alle Attribute der Gruppe in "default" verschoben
	void CConfigFile::remove(std::string pGroup, bool pMove);
	
	// remove
	// Löscht Attribut 'pName' aus Gruppe 'pGroup' und fügt es in die Gruppe "default" ein
	void CConfigFile::removeFromGroup(std::string pName, std::string pOldGroup);
	
	
	
	// move
	// Verschiebt Attribut 'pName' aus der Gruppe 'pOldGroup' in die Gruppe 'pNewGroup'
	void CConfigFile::move(std::string pName, std::string pOldGroup, std::string pNewGroup);
	
	// move
	// Verschiebt Attribut 'pName' aus der Gruppe "default" in die Gruppe 'pNewGroup'
	void CConfigFile::move(std::string pName, std::string pNewGroup)
  </ul>
</p>
