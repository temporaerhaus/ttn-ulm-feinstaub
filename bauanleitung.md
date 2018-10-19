# luftdaten.info mit LoRa / TTN

Die folgende Anleitung beschreibt eine Variante zum Bau des Feinstaubsensors
von bzw. für [luftdaten.info](http://luftdaten.info).
Statt die Messdaten mit einem ESP8266 per WLAN zu übertragen kommt LoRa(WAN) und
das [The Things Network (TTN)](https://www.thethingsnetwork.org) zum Einsatz.
Viele der hier aufgeführten Informationen und Ideen stammen von
luftdaten.info, so dass man bei Fragen auch gut [dort nachlesen kann](http://luftdaten.info/feinstaubsensor-bauen/).

Wir gehen hier davon aus, dass du an unserem Ulmer
Feinstaub Projekt mit [TTN](https://www.thethingsnetwork.org/community/ulm/)
teilnimmst, denn da rufen wir die Messdaten an einer zentralen
Stelle aus TTN ab und leiten sie gesammelt direkt an die API von
luftdaten.info weiter.

Wer dies nicht möchte oder braucht, kann natürlich die Daten in TTN z.B.
via MQTT selbst abholen und weiterverarbeiten.

**Kontakt und Rückfragen** am einfachsten im (verschwoerhaus.slack.com)
im Channel #feinstaub.


## Benötigte Hardware

Neben den Sensoren hast du die Wahl zwischen zwei Arduinos.
Der The Things UNO ist etwas größer und teurer; der Adafruit
ist kleiner und billiger und allgemein unsere Empfehlung.

* SDS011 - [Bestellen](http://de.aliexpress.com/wholesale?site=deu&SortType=price_asc&shipCountry=de&SearchText=sds011&CatId=523)
* DHT22 - [Bestellen](http://de.aliexpress.com/wholesale?site=deu&SortType=price_asc&shipCountry=de&SearchText=dht22&CatId=523)
* BME280 (statt DHT22) - [Bestellen](de.aliexpress.com/wholesale?site=deu&SortType=price_asc&shipCountry=de&SearchText=bme280&CatId=523)
* LoRA Node (nur eine nötig)
 * The Things UNO - [Bestellen](https://shop.thethingsnetwork.com/index.php/product/the-things-uno/)
 * Adafruit Feather - [Bestellen](http://www.exp-tech.de/adafruit-feather-m0-with-rfm95-lora-radio-900mhz) (nicht wundern, 900Mhz ist korrekt)
* Micro USB Flachband Kabel, 2m - [Bestellen](https://www.amazon.de/s/?field-keywords=micro+usb+flachbandkabel+2m)
* Kabel (Dupont)
* Kabelbinder
* flexibler Schlauch Innendurchmesser 6 mm (Baumarkt)
* Wetterschutz, z.B. Marley Silent HT Bogen (DN 75 87°, Baumarkt)

Andere Arduinos mit LoRa an Board funktionieren natürlich auch, nur
haben wir für diese soweit keine genaue Anleitung parat. Aber, wenn du
dich mit der Materie auskennst, ist eine Anpassung sicher sehr einfach.


## Benötigte Software

Den Arduino-Sketch bekommt man in unserem [Github Repository](https://github.com/verschwoerhaus/ttn-ulm-feinstaub)
 ([direkter Download](https://github.com/verschwoerhaus/ttn-ulm-feinstaub/archive/master.zip)).

Kopiere (oder `git clone`) den kompletten Ordner in dein Arduino-Sketch-Verzeichnis
und öffne diesen in deiner Arduino IDE.

Damit der Sketch funktioniert, musst du noch folgende Bibliotheken in
der Arduino IDE über den Arduino Library Manager hinzufügen:

* TheThingsNetwork
* Adafruit Unified Sensor (weit nach unten scrollen)
* DHT sensor library
* Adafruit BME280 library


## Zusammenbauen

### The Things UNO

#### Feinstaubsensor SDS011

Am Feinstaubsensor werden von links nach rechts (wenn der Sensor flach
auf dem Tisch liegt, Lüfter oben) folgende Pins verbunden:

* PIN 1: freilassen
* PIN 2: freilassen
* PIN 3: 5V am The Things Uno
* PIN 4: freilassen
* PIN 5: GND am The Things Uno (egal welcher GND, es gibt 2)
* PIN 6: PIN 8 am The Things Uno
* PIN 7: PIN 9 am The Things Uno

(Wer sich auskennt dann die Belegung der PINs auch im Sketch ändern.)

#### Temperatur- und Feuchtigkeitssensor

Die Software unterstützt mit dem DHT22 und dem BME280 zwei unterschiedliche
Temperatur- und Luftfeuchtigkeitssensoren. In der Software ist standardmäßig
der DHT22 aktiviert. Durch einkommentieren von "#define BME280" unterstützt
die Software den BME280. Der Anschluß der beiden Sensoren wird im folgenden
beschrieben.

##### DHT22 Temperatur- und Feuchtigkeitssensor

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

##### BME280 (statt DHT22) Temperatur- und Feuchtigkeitssensor

Am BME280 sind die Pins bereits mit ihren I2C Namen markiert. Sie werden
eins zu eins mit den Pins am The Things Uno verbunden:

* VIN: 3.3V am The Things Uno
* GND: GND am The Things Uno (egal welcher GND)
* SCL: SCL am The Things Uno
* SDA: SDA am The Things Uno



### Adafruit Feather

Anleitung hierzu folgt bald.


### Zusammenstecken

Wie man die Einzelteile in das Rohr einsetzt wird sehr gut direkt
bei [luftdaten.info erklärt](http://luftdaten.info/feinstaubsensor-bauen/#komponenten-zusammenbau)
erklärt. Besser könnten wir das hier auch nicht.


## Bei TTN registrieren & freischalten

Damit du Daten via TTN versenden und empfangen kannst, musst
du dich bei TTN anmelden.
Wie oben geschrieben kommen die Messdaten am einfachsten über die Ulmer
Feinstaub TTN-App `ttnulm-particulates` zu luftdaten.info; zu dieser App
fügen wir dich natürlich gerne hinzu. Danach kannst du in dieser App
ein Device erstellen, um deinen Feinstaubsensor am Netzwerk anzumelden
und loszulegen.

### Schritt für Schritt:

1. [Anmelden bei The Things Network](https://www.thethingsnetwork.org/).
Rechts oben auf *Sign up* klicken und registrieren.
2. Deinen The Things Network Username über [dieses Formular](TODO) oder
 kurz Nachricht im Verschwörhausslack (siehe weiter oben) an uns schicken.
3. Wir fügen dich über deinen Username zu unserer TTN Feinstaub App hinzu
und benachrichtigen dich via E-Mail, sobald du hinzugefügt wurdest (wir
verwenden deine Mailadresse für nichts anderes, versprochen).
4. Erstelle in deiner neuen TTN Feinstaub App `ttnulm-particulates` ein
neues Device. Die Device EUI kannst du zuerst automatisch generieren
lassen (2 Pfeilchen links), diese wird aber später nochmal ersetzt.
Achte darauf, dass du bei der Erstellung die Aktivierungsmethode OTAA
auswählst (sollte aber der Default sein).
5. Das luftdaten.info Projekt möchte zu jedem Sensor noch Details wie
Aufstellungsort, Umgebung, etc. wissen, damit die Daten möglichst gut
ausgewertet werden können. Wir übernehmen diese Kommunikation für dich,
fülle dazu einfach [dieses Formular](TODO) aus.
6. Du bekommst von uns nochmal eine Mail, wenn alles freigeschalten und
abgeschlossen ist.


## Arduino flashen

Bevor der Arduino-Sketch auf den Arduino geflasht wird,
müssen noch die TTN Daten deines Devices (siehe vorigen Schritt)
hinzugefügt werden.

Öffne die Datei ttnulmdust.ino und ersetzte dort die zwei Zeilen
```
    const char *devAddr = "";
    const char *appSKey = "";
```
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
Daten auf. Darauf achten, dass man auch dein Device ausgewählt hat, weil
 im App-übergreifenden *Data*-Bereich alle Daten aller Devices sieht.

*Es funktioniert irgendwie nicht?* Hilfe zum Debugging [können wir hoffentlich bald anbieten](TODO).
