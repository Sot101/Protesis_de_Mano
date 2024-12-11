#include <Servo.h>
#include <SoftwareSerial.h>

// Comunicación Bluetooth
SoftwareSerial BT1(10, 11);

// Pines de los servomotores
const int pinPulgar1 = 2;
const int pinPulgar2 = 3;
const int pinIndice = 4;
const int pinMedio = 5;
const int pinAnular = 6;
const int pinMenique = 7;

// Objetos Servo
Servo servoPulgar1;
Servo servoPulgar2;
Servo servoIndice;
Servo servoMedio;
Servo servoAnular;
Servo servoMenique;

char comando;

void setup() {
    // Configurar comunicación
    BT1.begin(9600);
    Serial.begin(9600);

    // Asociar pines a los servos
    servoPulgar1.attach(pinPulgar1);
    servoPulgar2.attach(pinPulgar2);
    servoIndice.attach(pinIndice);
    servoMedio.attach(pinMedio);
    servoAnular.attach(pinAnular);
    servoMenique.attach(pinMenique);

    // Inicializar todos los servos en posición inicial
    servoPulgar1.write(0);
    servoPulgar2.write(0);
    servoIndice.write(0);
    servoMedio.write(0);
    servoAnular.write(0);
    servoMenique.write(0);
}

void loop() {
    // Leer comando del Bluetooth
    if (BT1.available()) {
        comando = BT1.read();
        Serial.println(comando);

        switch (comando) {
            // Movimiento del pulgar
            case 'P':
                servoPulgar1.write(servoPulgar1.read() == 0 ? 180 : 0);
                servoPulgar2.write(servoPulgar2.read() == 0 ? 180 : 0);
                Serial.println("Pulgar movido");
                break;

            // Movimiento del índice
            case 'I':
                servoIndice.write(servoIndice.read() == 0 ? 180 : 0);
                Serial.println("Índice movido");
                break;

            // Movimiento del medio
            case 'M':
                servoMedio.write(servoMedio.read() == 0 ? 180 : 0);
                Serial.println("Medio movido");
                break;

            // Movimiento del anular
            case 'A':
                servoAnular.write(servoAnular.read() == 0 ? 180 : 0);
                Serial.println("Anular movido");
                break;

            // Movimiento del meñique
            case 'E':
                servoMenique.write(servoMenique.read() == 0 ? 180 : 0);
                Serial.println("Meñique movido");
                break;

            // Reset de todos los servos
            case 'S':
                servoPulgar1.write(0);
                servoPulgar2.write(0);
                servoIndice.write(0);
                servoMedio.write(0);
                servoAnular.write(0);
                servoMenique.write(0);
                Serial.println("Todos los servos en posición inicial");
                break;

            default:
                Serial.println("Comando desconocido");
                break;
        }
    }
}
