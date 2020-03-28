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
  <code>
	std::string getValueOfAttribute(std::string attribute)
  </code>
  <br/>
  und
  <br/>
  <code>
	bool setValueOfAttribute(std::string attribute, std::string value)
  </code>
  <br/>
  sollten sich auch von selbst erklären! Und zu guter letzt:
  <br/>
  <code>
	bool open(std::string file_directory)
  </code>
  <br/>
  und
  <br/>
  <code>
	void close()
  </code>
  <br/>
  öffnen bzw. schließen die aktuelle Datei.
</p>

