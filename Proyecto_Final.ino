/*
Fundación Kinal
Centro Educativo Técnico Laboral Kinal 
Taller de Electrónica 5to 
Byron Renato Dávila Santiago 
2019618
Proyecto Final 
*/


#include <SoftwareSerial.h>

const int Pin_Analogico = A0;
const float ResConocida = 1000.0;
float sensibilidad = 0.185; 
SoftwareSerial Bluetooth_Davila(7, 6);

unsigned long TimePrevio = 0;
const unsigned long IntervaloMed = 1000;

void setup() {
  Serial.begin(9600);
  Bluetooth_Davila.begin(9600);
}

char comando_recibido;

void loop() {
  if (Bluetooth_Davila.available()) {
    comando_recibido = Bluetooth_Davila.read();
    Serial.println(comando_recibido);

    if (comando_recibido == 'A') {
      Medir_Volt();
    } else if (comando_recibido == 'B') {
      Medir_resis();
    } else if (comando_recibido == 'C') {
      Medir_corriente();
    }
  }

  unsigned long tiempoActual = millis();
  if (tiempoActual - TimePrevio >= IntervaloMed) {
    TimePrevio = tiempoActual;
  }
}

void Medir_Volt() {
  int Valor_Sensor = analogRead(A1);
  float Valor_Volt = map(Valor_Sensor, 0, 1023, 0, 250) / 10.0;
  Serial.print("Voltaje: ");
  Serial.println(Valor_Volt);
  Bluetooth_Davila.print(Valor_Volt);
  Bluetooth_Davila.print(";");
}

void Medir_resis() {
  int valorAnalogico = analogRead(Pin_Analogico);
  float tensionMedidal = (float)valorAnalogico * (5.0 / 1023.0);
  float Corriente = tensionMedidal / ResConocida;
  float Res_desconocida = tensionMedidal / Corriente;
  Serial.print("Resistencia: ");
  Serial.println(Res_desconocida);
  Bluetooth_Davila.print(Res_desconocida);
  Bluetooth_Davila.print(";");
}

void Medir_corriente() {
  float Corriente = calcCorriente(500);
  Serial.print("Corriente: ");
  Serial.println(Corriente, 3);
  Bluetooth_Davila.print(Corriente, 3);
  Bluetooth_Davila.print(";");
}

float calcCorriente(int Numero_Muestra) {
  float leersensor = 0;
  float Corriente = 0;

  for (int i = 0; i < Numero_Muestra; i++) {
    leersensor = analogRead(A2) * (5.0 / 1023.0);
    Corriente = Corriente + (leersensor - 2.5);
  }

  Corriente = Corriente / (Numero_Muestra * sensibilidad);
  return (Corriente);
}
