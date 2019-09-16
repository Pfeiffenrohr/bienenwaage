/*
 * (C) 2015 Thorsten Gurzan - beelogger.de
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
*/
 
// beelogger.de - Arduino Datenlogger für Imker
// Erläuterungen dieses Programmcodes unter http://beelogger.de
 
#include <HX711.h>  
HX711 scale(A1, A0); 
 
char c;
 
long Tara;
float Scale;
 
void setup(){ 
  Serial.begin(9600); 
  delay(5000); 
 
  Serial.println(F("Zur Kalibrierung der Stockwaage bitte den Anweisungen folgen!"));
  Serial.println(); 
  Serial.println(F("Waage ohne Gewicht - Kalibrierung mit '1' und 'Enter' starten!")); 
  Serial.println(); 
  while (c!='1') {c = Serial.read();};
  c='x';
  Serial.println(F("Kalibriere ... ")); 
  Serial.println(); 
  scale.set_scale();
  scale.read_average(10);
  Tara = scale.read_average(10);
  Serial.println(F("Waage mit genau 1 Kg beschweren - Kalibrierung mit '2' und 'Enter' starten!")); 
  Serial.println(); 
  while (c!='2') {c = Serial.read();};
  Serial.println(F("Kalibriere ... ")); 
  Serial.println(); 
  scale.set_offset(Tara);
  scale.get_units(10);
  Scale = ((scale.get_units(10)) / 1000);
  scale.set_scale(Scale); 
  Serial.print(F("Pruefe Gewicht: ")); 
  Serial.println(scale.get_units(10), 1);
  Serial.println(); 
  Serial.print(F("Taragewicht: "));
  Serial.println(Tara); 
  Serial.println(); 
  Serial.print(F("Skalierung: ")); 
  Serial.println(Scale); 
  Serial.println(); 
}
 
void loop(){
 
} 
