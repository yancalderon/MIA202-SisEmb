//----- MIA-202 Seccion A. Grupo 3.
//   Integrantes:
// - Yan Franco Calderón Félix
// - William Hanco
// - Omar Parihuana
// - Julio Talaverano
// - Hilber Alexander Cieza Delgado
//
// ---- Implementación de un agente basado en sistemas embebidos
// ---- para monitoreo de variables críticas en un gabinete de servidores
// ---- Diciembre 2024
//

// Inicializacion de librerias necesarias para el uso de los sensores DHT y Display LCD
// basado en I2C.

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <DHT.h>
#include <DHT_U.h>



int SENSOR_TEMP = 2;  // PIN para Sensor de Temperatura
int LED_TEMP = 3;     // PIN para LED que indica Temperatura fuera de rango.
int LED_HUM = 4;      // PIN para LED que indica Humedad fuera de rango.
int LED_OPEN = 5;     // PIN para LED que indica Puerta Abierta.

int TRIG = 10;       // PIN para Sensor ultrasonico (TRIGGER)
int ECO = 9;         // PIN para Sensor ultrasonico (ECHO)
int DURACION;        // Variable para medir duracion del retorno de la onda ultrasonica
int DISTANCIA;       // Variable para convertir la duracion a distancia.



float HUMEDAD = 0;     // Variable para almacenar el % de humedad relativa.
float TEMPERATURA = 0; // Variable para almacenar la temperatura del ambiente.

DHT dht(SENSOR_TEMP, DHT11);          //Inicializacion del Sensor DHT
LiquidCrystal_I2C lcd(0x27, 16, 2);   // Inicializacion del LCD_I2C 



void setup() {
  // Establecer modo de operacion de los PIN
  pinMode(LED_TEMP, OUTPUT);
  pinMode(LED_HUM, OUTPUT);
  pinMode(LED_OPEN, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECO, INPUT);
  // Inicio del Display
  lcd.init();
  lcd.backlight();
  // Inicio del sensor DHT11.
  dht.begin();
  Serial.begin(9600); //Inicio de puerto serial


}

void loop() {
  
  HUMEDAD = dht.readHumidity();            // Leer el valor de Humedad
  TEMPERATURA = dht.readTemperature();     // Leer el valor de Temperatura
  
  lcd.clear();                             // Borrar la pantalla del LCD.
  lcd.setCursor(0,0);                      // Colocar el cursor primera fila primera columna.
  lcd.print("Temp:");                      // Mostrar: Temp.
  lcd.print(TEMPERATURA);                  // Mostrar el valor de la temperatura leida.
  lcd.write(0XDF);                         // Caracter especial   
  lcd.print("C");                      
  lcd.setCursor(0,1);                      // Colocar el cursor segunda fila primera columna.
  lcd.print("Humedad:");                   // Mostrar Humedad
  lcd.print(HUMEDAD);                      // Mostrar el valor de la humedad leida.  
  lcd.print("%RH");  
  Serial.print(TEMPERATURA);
  Serial.print(",");
  Serial.print(HUMEDAD);                         
  delay(1000);                                   
  
  if (TEMPERATURA < 18 || TEMPERATURA > 27) {               // Si el valor de Temperatura NO esta entre 18 y 27 grados. 
    digitalWrite(LED_TEMP, HIGH);                           // Encender el LED en el PIN 3 por 1 segundo.
  } else{                                                  // Si el valor de temperatura esta entre 18 y 27 grados apagar LED. 
    digitalWrite(LED_TEMP, LOW);
  }
                              
  if (HUMEDAD < 45 || HUMEDAD > 55){                      // Si humedad NO esta entre 45 y 55% de humedad relativa.  
    digitalWrite(LED_HUM, HIGH);                          // Encender LED en el PIN 4 por 1 segundo.
  } else{
    digitalWrite(LED_HUM, LOW);                         // Si el valor de humedad esta entre 45 y 55% apagar LED.
  }
                              

  digitalWrite(TRIG, HIGH);                              // Generar pulso ultrasonico.     
  delay(1);
  digitalWrite(TRIG, LOW);                                       
  DURACION = pulseIn(ECO, HIGH);                         // Calcular la duracion del pulso enviado.  
  DISTANCIA = DURACION / 58.2;                           // Calcular la distancia en funcion de la duracion del pulso. 
  
  if (DISTANCIA >= 10) {                                 // Si la distancia es mayor o igual a 10 cm.
    // Borrar la pantalla indicar que la puerta esta abierta en el LCD y prender el LED 5. 
    lcd.clear();                                         
    lcd.setCursor(0,0);
    lcd.print("Puerta Abierta:");
    lcd.setCursor(0,1);
    lcd.print("--CIERRELA--");
    digitalWrite(LED_OPEN, HIGH);
    Serial.println(",1");
  } else {
    digitalWrite(LED_OPEN, LOW);   //Apagar LED si la distancia es menor a 10cm.
    Serial.println(",0");
  } 
}
