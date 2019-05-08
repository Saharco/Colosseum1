#include"library1.h"
#include"Colosseum.h"

/*
 * A wrapper for the Colosseum class. Returns status codes (as previously defined).
 * For every function, a corresponding method in the Colosseum class is called.
 */

void* Init() {
    Colosseum * DS = new Colosseum();
    return (void*)DS;
}
StatusType AddTrainer(void *DS, int trainerID){
    return ((Colosseum*)DS)-> AddTrainer (trainerID);
}
StatusType BuyGladiator(void *DS, int gladiatorID, int trainerID, int level){
    return ((Colosseum*)DS)->BuyGladiator(gladiatorID,trainerID,level);
}
StatusType FreeGladiator(void *DS, int gladiatorID){
    return ((Colosseum*)DS)->FreeGladiator(gladiatorID);
}
StatusType LevelUp(void *DS, int gladiatorID, int levelIncrease){
    return ((Colosseum*)DS)->LevelUp(gladiatorID,levelIncrease);
}
StatusType GetTopGladiator(void *DS, int trainerID, int * gladiatorID){
    return ((Colosseum*)DS)->GetTopGladiator(trainerID,gladiatorID);
}
StatusType GetAllGladiatorsByLevel(void *DS, int trainerID, int **gladiators, int *numOfGladiator){
    return ((Colosseum*)DS)->GetAllGladiatorsByLevel(trainerID,gladiators,numOfGladiator);
}
StatusType UpgradeGladiator(void *DS, int gladiatorID, int upgradedID){
    return ((Colosseum*)DS)->UpgradeGladiator(gladiatorID,upgradedID);
}
StatusType UpdateLevels(void *DS, int stimulantCode, int stimulantFactor){
    return ((Colosseum*)DS)->UpdateLevels(stimulantCode,stimulantFactor);
}
void Quit(void **DS){
    delete((Colosseum*)*DS);
    *DS=NULL;
}