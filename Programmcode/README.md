# Programmstruktur
Im Wesentlichen lässt sich unsere Programmstruktur in zwei Teile seperieren, einerseits das Programm für die ESP und andererseits die Programme der beiden Raspberry Pi's.  
Als Schnittstelle zwischen dem Antrieb des Roboterautos und dem einen dafür zuständigen Raspberry Pi fungiert ein ESP, welcher mittels Arduino programmiert wurde. Die Raspberry Pi's programmierten wir mit C und C++, da wir im Entwicklungsprozess feststellten, dass Python sich auf die Geschwindigkeit negativ ausgewirkt hätte. 

## Module des Codes

Unser Code lässt sich in mehrere Teile unterteilen, dieser verfügt unter anderem über ein Hauptprogramm. Des Weiteren über ein Programm, welches die Kameras auswertet und die Daten permanent an das Hauptprogramm übermittelt. Außerdem gibt es noch ein Programm, das den Gyro Sensor ausließt. Diese Teile waren bereits der ersten Version. Bei unserer jetzigen Version sind noch die Programmteile hinzugekommen, die den Lidar und die Positionskamera auswerten und das Kartographieren der Karte übernehmen. Das Hauptprogramm, welches auf dem Raspberry Pi läuft, dient dann dazu alle Daten zusammen fließen zu lassen und die Geschwindigkeit und Richtung des Autos vorzugeben.

## Zusammenhang der Module und der elektromechatronischen Komponenten
Damit die Motoren jedoch angesteuert werden können, übermittelt das Hauptprogramm zunächst die Informationen an den einen ESP, der dann den Antriebs- und die Servomotoren ansteuert. Dementsprechend lässt sich sagen, dass der eine ESP die Aufgabe erfüllt die Motoren anzusteuern.

## Erstellen/Kompilieren/Hochladen
Die ESP's werden über Arduino kompiliert und das Programm anschließend mittels USB hochgeladen. Auf den Raspberry Pi's werden die Programme direkt geschrieben und beim Power Up gestartet. 
