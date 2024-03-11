#include "hw_mux.h"
#include "tool_logger.h"

extern bool debug_print;

static void MUX_WR_ENABLE(const uint8_t *pMUX_NUM) {
    uint8_t MUX_NUM = *pMUX_NUM;

    switch (MUX_NUM) {
        case 0:
            digitalWrite(MUX_WR_1, LOW);
            break;
        case 1:
            digitalWrite(MUX_WR_2, LOW);
            break;
        case 2:
            digitalWrite(MUX_WR_3, LOW);
            break;
        case 3:
            digitalWrite(MUX_WR_4, LOW);
            break;
        default:
            break;
    }
}

static void MUX_WR_DISABLE(const uint8_t *pMUX_NUM) {
    uint8_t MUX_NUM = *pMUX_NUM;

    switch (MUX_NUM) {
        case 0:
            digitalWrite(MUX_WR_1, HIGH);
            break;
        case 1:
            digitalWrite(MUX_WR_2, HIGH);
            break;
        case 2:
            digitalWrite(MUX_WR_3, HIGH);
            break;
        case 3:
            digitalWrite(MUX_WR_4, HIGH);
            break;
        default:
            break;
    }
}

static bool setDigitalPinState(const uint8_t *ARD_PIN_NUM) {
    uint8_t temp = *ARD_PIN_NUM;

    /* 如果输入的数字不在1-32之间则返回false */
    if (temp < 1 || temp > 32) return false;

    temp = temp - 1; // 00000对应的是s1
    digitalWrite(D0_pin, bitRead(temp, 0) ? HIGH : LOW);
    digitalWrite(D1_pin, bitRead(temp, 1) ? HIGH : LOW);
    digitalWrite(D2_pin, bitRead(temp, 2) ? HIGH : LOW);
    digitalWrite(D3_pin, bitRead(temp, 3) ? HIGH : LOW);
    digitalWrite(D4_pin, bitRead(temp, 4) ? HIGH : LOW);

    // if (debug_print) {
    //     for (uint8_t i = 0; i < 5; i++) {
    //         logger.logf("%d ", bitRead(temp, i));
    //     }
    // }

    return true;
}

void setupMux() {
    pinMode(D0_pin, OUTPUT);
    pinMode(D1_pin, OUTPUT);
    pinMode(D2_pin, OUTPUT);
    pinMode(D3_pin, OUTPUT);
    pinMode(D4_pin, OUTPUT);

    pinMode(MUX_WR_1, OUTPUT);
    pinMode(MUX_WR_2, OUTPUT);
    pinMode(MUX_WR_3, OUTPUT);
    pinMode(MUX_WR_4, OUTPUT);

    digitalWrite(MUX_WR_1, HIGH);
    digitalWrite(MUX_WR_2, HIGH);
    digitalWrite(MUX_WR_3, HIGH);
    digitalWrite(MUX_WR_4, HIGH);
}

void handleMux(const uint8_t *config) {
    for (uint8_t i = 0; i < 4; i++) {
        uint8_t ADG_PIN_NUMBER = 0;

        if (0 < config[i] && config[i] <= 16) {
            ADG_PIN_NUMBER = 16 + config[i];
        } else if (16 < config[i] && config[i] <= 32) {
            ADG_PIN_NUMBER = 33 - config[i];
        }

        MUX_WR_ENABLE(&i);
        setDigitalPinState(&ADG_PIN_NUMBER);
        MUX_WR_DISABLE(&i);
    }
}
