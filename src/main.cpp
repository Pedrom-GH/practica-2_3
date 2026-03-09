#include <Arduino.h>

#define LED_PIN     17   // Pin del LED
#define BUTTON_UP   15  // Botón para aumentar la velocidad
#define BUTTON_DOWN 16  // Botón para disminuir la velocidad

volatile int blinkInterval = 500; // Intervalo de parpadeo en ms
volatile bool ledState = false;
volatile unsigned long lastPressUp = 0;
volatile unsigned long lastPressDown = 0;

hw_timer_t *timer = NULL;  // Timer de hardware

// Función que se ejecuta en la interrupción del Timer
void IRAM_ATTR onTimer() {
    unsigned long currentMillis = millis();

    // Antirrebote para el botón de subir frecuencia
    if (digitalRead(BUTTON_UP) == LOW && (currentMillis - lastPressUp > 200)) {
        lastPressUp = currentMillis;
        if (blinkInterval > 100) blinkInterval -= 100; // Aumenta la velocidad
    }

    // Antirrebote para el botón de bajar frecuencia
    if (digitalRead(BUTTON_DOWN) == LOW && (currentMillis - lastPressDown > 200)) {
        lastPressDown = currentMillis;
        if (blinkInterval < 2000) blinkInterval += 100; // Disminuye la velocidad
    }

    // Control de parpadeo del LED
    ledState = !ledState;
    digitalWrite(LED_PIN, ledState);
    
    timerAlarmWrite(timer, blinkInterval * 1000, true); // Actualiza el timer con el nuevo intervalo
}

void setup() {
    Serial.begin(115200);

    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_UP, INPUT_PULLUP);   // Configurar con resistencia pull-up
    pinMode(BUTTON_DOWN, INPUT_PULLUP); // Configurar con resistencia pull-up

    // Configurar Timer
    timer = timerBegin(0, 80, true); // Timer 0, prescaler 80 (1 tick = 1us)
    timerAttachInterrupt(timer, &onTimer, true);
    timerAlarmWrite(timer, blinkInterval * 1000, true); // Iniciar con 500ms
    timerAlarmEnable(timer); // Habilitar la alarma del Timer
}

void loop() {
    // No hacemos nada aquí, todo ocurre en la interrupción del timer
}
