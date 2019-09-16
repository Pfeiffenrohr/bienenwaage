/*
 * (C) 2015 Thorsten Gurzan - beelogger.de
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 * 
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
 
// beelogger.de - Arduino Datenlogger für Imker
// Erläuterungen dieses Programmcodes unter http://beelogger.de
 
#include <HX711.h> 
HX711 scale(A1, A0); 
 
byte Anzahl_Sensoren_Gewicht = 1; // Mögliche Werte: '0','1'
 
long Taragewicht = 28556;  // Hier ist der Wert aus der Kalibrierung einzutragen
float Skalierung = 27.73;  // Hier ist der Wert aus der Kalibrierung einzutragen
 
long Gewicht = 999999;                      
long LetztesGewicht = 0.82; 
 
void setup() { 
  Serial.begin(9600); 
  
  if (Anzahl_Sensoren_Gewicht == 1) {
    scale.set_offset(Taragewicht);  
    scale.set_scale(Skalierung);
  } 
}
 
 
void loop() {
  if (Anzahl_Sensoren_Gewicht == 1) {
    delay(5000);
    for(byte j=0 ;j < 3; j++) { // Anzahl der Widerholungen, wenn Abweichung zum letzten Gewicht zu hoch
      Gewicht= scale.get_units(10);
      if ((Gewicht-LetztesGewicht < 200) and (Gewicht-LetztesGewicht > -200)) j=10;  // Abweichung für Fehlererkennung
      if (j < 3) delay(3000); // Wartezeit zwischen Wiederholungen
    } 
    LetztesGewicht = Gewicht;
    Serial.print("Gewicht: ");
    Serial.print(Gewicht);
    Serial.println(" g");
  } 
}
