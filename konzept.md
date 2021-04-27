## Muskel Herzfrequenz Sensor => Vibrationsmotor/OLED/CV
- Sensoren
	- Muskelspannungssensor
	- Herzfrequenzsensor

- Aktoren
	- Vibrationsmotor
	- OLED Display
	- Control Voltage (CV) (Analoge Spannung mit 0...5V) für analoge Synths

- Funktion
	- Muskelspannung wird per ADC eingelesen
	- Herzfrequenz wird per ADC eingelesen
	- Beide Werte werden auf Display ausgegeben
	- Herzfrequenz wird per Vibrationsmotor ausgegeben (montiert auf Unterarm)
	- Muskelspannung und Herzfrequenz wird in MIDI/CV Wert übersetzt und ausgegeben

- Optional/Erweiterungen
	- Mikrophon für BPM Detektion
	- Gyroskop für MIDI/CV
	- MIDI
	- 3D gedrucktes Armband

- Vision
	- Musiker/DJ koppelt Körperfunktionswerte direkt an Tempo und Parameter der Musik (Filter, Pitch, Modulation, etc)
	- Körper steuert Musik bzw. Licht = BIO-HACKING

- Fragen/Probleme
	- Synchronisation des Vibrationsmotors und des Herzfrequenzsensors
	- analoge Filter für CV Ausgang (PWM => echt analoge Spannung)
	- Software Filter für Mittelung der Pulssignale (langes Mittelwertfilter ~5...10min)
	- Bauteile von BHT besorgen? ~ 7€ Pulssensor + 50€ Muskelsensor + 20€ Kleinteile (Filter etc.) (+ 20€ 3D Druck) = 80€ (100€)


Yazan Saleh, Lukas Güldenstein