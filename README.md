# Future Engineers - Leoneers_QDi - Deutschlandfinale

Dieses Repository bietet Einblicke in den Weiterentwicklungsprozess unseres autonom fahrenden Roboterautos nach unserem Vorentscheid in Dortmund, welches wir im Rahmen der Future Engineers Category der World Robot Olympiad (WRO) konzipiert und programmiert haben. 
Die unveränderte Dokumentation des Vorentscheids ist diesem [Branch](https://github.com/fappsde/Leoneers_QDi/blob/Vorentscheid_Dortmund/README.md) zu finden.

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

Nachdem wir die Entscheidung getroffen hatten, dass wir teilnehmen wollen, kam die Frage auf mit welchem Roboter System wir die Herausforderung des Autonomenfahrens meistern wollen? In der Vergangenheit arbeiteten wir immer gerne mit dem EV3 und Lego Technik. Jedoch stießen wir insbesondere die Programmierung betreffend mit der EV3 eigenen Programmiersprache auf Grenzen. Aus diesem Grund entschlossen wir uns dazu eine neue Herausforderung anzunehmen, und die EV3 Welt hinter uns zu lassen. Mit dieser Entscheidung dem Mindstorms Kosmos den Rücken zu zukehren, waren wir nun allerdings mit der Frage konfrontiert für welches der zahlreichen Robotersysteme, welche auf dem Markt erhältlich sind, wir uns entscheiden wollten.

Zunächst überlegten wir uns das Roboterauto von Grund auf selbst mittels CAD und additiver Herstellung zu konstruieren. Jedoch wurde uns schnell klar, dass dies zeitlich neben dem Studium leider nicht möglich gewesen wäre. Deshalb wählten wir ein altes ferngesteuertes Auto als Chassis unseres ersten Roboterautos für die Teilnahme am Vorentscheid. Als EV3 Stein Ersatz arbeiteten wir von nun an mit einem Raspberry Pi CM4. Des Weiteren verfügte das erste Auto über zwei Raspberry Pi Kameras, welche wir für das autonome Fahren und das Erkennen der Hindernisse verwendeten. Damit der Antriebs- und der Servomotor vernünftig vom Raspberry Pi angesteuert werden können, mussten wir feststellen, dass ein ESP 32 Mikrocontroller notwendig war, da das PWM (Pulsweitenmodulation) Signal ansonsten nicht konstant gewesen wäre, was ein Zittern des Servomotors verursacht hätte. Mit dieser technischen Ausstattung fuhren wir zum Vorentscheid nach Dortmund.

## Vorentscheid Dortmund

Aufgrund unserer zahlreichen Teilnahmen seit der 5. Klasse an der WRO wissen wir, dass der eigentliche Wettbewerbstag immer etwas ganz besonderes ist. Nicht nur, weil es ein schönes Ereignis in einer uns fremden Stadt ist, sondern auch, weil an diesem Tag die intensive Arbeit der letzten Wochen und Monate auf die Probe gestellt. Des Öfteren fuhren wir mit einem sehr guten Gefühl zum Wettbewerb, und mussten dann leider feststellen, dass wir uns getäuscht hatten, da der Roboter auf einmal nicht mehr das machte, was er sollte bspw. aufgrund der örtlichen Gegebenheiten, welche die Sensorik anders beeinflusst als bei der Vorbereitung. Dementsprechend lernten wir über die Jahre mit Überraschungen am Wettbewerbstag umgehen zu können. 
Ein ähnliches Erlebnis hatten wir auch bei unserem Vorentscheid in Dortmund im Juni. Zuhause und vor den Wettbewerbsdurchläufen vor Ort alles perfekt läuft. Doch dann kommt der entscheidende Durchlauf, wo unser Auto auf einmal nach der ersten Kurve an der Bande hängen blieb, was in diesem Augenblick natürlich sehr ärgerlich und frustrierend ist. Aber auch das gehört zur WRO dazu.

Dennoch hatten wir das Glück, dass wir uns als 1. Platz für das Deutschlandfinale qualifizieren konnten. Wir freuen uns sehr auf diese neue Herausforderung, denn in Dortmund stellten wir die folgenden Probleme fest, die es zu lösen galt:
 * Zu großer Wenderadius, wodurch das Auto die Kurven zu groß fuhr und auch nicht flexibel genug den Hindernissen ausweichen konnte.
 * Die Bandenerkennung, die wir mittels einer Bilderkennung implementierten, war vor Ort fehleranfällig, da das Bild der Kamera überbelichtet war, sodass  die schwarze Bande nicht mehr erkannt werden konnte. 

## Umbau des Roboterautos
Mit den in Dortmund festgestellten Problemen begann für uns zu Hause wieder die Arbeit, diese Probleme zu beheben. Dadurch, dass der Wenderadius zu groß war und auch an dem alten Modell nicht mehr vergrößert werden konnte, mussten wir um diesen zu verkleinern noch eine Hinterradlenkung einbauen. Wir entschieden uns einen zweiten Prototypen zu entwickeln, um das Auto, was bereits funktionierte auch wenn nicht perfekt, nicht zu demontieren, um so ggf. auf ein Backup zurückgreifen zu können. 

### Doppelachslenkung

Damit wir die Hinterradlenkung verbauen konnten, mussten wir zunächst die aktuelle Bodenplatte des Roboterautos neu konstruieren, um jeweils vorne und hinten eine Lenkeinheit verbauen zu können. Außerdem musste die Position des Motors geändert werden und ein weiterer Servomotor musste verbaut werden. Zunächst fertigten wir eine technische Zeichnung auf Papier an um diese anschließen in das CAM Programm zu übertragen. Dadurch, dass wir die neue Bodenplatte aus Aluminium frästen, waren diese Arbeitsschritte notwendig. 

### Elektromechanische Komponenten und Platinendesign 

Eine weitere Umbaumaßnahme ging mit dem Design einer eigenen Platine einher. Damit entstand wohl auch der optisch größte Unterschied vom neuen Modell zum alten, denn dank der Platine verschwand der auffällige Kabelbaum. Nicht nur optisch verschönert die Platine das Auto, sondern sie verfügt über jeweils zwei verbaute Rasberry Pi CM4 Module und ESP32 über mehr Rechenleistung und spart gleichzeitig noch Gewicht ein.

Beim alten Modell ist der verbaute ESP32 regelmäßig abgestürzt. Es lag die Vermutung nahe, dass das Abstürzen durch zu wenig Rechenleistung verursacht wurde, sodass das Ansteuern der Lenkung und der Beleuchtung zu viel für einen ESP war. Deshalb designten wir die Platine so, dass die Platine nun Platz für zwei ESP32 bietet. Der eine ESP übernimmt die Ansteuerung der Doppelachslenkung und der zweite ESP ist dafür gedacht die nicht unbedingt notwendigen Spielereien, wie das Bremslicht anzusteuern. So konnten wir bis dato Abstürze des ESP‘s verhindern.

In unserem alten Modell verbauten wir zwei Kameras, die jeweils einen Blickwinkel von 60 Grad, so erzielten wir einen Blickwinkel von 120 Grad. Allerdings mussten wir im Vorentscheid feststellen, dass dies insbesondere beim Vorbeifahren an den Hindernissen zu wenig war. Daher war  der nächste Schritt naheliegend eine dritte Kamera zu verbauen, um einen größeren Öffnungswinkel von 180 Grad zu erhalten. Allerdings begrenzte ein Rasberry Pi uns daran, denn an einen Rasberry Pi können lediglich nur zwei Kameras angeschlossen werden. Da für uns klar war, dass wir drei Kameras verbauen wollten, mussten wir noch einen zweiten Rasberry Pi anschließen, was in dem Design der Platine ebenfalls berücksichtigt wurde.


## Autonomes Fahren und Hinderniserkennung

Als wir mit dem Aufbauprozess des Autos soweit fertig waren, konnten wir uns ab diesem Zeitpunkt an die eigentlichen Aufgaben des autonomen Fahrens und der Hinderniserkennung begeben. Die erste Idee, die wir für die Orientierung auf dem Spielfeld verfolgten, war die Verwendung von Lidar Sensoren, welche verbauten wir vorne und jeweils an den Seiten des Autos. Lidar Sensoren werden bereits für das autonome Fahren in Kraftfahrzeuge verbaut, da diese ein Abbild der Umgebung erzeugen können. Dadurch, dass die Lidar Sensoren mit Infrarotlicht arbeiten, stellten wir fest, dass dies mit der schwarzen Bande des Spielfeldes nicht kompatibel war, weshalb wir die Idee der Lidar Sensoren verwerfen mussten.
Die Alternative auf die wir uns von dort an konzentrierten, war die Verwendung der Kameras. Wir arbeiten mit zwei Kameras, da wir dadurch einen größeren Öffnungswinkel erzeugen können. Mithilfe der Kameras gelingt es uns die Banden und die Hindernisse zu erkennen, d.h. die Hauptbestandteile unseres Programms bestehen aus Bilderkennungsalgorithmen.

# Übersicht der technischen Komponenten

* 2 x Raspberry Pi CM4
* 2 x ESP 32 Mikrocontroller
* 3 x Raspberry Pi Kamera
* Gyro Sensor
* Lidar Sensor
* Positionskamera
