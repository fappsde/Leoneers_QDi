# Programmstruktur
Im Wesentlichen lässt sich unsere Programmstruktur in zwei Teile seperieren, einerseits das Programm für den ESP und andererseits das Programm des Raspberry Pi's.  
Als Schnittstelle zwischen dem Antrieb des Roboterautos und dem Raspberry Pi fungiert der ESP, welcher mittels Arduino programmiert wurde. Den Raspberry programmierten wir mit C und C++, da wir im Entwicklungsprozess feststellten, dass Python sich auf die Geschwindigkeit negativ auswirkt hätte. 

## Module des Codes

Unser Code lässt sich in mehrere Teile unterteilen, dieser verfügt unter anderem über ein Hauptprogramm. Des Weiteren über ein Programm, welches die Kameras auswertet und die Daten permanent an das Hauptprogramm übermittelt. Außerdem gibt es noch ein Programm, das den Gyro Sensor ausließt. Das Hauptprogramm, welches auf dem Raspberry Pi läuft, dient dann dazu alle Daten zusammen fließen zu lassen und die Geschwindigkeit und Richtung des Roboterautos zu bestimmen. 

## Zusammenhang der Module und der elektromechatronischen Komponenten
Damit die Motoren jedoch angesteuert werden können, übermittelt das Hauptprogramm zunächst die Informationen an des ESP, der dann den Antriebs- und Servomotor ansteuert. Dementsprechend lässt sich sagen, dass der ESP die Aufgabe erfüllt die Motoren anzusteuern.

## Erstellen/Kompilieren/Hochladen
Der ESP wird über Arduino kompiliert und das Programm anschließend mittels USB hochgeladen. Auf den Raspberry Pi werden die Programme direkt geschrieben und beim Power Up gestartet. 
