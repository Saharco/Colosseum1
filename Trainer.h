//
// Created by sahar on 02/12/2017.
//

#ifndef COLOSSEUM_TRAINER_H
#define COLOSSEUM_TRAINER_H

#include <iostream>
#include "Squad.h"
#include "Exceptions.h"

class Trainer {
    int id;
    Squad* squad;
public:
    Trainer(int id);
    Trainer(const Trainer& trainer){};
    ~Trainer();
    int getID() const;
    Squad* getSquad() const;
};
#endif //COLOSSEUM_TRAINER_H
