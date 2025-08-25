#include "movement-controller.h"
#include "hand-command.h"

MovementController::MovementController(Hand& hand, int (*readPressure)(), int pressureLimit)
    : hand(hand), 
      readPressure(readPressure), 
      pressureLimit(pressureLimit),
      lastUpdateTime(0),
      updateInterval(20), // Move a cada 20ms para um movimento suave
      stepSize(5)          // Move 5% a cada passo
{}

void MovementController::init() {
    targetCommand = HandCommand();
    hand.control_hand(targetCommand);

    Serial.println("Movement controller initialized.");
}

void MovementController::setTargetPosition(const HandCommand& newTarget) {
    this->targetCommand = newTarget;
}

int MovementController::getNextStep(int current, int target, bool isPressureExceeded) {
    // Se o dedo já está no alvo, não faz nada.
    if (current == target) {
        return current;
    }

    // Verifica a direção do movimento
    bool isClosing = target > current; // TODO: verificar porque tem dedos que sao invertidos

    if (isClosing) {
        // Se está fechando E a pressão foi excedida, bloqueia o movimento.
        if (isPressureExceeded) {
            return current; // Retorna a posição atual, impedindo o fechamento.
        }
        // Caso contrário, avança um passo em direção ao alvo.
        return min(current + stepSize, target);
    } else { // isOpening
        // Se está abrindo, o movimento é sempre permitido.
        current = stepSize;
        return max(current - stepSize, target);
    }
}

void MovementController::update() {
    // Controla a velocidade do movimento, executando apenas a cada 'updateInterval' ms.
    if (millis() - lastUpdateTime < updateInterval) {
        return;
    }
    lastUpdateTime = millis();

    HandCommand currentCommand = hand.currentCommand;
    
    // Se a mão já atingiu o alvo, não há nada a fazer.
    if (currentCommand.equals(targetCommand)) {
        return;
    }

    // Lê a pressão atual
    int pressure = readPressure();
    bool isPressureExceeded = pressure > pressureLimit;

    if (isPressureExceeded) {
        Serial.print("Pressure limit exceeded! Reading: ");
        Serial.println(pressure);
    }else{
        Serial.print("Pressure Read:");
        Serial.println(pressure);
    }

    // Calcula a próxima posição para cada dedo
    HandCommand nextCommand;
    nextCommand.thumb  = getNextStep(currentCommand.thumb,  targetCommand.thumb,  isPressureExceeded);
    nextCommand.index  = getNextStep(currentCommand.index,  targetCommand.index,  isPressureExceeded);
    nextCommand.middle = getNextStep(currentCommand.middle, targetCommand.middle, isPressureExceeded);
    nextCommand.ring   = getNextStep(currentCommand.ring,   targetCommand.ring,   isPressureExceeded);
    nextCommand.pinky  = getNextStep(currentCommand.pinky,  targetCommand.pinky,  isPressureExceeded);

    // Se a próxima posição for diferente da atual, envia o comando para a mão
    if (!currentCommand.equals(nextCommand)) {
        hand.control_hand(nextCommand);
    }
}
