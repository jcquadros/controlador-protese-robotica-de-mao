#ifndef MOVEMENT_CONTROLLER_H
#define MOVEMENT_CONTROLLER_H

#include "hand.h"
#include "hand-command.h"

/**
 * @class MovementController
 * @brief Gerencia o movimento da mão de forma gradual e segura.
 *
 * Esta classe controla a transição entre a posição atual e a desejada,
 * movendo os dedos em pequenos incrementos e monitorando um sensor de pressão
 * para evitar força excessiva ao fechar a mão.
 */
class MovementController {
private:
    Hand& hand;                  // Referência para o objeto da mão que será controlado.
    HandCommand targetCommand;   // A posição final desejada para os dedos.
    
    int pressureLimit;           // O valor máximo de pressão permitido.
    int (*readPressure)();       // Ponteiro de função para ler o sensor de pressão.

    unsigned long lastUpdateTime; // Última vez que a posição foi atualizada (em ms).
    int updateInterval;          // Intervalo entre cada passo do movimento (em ms).
    int stepSize;                // Tamanho do incremento/decremento a cada passo (em porcentagem).

    /**
     * @brief Move um único dedo em direção ao seu alvo.
     * @param current A posição percentual atual do dedo.
     * @param target A posição percentual alvo do dedo.
     * @param isPressureExceeded Verdadeiro se o limite de pressão foi atingido.
     * @return A nova posição percentual do dedo.
     */
    int getNextStep(int current, int target, bool isPressureExceeded);

public:
    /**
     * @brief Construtor do MovementController.
     * @param hand Referência para o objeto Hand.
     * @param readPressure Ponteiro para a função que lê o sensor de pressão.
     * @param pressureLimit O valor de leitura do sensor que não deve ser excedido.
     */
    MovementController(Hand& hand, int (*readPressure)(), int pressureLimit);

    /**
     * @brief Inicializa o controlador com o estado atual da mão.
     */
    void init();

    /**
     * @brief Define uma nova posição alvo para a mão.
     * @param newTarget O novo comando com as posições desejadas.
     */
    void setTargetPosition(const HandCommand& newTarget);

    /**
     * @brief Atualiza o estado da mão, movendo os dedos um passo.
     * Este método deve ser chamado repetidamente no loop principal do programa.
     */
    void update();
};

#endif // MOVEMENT_CONTROLLER_H
