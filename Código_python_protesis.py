# final V2 - bluetooth

import cv2
import mediapipe as mp
import time
import serial

# Configuración de MediaPipe
mp_hands = mp.solutions.hands
hands = mp_hands.Hands(max_num_hands=1, min_detection_confidence=0.7, min_tracking_confidence=0.7)
mp_drawing = mp.solutions.drawing_utils

# Configuración del puerto serial
try:
    serial_port = serial.Serial('COM9', 9600)  # Cambia 'COM9' al puerto correspondiente
    time.sleep(2)  # Espera para que se establezca la conexión
except serial.SerialException:
    print("No se pudo abrir el puerto serial. Verifica la conexión.")
    serial_port = None

# Captura de video desde la cámara
cap = cv2.VideoCapture(0)

# Función para calcular el porcentaje de cierre del pulgar
def posicion_exacta_pulgar(hand_landmarks, pulgar_punta, medio_nudillo, menique_nudillo):
    medio_nudillo_x = hand_landmarks.landmark[medio_nudillo].x
    pulgar_punta_x = hand_landmarks.landmark[pulgar_punta].x
    menique_nudillo_x = hand_landmarks.landmark[menique_nudillo].x
    referencia_100 = 2 * abs(medio_nudillo_x - menique_nudillo_x)

    if medio_nudillo_x >= menique_nudillo_x:
        porc_cierre = round(((-medio_nudillo_x + pulgar_punta_x) * 100 / referencia_100))
    else:
        porc_cierre = round((-(-medio_nudillo_x + pulgar_punta_x) * 100 / referencia_100))

    porc_cierre = max(0, min(100, porc_cierre))  # Limitar a rango 0-100
    return 180 - (porc_cierre * 180 // 100)  # Inversión: 100% = 0°

# Función para calcular el porcentaje de cierre de otros dedos
def posicion_exacta_dedo(hand_landmarks, dedo_punta, dedo_nudillo):
    punto_palma_y = hand_landmarks.landmark[mp_hands.HandLandmark.WRIST].y
    punto_nudillo_y = hand_landmarks.landmark[dedo_nudillo].y
    punto_punta_y = hand_landmarks.landmark[dedo_punta].y
    palma_nudillo_dist = round(0.8 * abs(punto_palma_y - punto_nudillo_y), 3)

    porc_cierre = round(((-punto_punta_y + punto_nudillo_y) * 100 / palma_nudillo_dist))
    porc_cierre = max(0, min(100, porc_cierre))  # Limitar a rango 0-100

    return porc_cierre * 180 // 100  # 100% = 180°

while cap.isOpened():
    time.sleep(0.05)
    ret, frame = cap.read()
    if not ret:
        break

    # Convierte la imagen a RGB para MediaPipe
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
    results = hands.process(frame_rgb)

    if results.multi_hand_landmarks:
        for hand_landmarks in results.multi_hand_landmarks:
            mp_drawing.draw_landmarks(frame, hand_landmarks, mp_hands.HAND_CONNECTIONS)

            # Calcular los ángulos de los dedos
            angulos_dedos = {
                "Pulgar": posicion_exacta_pulgar(hand_landmarks, mp_hands.HandLandmark.THUMB_TIP, mp_hands.HandLandmark.MIDDLE_FINGER_MCP, mp_hands.HandLandmark.PINKY_MCP),
                "Indice": 180 - posicion_exacta_dedo(hand_landmarks, mp_hands.HandLandmark.INDEX_FINGER_TIP, mp_hands.HandLandmark.INDEX_FINGER_MCP),
                "Medio": 180 - posicion_exacta_dedo(hand_landmarks, mp_hands.HandLandmark.MIDDLE_FINGER_TIP, mp_hands.HandLandmark.MIDDLE_FINGER_MCP),  # Inversión
                "Anular": 180 - posicion_exacta_dedo(hand_landmarks, mp_hands.HandLandmark.RING_FINGER_TIP, mp_hands.HandLandmark.RING_FINGER_MCP),
                "Menique": 180 - posicion_exacta_dedo(hand_landmarks, mp_hands.HandLandmark.PINKY_TIP, mp_hands.HandLandmark.PINKY_MCP),
            }

            # Formatear los datos para enviar por puerto serial
            datos_serial = f"{angulos_dedos['Pulgar']},{angulos_dedos['Indice']},{angulos_dedos['Medio']},{angulos_dedos['Anular']},{angulos_dedos['Menique']}\n"

            if serial_port and serial_port.is_open:
                try:
                    serial_port.write(datos_serial.encode())  # Enviar datos por serial
                except serial.SerialException as e:
                    print(f"Error al enviar datos por serial: {e}")

            # Mostrar en consola
            print(datos_serial.strip())

    # Mostrar ventana de la cámara
    cv2.imshow("Detección de Manos", frame)
    if cv2.waitKey(1) & 0xFF == ord("q"):
        break

cap.release()
if serial_port:
    serial_port.close()
cv2.destroyAllWindows()
