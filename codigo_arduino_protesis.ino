#include <Servo.h>
#include <SoftwareSerial.h>

// Comunicación Bluetooth
SoftwareSerial BT1(10,11);


// Declaración de los servos
Servo servoPulgar, servoPulgar2, servoIndice, servoMedio, servoAnular, servoMenique;

// Pines a los que están conectados los servos
const int pinPulgar = 3;
const int pinPulgar2 = 5;
const int pinIndice = 6;
const int pinMedio = 9;
const int pinAnular = 12;
const int pinMenique = 13;
const int fp = 1.5; 
void setup() {
  Serial.begin(9600); // Configura la comunicación serial
  BT1.begin(9600);
   


  // Conecta los servos a los pines
  servoPulgar.attach(pinPulgar);
  servoPulgar2.attach(pinPulgar2);
  servoIndice.attach(pinIndice);
  servoMedio.attach(pinMedio);
  servoAnular.attach(pinAnular);
  servoMenique.attach(pinMenique);


  // Inicializa los servos en la posición 90° (posición neutral)
  servoPulgar.write(90);
  servoPulgar2.write(90);
  servoIndice.write(90);
  servoMedio.write(90);
  servoAnular.write(90);
  servoMenique.write(90);
}

void loop() {
  // Verifica si hay datos disponibles en el puerto serial
  if (Serial.available() > 0) {
    String datos = Serial.readStringUntil('\n'); // Lee hasta un salto de línea
    datos.trim(); // Elimina espacios en blanco y saltos de línea adicionales
    
    // Divide los datos en una lista separada por comas
    int angulos[5];
    int i = 0;
    int inicio = 0;
    for (int fin = 0; fin < datos.length(); fin++) {
      if (datos[fin] == ',' || fin == datos.length() - 1) {
        String subCadena = datos.substring(inicio, fin + (fin == datos.length() - 1 ? 1 : 0));
        angulos[i++] = subCadena.toInt(); // Convierte la subcadena a entero
        inicio = fin + 1;
        
      }
    }

    // Asegúrate de que hay exactamente 5 valores antes de escribirlos
    if (i == 5) {
      // Escribe los valores a los servos
      servoPulgar.write(180); // Pulgar inverso
      servoPulgar2.write(map(angulos[0]*fp, 0, 100, 170, 0)); // Pulgar inverso
      servoIndice.write(map(angulos[1], 0, 100, 0, 180));
      servoMedio.write(map(angulos[2], 0, 100, 180, 0)); // Medio inverso
      servoAnular.write(map(angulos[3], 0, 100, 0, 180));
      servoMenique.write(map(angulos[4], 0, 100, 0, 180));
    } 
  }
}
