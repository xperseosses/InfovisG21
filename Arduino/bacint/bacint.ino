/*
  Proyecto: Torrente Biónico - Control Sincronizado
  ENTRADAS: 
  - '0', '1', '2': Fija el Estado Físico y el LED (Semaforo)
  - 'P': Comando de Pulso (Activa la bomba y el LED Rojo por un instante)
*/

// --- PINES ---
const int PIN_BOMBA = 7;        
const int PIN_LED_VERDE = 5;    
const int PIN_LED_AMARILLO = 4; 
const int PIN_LED_ROJO = 6;     

// --- VARIABLES DE ESTADO ---
int estadoActual = -1; // -1: STANDBY. 0: Calma. 1: Agitado. 2: Crítico.

void setup() {
  Serial.begin(9600);
  pinMode(PIN_BOMBA, OUTPUT);
  pinMode(PIN_LED_VERDE, OUTPUT);
  pinMode(PIN_LED_AMARILLO, OUTPUT);
  pinMode(PIN_LED_ROJO, OUTPUT);
  apagarTodosLosActuadores(); 
}

void loop() {
  if (Serial.available() > 0) {
    char lectura = Serial.read();

    // COMANDO DE ESTADO: Fija las luces y el estado.
    if (lectura >= '0' && lectura <= '2') {
      estadoActual = lectura - '0';
      fijarLuzDeEstado(estadoActual);
    }
    
    // COMANDO DE PULSO: Ejecuta el pulso FÍSICO sincronizado con el sonido.
    else if (lectura == 'P') { 
      ejecutarPulsoSincronizado();
    }
  }

  // Lógica de Crítico: Parpadeo de alarma constante (no sincronizado)
  if (estadoActual == 2) {
      if (millis() % 500 < 200) { 
        digitalWrite(PIN_LED_ROJO, HIGH);
      } else {
        digitalWrite(PIN_LED_ROJO, LOW);
      }
  }
}

// --- FUNCIONES DE ACCIÓN ---

void fijarLuzDeEstado(int estado) {
  // Apaga todos los LEDs antes de encender el correcto
  apagarTodosLosActuadores();

  if (estado == 0) { // CALMA
    digitalWrite(PIN_LED_VERDE, HIGH);
  } else if (estado == 1) { // AGITADO
    digitalWrite(PIN_LED_AMARILLO, HIGH);
  } else if (estado == 2) { // CRÍTICO
    // El LED Rojo será controlado por el loop para el parpadeo de ALARMA (asíncrono)
    digitalWrite(PIN_LED_ROJO, LOW); // Lo inicializa en LOW
  }
}

void ejecutarPulsoSincronizado() {
  // Disparamos la bomba (Latido físico)
  digitalWrite(PIN_BOMBA, HIGH);
  
  // OPCIONAL: Si quieres que el LED del estado actual "parpadee" con el latido,
  // podrías apagarlo y prenderlo, pero para simplificar y arreglar tu problema:
  // NO encendemos el LED Rojo aquí forzosamente.
  
  // Solo si estamos en estado CRÍTICO (2), reforzamos el rojo
  if (estadoActual == 2) {
     digitalWrite(PIN_LED_ROJO, HIGH);
  }
  
  delay(150); // Duración del pulso (Sístole)

  // Apagar bomba
  digitalWrite(PIN_BOMBA, LOW);
  
  // Si encendimos el rojo por el pulso en estado 2, lo apagamos
  if (estadoActual == 2) {
     digitalWrite(PIN_LED_ROJO, LOW);
  }
}

void apagarTodosLosActuadores() {
  digitalWrite(PIN_BOMBA, LOW);
  digitalWrite(PIN_LED_VERDE, LOW);
  digitalWrite(PIN_LED_AMARILLO, LOW);
  digitalWrite(PIN_LED_ROJO, LOW);
}