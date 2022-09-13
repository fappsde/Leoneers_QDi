# Future Engineers - Leoneers_QDi - Deutschlandfinale

Dieses Repository bietet Einblicke in den Weiterentwicklungsprozess unseres autonom fahrenden Roboterautos nach unserem Vorentscheid in Dortmund, welches wir im Rahmen der Future Engineers Category der World Robot Olympiad (WRO) konzipiert und programmiert haben. 
Die alte Dokumentation des Vorentscheids ist diesem [Branch](https://github.com/fappsde/Leoneers_QDi/blob/Vorentscheid_Dortmund/README.md) zu finden.

# Inhalt
  * Bilder
  * Programmcode
    * Module des Codes
    * Zusammenhang der Module und der elektromechatronischen Komponenten
    * Prozess des Erstellens, Kompilierens und Hochladens auf die Steuergeräte
  * Technische Zeichnungen
  * Video
  
# Einleitung
Wir, dass sind Felix und Philipp aus Aachen, haben seit der 5. Klasse jährlich an der WRO teilgenommen. Dementsprechend war die WRO eine Konstante in unserer Schulzeit, auf die wir uns jedes Jahr aufs neue freuten. Eigentlich wollten wir im Jahr 2020 das letzte Mal vor unserem Abitur 2021 in der Football Category antreten. Daraus wurde leider nichts. Umso größer war die Freude als wir feststellten, dass wir dieses Jahr ein aller letztes Mal die Möglichkeit haben an der WRO teilnehmen zu können. Zwar stellten wir fest, dass die Football Category, welche uns die vergangenen Jahre immer viel Spaß bereitete, nicht mehr ausgetragen wird, aber genau so spannend und interessant erschienen uns die neuen Kategorien der WRO. 
Da uns beide die Thematik der zukünftigen Mobilität und das autonome Fahren interessiert, entschieden wir uns dazu an der Future Category teilzunehmen, und ein autonomfahrendes Auto zu konzipieren.

## Anfängliche Entscheidungen

Nachdem wir die Entscheidung getroffen hatten, dass wir teilnehmen wollen, kam die Frage auf mit welchem Roboter System wir die Herausforderung des Autonomenfahrens meistern wollen? In der Vergangenheit arbeiteten wir immer gerne mit dem EV3 und Lego Technik. Jedoch stießen wir insbesondere die Programmierung betreffend mit der EV3 eigenen Programmiersprache auf Grenzen. Aus diesem Grund entschlossen wir uns dazu eine neue Herausforderung anzunehmen, und die EV3 Welt hinter uns zu lassen. Mit dieser Entscheidung dem Mindstorms Kosmos den Rücken zu zukehren, waren wir nun allerdings mit der Frage konfrontiert für welches der zahlreichen Robotersysteme, welche auf dem Markt erhältlich sind, wir uns entscheiden.

## Aufbau des Roboterautos

Zunächst überlegten wir uns das Roboterauto von Grund auf selbst mittels CAD und additiver Herstellung zu konstruieren. Jedoch wurde uns schnell klar, dass dies zeitlich neben dem Studium leider nicht möglich gewesen wäre. Deshalb wählten wir ein altes ferngesteuertes Auto als Chassis unseres Roboterautos. Als EV3 Stein Ersatz arbeiteten wir von nun an mit einem Raspberry Pi CM4. Des Weiteren verfügt das Auto über zwei Raspberry Pi Kameras, welche wir für das autonome Fahren und das Erkennen der Hindernisse verwenden.
Damit der Antriebs- und der Servomotor vernünftig vom Raspberry Pi angesteuert werden können, mussten feststellen, dass ein ESP 32 Mikrocontroller notwendig ist, da das PWM (Pulsweitenmodulation) Signal ansonsten nicht konstant gewesen wäre, was ein Zittern des Servomotors verursacht hätte. 
Mehr Information über den Aufbau des Programmes und den Zusammenhang des Codes und der elektromechatronischen Komponenten, finden Sie beim [Programmcode](https://github.com/fappsde/Leoneers_QDi/tree/main/Programmcode).

## Autonomes Fahren und Hinderniserkennung

Als wir mit dem Aufbauprozess des Autos soweit fertig waren, konnten wir uns ab diesem Zeitpunkt an die eigentlichen Aufgaben des autonomen Fahrens und der Hinderniserkennung begeben. Die erste Idee, die wir für die Orientierung auf dem Spielfeld verfolgten, war die Verwendung von Lidar Sensoren, welche verbauten wir vorne und jeweils an den Seiten des Autos. Lidar Sensoren werden bereits für das autonome Fahren in Kraftfahrzeuge verbaut, da diese ein Abbild der Umgebung erzeugen können. Dadurch, dass die Lidar Sensoren mit Infrarotlicht arbeiten, stellten wir fest, dass dies mit der schwarzen Bande des Spielfeldes nicht kompatibel war, weshalb wir die Idee der Lidar Sensoren verwerfen mussten.
Die Alternative auf die wir uns von dort an konzentrierten, war die Verwendung der Kameras. Wir arbeiten mit zwei Kameras, da wir dadurch einen größeren Öffnungswinkel erzeugen können. Mithilfe der Kameras gelingt es uns die Banden und die Hindernisse zu erkennen, d.h. die Hauptbestandteile unseres Programms bestehen aus Bilderkennungsalgorithmen.
# Technische Komponenten

* Raspberry Pi CM4
* ESP 32 Mikrocontroller
* Raspberry Pi Kamera
* Gyro Sensor
