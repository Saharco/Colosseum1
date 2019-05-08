//
// Created by sahar on 02/12/2017.
//
#include "Gladiator.h"
#include "Trainer.h"

Gladiator::Gladiator(int id, int level, Trainer *trainer) {
    if (id <= 0) {
        throw badIdException();
    }
    if (level <= 0) {
        throw badLevelException();
    }
    this->id = id;
    this->level = level;
    this->trainer = trainer;
}

int Gladiator::getID() const {
    return  id;
}

int Gladiator::getLevel() const {
    return level;
}

Trainer *Gladiator::getTrainer() const {
    return trainer;
}

void Gladiator::setID(int new_id) {
    if (new_id <= 0) {
        throw badIdException();
    }
    id = new_id;
}

void Gladiator::increaseLevel(const int increase) {
    if (increase <= 0) {
        throw badLevelException();
    }
    level += increase;
}