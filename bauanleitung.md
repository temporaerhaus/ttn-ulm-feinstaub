# Luftdaten.info und LoRa / TTN

Die folgende Anleitung beschreibt eine Alternative zum
ESP8266 von [luftdaten.info](http://luftdaten.info). Viele
der hier aufgeführten Informationen und Ideen stammen von
luftdaten.info, so dass man bei Fragen auch gut dort
nachlesen kann.

Die unten stehende Anleitung geht davon aus, dass du an unserem Ulmer
Feinstaub Projekt mit TTN teilnimmst, da wir die Daten an einer zentralen
Stelle aus TTN ausführen und von dort direkt gesammelt an die API von
luftdaten.info weiterleiten.

Wer dies nicht möchte oder braucht, kann natürlich die Daten in TTN z.B.
via MQTT selbst abholen und weiterverarbeiten.

## Benötigte Hardware

Neben den Sensoren hast du die Wahl zwischen zwei Arduinos.
Der The Things UNO ist etwas größer und teurer, der Adafruit
ist kleiner und billiger und allgemein unsere Empfehlung.

* SDS011 - [Bestellen](http://de.aliexpress.com/wholesale?site=deu&SortType=price_asc&shipCountry=de&SearchText=sds011&CatId=523)
* DHT22 - [Bestellen](http://de.aliexpress.com/wholesale?site=deu&SortType=price_asc&shipCountry=de&SearchText=dht22&CatId=523)
* LoRA Node (nur eine nötig)
 * The Things UNO - [Bestellen](https://shop.thethingsnetwork.com/index.php/product/the-things-uno/)
 * Adafruit Feather - [Bestellen](http://www.exp-tech.de/adafruit-feather-m0-with-rfm95-lora-radio-900mhz) (nicht wundern, 900Mhz ist korrekt)
* Micro USB Flachband Kabel, 2m - [Bestellen](https://www.amazon.de/s/?field-keywords=micro+usb+flachbandkabel+2m)
* Kabel (Dupont)
* Kabelbinder
* Schlauch, durchsichtig, Durchmesser 6 mm (Baumarkt)
* Wetterschutz, z.B. Marley Silent HT Bogen (DN 75 87°)

Andere Arduinos mit LoRa an Board funktionieren natürlich auch, nur
haben wir für diese leider keine genaue Anleitung parat. Aber, wenn du
dich mit der Materie auskennst, ist das sicher sehr einfach anzupassen.

## Benötigte Software

Den Arduino-Sketch bekommt man in unserem [Github Repository](https://github.com/verschwoerhaus/ttn-ulm-feinstaub)
 bzw. als [direkter Download](https://github.com/verschwoerhaus/ttn-ulm-feinstaub/archive/master.zip).

Kopiere (oder git clone) den kompletten Ordner in dein Arduino Sketch Verzeichnis
und öffne diesen in deiner Arduino IDE.

Damit der Sketch funktioniert, musst du noch folgende Bibliotheken in
der Arduino IDE über den Arduino Library Manager hinzufügen:

* TheThingsNetwork
* Adafruit Unified Sensor (weit nach unten scrollen)
* DHT sensor library


## Zusammenbauen

#### The Things UNO

**Feinstaubsensor SDS011**

Am Feinstaubsensor werden von links nach rechts (wenn der Sensor flach
auf dem Tisch liegt, Lüfter oben) folgende Pins verbunden:

* PIN 1: freilassen
* PIN 2: freilassen
* PIN 3: 5V am The Things Uno
* PIN 4: freilassen
* PIN 5: GND am The Things Uno (egal welcher GND, gibt 2)
* PIN 6: PIN 8 am The Things Uno
* PIN 7: PIN 9 am The Things Uno

(Wer sich auskennt dann die Belegung der PINs auch im Sketch ändern.)

**DHT22 Temperatur- und Feuchtigkeitssensor**

Am Sensor werden vorne (das Gitter) von links nach rechts folgende Pins
verbunden:

* PIN 1: 3.3V am The Things Uno
* PIN 2: PIN 10 am The Things Uno
* PIN 3: freilassen
* PIN 4: GND am The Things Uno (egal welcher GND, gibt 2)

Eventuell kann es helfen, die Kabel am Sensorrücken mit einem Stück
Gewebeband zu fixieren. Manche Kabel sitzen nicht sehr satt and den
dünnen Beinchen.

(Auch hier kann man die PIN-Belegung im Sketch anpassen, falls nötig.)

#### Adafruit Feather

Anleitung hierzu folgt bald.


#### Zusammensetzen

Wie man die Einzelteile in das Rohr einsetzt wird sehr gut direkt
bei [luftdaten.info erklärt](http://luftdaten.info/feinstaubsensor-bauen/#komponenten-zusammenbau)
erklärt. Besser könnten wir das hier auch nicht.


## Bei TTN registrieren & freischalten

Damit du Daten via TTN versenden und empfangen kannst, musst
du dich bei TTN anmelden und dann von uns in die Ulmer Feinstaub TTN App
`ttnulm-particulates` hinzugefügt werden. Danach kannst du in dieser App
ein Device erstellen, das deinen Feinstaubsensor identifiziert.

1. [Anmelden bei The Things Network](https://www.thethingsnetwork.org/).
Rechts oben auf *Sign up* klicken und registrieren.
2. Deinen The Things Network Username über [dieses Formular](TODO) an uns schicken.
3. Wir fügen dich über deinen Username zu unserer TTN Feinstaub App hinzu
und benachrichtigen dich via E-Mail (wir verwenden diese für nichts anderes,
versprochen), wenn du hinzugefügt wurdest.
4. Erstelle in deiner neuen TTN Feinstaub App `ttnulm-particulates` ein
neues Device. Die Device EUI kannst du zuerst automatisch generieren
lassen (2 Pfeilchen links), diese wird aber später nochmal ersetzt.
Achte darauf, dass du bei der Erstellung die Aktivierungsmethode OTAA
auswählst (sollte aber der Default sein).
5. Das luftdaten.info Projekt möchte zu jedem Sensor noch Details wie
Aufstellort, Umgebung, etc. wissen, damit die Daten möglichst gut
ausgewertet werden können. Wir übernehmen diese Kommunikation für dich,
fülle dazu einfach [dieses Formular](TODO) aus.
6. Du bekommst von uns nochmal eine Mail, wenn alles freigeschalten und
abgeschlossen ist.


## Arduino flashen

Bevor der Arduino Sketch auf den Arduino geflasht wird,
müssen noch die TTN Daten deines Devices (siehe Schritt
zuvor) hinzugefügt werden.

Öffne die Datei ttnulmdust.ino und ersetzte dort die zwei Zeilen

    const char *devAddr = "";
    const char *appSKey = "";

mit den zwei Zeilen, die du am unteren Ende der Device Seite bei TTN
siehst.

Danach kannst du wie gewohnt über die Arduino IDE den Flashvorgang starten.

**Wichtiger Schritt:**

Beim ersten Start wird dir auf der seriellen Konsole die *echte* Device EUI
der Node angezeigt. Kopiere diese und ersetze die vorher generierte Device EUI
unter `Settings` bei deinem Device in der TTN Console. Nur dann kann deine OTAA Aktivierung funktionieren, es muss
die Device EUI bei TTN mit der auf der seriellen Console übereinstimmen.

## Funktioniert es?

Ob Daten ankommen und damit alles geklappt hat, kannst du in der TTN Console
im Menüpunkt *Data* beim Device sehen. Dort tauchen je nach Sendeintervall deine
Daten auf. Darauf achten, dass man auch sein Device ausgewählt hat, weil
 im App-übergreifenden *Data* Bereich alle Daten aller Devices sieht.