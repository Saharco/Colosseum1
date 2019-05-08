//
// Created by sahar on 02/12/2017.
//

#include "Trainer.h"

Trainer::Trainer(int id) {
    if(id<=0) {
        throw badIdException();
    }
    this->id = id;
    squad = new Squad();
}
Trainer::~Trainer() {
    delete(squad);
}

int Trainer::getID() const {
    return id;
}
Squad* Trainer::getSquad() const {
    return squad;
}