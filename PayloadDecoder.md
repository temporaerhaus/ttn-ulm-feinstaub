# Payload Decoder für die TTN Konsole

Um die übertragenen Daten in der The Things Network Konsole direkt sichtbar zu
machen und in einen lesbaren Wert zu konvertieren, kann der folgende Payload
Decoder in der Feinstaub Application in der The Things Network Konsole eingefügt
werden. Die Daten für PM10, PM2.5, Temperatur, Luftfeuchtigkeit und Luftdruck
(falls vorhanden) werden automatisch dekodiert und an die JSON Daten angehängt.

## JavaScript Code

```javascript
function Decoder(bytes, port) {
  var decoded = {};

  if (port === 1)
  {
    // decode dust sensor data
    decoded.pm10 = (bytes[0] << 8 | bytes[1]) / 100.0;
    decoded.pm25 = (bytes[2] << 8 | bytes[3]) / 100.0;
    
    // decode weather sensor data
    decoded.humidity = (bytes[4] << 8 | bytes[5]) / 100.0;
    decoded.temperature = (bytes[6] << 8 | bytes[7]) / 100.0;
    
    // if pressure was transfered, decode it, too
    if (bytes.length >= 10)
    {
      decoded.pressure = (bytes[8] << 8 | bytes[9]) / 100.0;
    }
  }

  return decoded;
}
```