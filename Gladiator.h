//
// Created by sahar on 02/12/2017.
//
#ifndef COLOSSEUM_GLADIATOR_H
#define COLOSSEUM_GLADIATOR_H

#include <iostream>
#include "Exceptions.h"

class Trainer; //Forward declaration, for the gladiator class to know about this class's existence

class Gladiator {
    int id; //The ID of the gladiator (unique, positive number)
    int level; //The level of the gladiator (positive number)
    Trainer* trainer; //Pointer to the trainer who owns the gladiator.
public:
    Gladiator(int id, int level, Trainer* trainer);
    Gladiator(const Gladiator& gladiator) {};
    ~Gladiator() {};
    int getID() const;
    int getLevel() const;
    Trainer* getTrainer() const;
    void setID(int new_id);
    void increaseLevel(const int increase);
};

#endif //COLOSSEUM_GLADIATOR_H
