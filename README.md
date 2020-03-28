# AConfigFileLib
Eine kleine Library die Config-Dateien ver- und bearbeiten kann.

# Features:
- Individueller Dateiname und Dateiendung
- Auslesen von Werten
- Ändern von Werten
- Freiheit bei Zuweisung-"Operator" (egal ob : =)

# Kommende Features:
- Gruppen-System:
    Ich habe vor in den nächsten Wochen ein Gruppen-System hinzuzufügen, damit man mehrere Variablen zusammenfassen kann
- Hinzufügen neuer Attribute:
    Es wird noch die möglichkeit kommen, dass man Attribute im Programm hinzufügen kann. Im Moment muss man dies noch von Hand       
    machen

# Format:
Das Format sieht im Moment noch so aus:
<code>Variable = Wert</code>
Hier kann das "=" durch jedes beliebige ZEICHEN ersetzt werden. Aber Achtung: Es darf nur EIN ZEICHEN verwendet werden! Wenn mehr als ein Zeichen verwendet wird, wird dieses zum Wert gezählt!

# Kommentare:
Kommentare können wie gewohnt mit "#" hinzugefügt werden. Aber Achtung, alles nach einem Kommentar wird ebenfalls als Kommentar gezählt! Als Beispiel:

<code>MenschenAufDerWelt = 5 # Alle Menschen auf der Welt</code>

Das funktioniert, das wiederum nicht:

<code>#Menschen auf der Welt MenschenAufDerWelt = 5</code>
