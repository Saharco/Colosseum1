//
// Created by sahar on 03/12/2017.
//

#ifndef COLOSSEUM_COLOSSEUM_H
#define COLOSSEUM_COLOSSEUM_H

#include "SplayTree.h"
#include "Squad.h"
#include "Trainer.h"
#include "Exceptions.h"
#include "library1.h"

class compareTrainerID : public Comparator<Trainer*> {
public:
    int operator()(Trainer* t1, Trainer* g2) {
        int result = t1->getID() - g2->getID();
        if(result < 0) { return 1; }
        else if(result > 0) { return -1; }
        return 0;
    }
};

class Colosseum {
    SplayTree<Trainer*>* trainers;
    compareTrainerID* compare_trainers_tree;
    Squad* main_squad;
public:
    Colosseum();
    ~Colosseum();
    StatusType AddTrainer(int trainerID);
    StatusType BuyGladiator(int gladiatorID,int trainerID,int level);
    StatusType FreeGladiator(int gladiatorID);
    StatusType LevelUp(int gladiatorID, int levelIncrease);
    StatusType GetTopGladiator(int trainerID, int * gladiatorID);
    StatusType GetAllGladiatorsByLevel(int trainerID, int **gladiators, int *numOfGladiator);
    StatusType UpgradeGladiator(int gladiatorID, int upgradedID);
    StatusType UpdateLevels(int stimulantCode, int stimulantFactor);
};


#endif //COLOSSEUM_COLOSSEUM_H