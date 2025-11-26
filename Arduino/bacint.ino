/*
  Proyecto: Torrente Sanguíneo Interactivo
  Descripción: Controla una bomba de agua y LEDs basado en señales Web Serial.
  Entradas: Serial ('0', '1', '2')
  Salidas: Relé (Bomba), LEDs (Verde, Amarillo, Rojo)
*/

const int PIN_BOMBA = 7;      // Relé conectado a este pin
const int PIN_LED_VERDE = 5;  // LED Estado Calma
const int PIN_LED_AMARILLO = 4; // LED Estado Agitado (Opcional, si tienes)
const int PIN_LED_ROJO = 6;   // LED Estado Crítico (PWM para efectos)

int estadoActual = 0; 

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BOMBA, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_AMARILLO, OUTPUT);
  pinMode(PIN_LED_ROJO, OUTPUT);
  
  // Test inicial
  digitalWrite(PIN_LED_VERDE, HIGH); delay(200); digitalWrite(PIN_LED_VERDE, LOW);
  digitalWrite(PIN_LED_ROJO, HIGH); delay(200); digitalWrite(PIN_LED_ROJO, LOW);
}

void loop() {
  if (Serial.available() > 0) {
    char lectura = Serial.read();
    if (lectura >= '0' && lectura <= '2') {
      estadoActual = lectura - '0';
    }
  }

  switch (estadoActual) {
    case 0: // CALMA (0% - 0.03%)
      digitalWrite(PIN_LED_VERDE, HIGH);
      digitalWrite(PIN_LED_AMARILLO, LOW);
      digitalWrite(PIN_LED_ROJO, LOW);
      // Bomba suave/constante
      digitalWrite(PIN_BOMBA, HIGH); 
      delay(2000); 
      digitalWrite(PIN_BOMBA, LOW); 
      delay(100);
      break;

    case 1: // AGITADO (0.06% - 0.12%)
      digitalWrite(PIN_LED_VERDE, LOW);
      digitalWrite(PIN_LED_AMARILLO, HIGH); // Si no tienes amarillo, usa Rojo fijo
      digitalWrite(PIN_LED_ROJO, LOW);
      // Taquicardia
      digitalWrite(PIN_BOMBA, HIGH); delay(150);
      digitalWrite(PIN_BOMBA, LOW); delay(250);
      break;

    case 2: // CRÍTICO (0.26%+)
      digitalWrite(PIN_LED_VERDE, LOW);
      digitalWrite(PIN_LED_AMARILLO, LOW);
      // Luz roja parpadeante (Alarma)
      digitalWrite(PIN_LED_ROJO, HIGH); 
      
      // Fallo sistémico (Arrítmico)
      if (random(0, 10) < 3) { // A veces bombea
        digitalWrite(PIN_BOMBA, HIGH); delay(300);
        digitalWrite(PIN_BOMBA, LOW);
      }
      digitalWrite(PIN_LED_ROJO, LOW); // Apaga luz
      delay(random(500, 1500)); // Silencio
      break;
  }
}