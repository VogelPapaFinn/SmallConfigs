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
</p>

