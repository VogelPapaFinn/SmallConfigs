<strong><font style="font-size:80px">Wie genau benutze ich die Library?</font></strong>

<p>
  Das ist eigentlich ziemlich einfach, du musst die <i>AConfigFileLib.hpp</i> und <i>AConfigFileLib.cpp</i> in dein Projekt laden.   
  Wenn du sie nun benutzen möchtest, musst du die <i>AConfigFileLib.hpp</i> in deine Quelldatei einbinden: 
  <br/><code>#include "AConfigFileLib.hpp"</code> [Als Beispiel]
</p>

<b><font style="font-size:80px">Wie verwende ich die Library in Code Form?</font></b>

<p>
  Das ist genau so einfach, denn die <i>AConfigFileLib.hpp</i> bietet [bisher] nur fünf Funktionen:
  <ul>
    <li>ConfigFile(std::string directory, MODE WRITE_OR_READ)</li>
    <li>std::string getValueOfAttribute(std::string attribute)</li>
    <li>bool setValueOfAttribute(std::string attribute, std::string value)</li>
    <li>void createAttributeWithValue(std::string attribute, char dot, std::string value);</li>
    <li>void open(std::string file_directory</li>
    <li>void close()</li>
  </ul>
  <i>Eine genaue Beschreibung zu den Funktionen findest du weiter unten.</i>
  <br/>
  <br/>
  Diese erklären sich eigentlich durch den Namen und durch evt. Parameter. So öffnet der Constructor beispielsweise die Datei im 
  angegebenen Modus. Dafür gibt es eine kleine enum im Header:
  <br/>
  <code>
  enum MODE
  { 
	  FILE_READ = std::ios::in,
	  FILE_WRITE = std::ios::out
  };
  </code>
  <br/>
  Wie du siehst, gibt es hier [bisher] nur die Möglichkeit zu lesen und zu schreiben. Wahrscheinlich werden in der Zukunft neue 
  Modi dazu kommen, also einfach mal die Augen offen halten!
  <br/><br/>
  <strong><font style="font-size:80px">Funktionen Beschreibung</font></strong>
  <br/>
  <ul>
  	<li>
		<strong><font style="font-size:20px">ConfigFile(std::string directory, FILE_MODE WRITE_OR_READ)</font></strong><br/>
		Dem Constructor werden der Speicherort der zuöffnenden Datei sowie der Modus, wie dei Datei geöffnet werden 
		soll, übergeben:<br/><br/>
		<code>std::string directory</code> ist der Pfad der zu der Datei führt. <br/><br/>
		<code>FILE_MODE WRITE_OR_READ</code> ist der Modus in dem die Datei geöffnet wird. Bisher stehen hier Lesen 
		<i>FILE_READ</i> oder Schreiben <i>FILE_WRITE</i> zur Auswahl. Diese gehören zum <i>enum FILE_MODE</i> in der 
		Header Datei.
	</li>
	<br/>
	<li>
		<strong><font style="font-size:20px">std::string getValueOfAttribute(std::string attribute)</font></strong><br/>
		Der Funktion <i>getValueOfAttribute()</i> wird nur das Attribute/Variable übergeben, von der man den Wert haben 
		will. Dieser wird dann, in Form eines Strings, zurückgegeben:
		<br/><br/>
		<code>std::string attribute</code> ist der Name des zu suchenden Attributes/Variable.<br/><br/>
		<i>return value:</i> Bei erfolgreichem auslesen des Wertes wird dieser (als ein <i>std::string</i>) 
		zurückgegeben, falls nicht wird <i>ERROR: ATTRIBUTE NOT FOUND</i> zurückgegeben.
	</li>
	<li>
		<strong><font style="font-size:20px">bool setValueOfAttribute(std::string attribute, std::string value)</font></strong><br/>
		Der Funktion <i>setValueOfAttribute()</i> werden der Name des Attributes/Variable und der neue Wert, den diese 	
		annehmen soll, übergeben:
		<br/><br/>
		<code>std::string attribute</code> ist der Name des zu suchenden Attributes/Variable.<br/><br/>
		<code>std::string value</code> ist der neue Wert, den das Attribut/Variable annimmt.<br/><br/>
		<i>return value:</i> Bei erfolgreichem überschreiben des Wertes wird <i>true</i> zurückgegeben. 
		<strong>ACHTUNG:</strong> falls immernoch eine Datei geöffnet wird, wird ebenfalls <i>false</i> zurückgegeben!
	</li>
	<li>
		<strong><font style="font-size:20px">void ConfigFile::createAttributeWithValue(std::string attribute, char dot, std::string value)</font></strong><br/>
		Der Funktion <i>createAttributeWithValue()</i> werden der Name des neuen Attributes, der Zuweisungs-Operator [':';'=';..] und der Wert übergeben:.
		<br/><br/>
		<code>std::string attribute</code> ist der Name des zu erstellenden Attributes.<br/><br/>
		<code>char dot</code> ist der zu nutzende Zuweisungs-Operator.<br/><br/>
		<code>std::string value</code> ist der Wert, den das Attribut annimmt.<br/><br/>
		<br/>
	</li>
	<li>
		<strong><font style="font-size:20px">bool open(std::string file_directory)</font></strong><br/>
		Der Funktion <i>open()</i> werden der Name der zu öffnenden Datei übergeben. Hierbei wird zuerst überprüft, ob 
		bereits eine Datei geöffnet ist. Falls die öffnung der Datei erfolgreich war wird <i>true</i> zurückgegeben:
		<br/><br/>
		<code>std::string file_directory</code> ist der Pfad/Name der zu öffnenden Datei.<br/><br/>
		<i>return value:</i> Bei erfolgreichem öffnen wird <i>true</i> zurückgegeben.
	</li>
	<li>
		<strong><font style="font-size:20px">bool close()</font></strong><br/>
		Der Funktion <i>close()</i> werden keine Parameter übergeben, denn diese schließt nur die aktuell geöffnete 
		Datei.
		<br/>
	</li>
  </ul>
</p>
