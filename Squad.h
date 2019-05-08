//
// Created by sahar on 02/12/2017.
//

#ifndef COLOSSEUM_SQUAD_H
#define COLOSSEUM_SQUAD_H

#include <iostream>
#include "SplayTree.h"
#include "Gladiator.h"

/*
 * The comparator for the IDs of two gladiators:
 * Returns 1 if the new gladiator's ID is bigger than a given gladiator's ID,
 * Returns -1 if the given gladiator's ID is bigger than the new gladiator's ID,
 * Returns 0 otherwise (equality)
 */
class CompareID : public Comparator<Gladiator*> {
public:
    CompareID(){};
    ~CompareID(){};
    int operator()(Gladiator* g1, Gladiator* g2) {
        int result = g1->getID() - g2->getID();
        if(result < 0) { return -1; }
        else if(result > 0) { return 1; }
        return 0;
    }
};

/*
 * The comparator for the levels & IDs of two gladiators:
 * Returns 1 if the new gladiator's level is bigger than a given gladiator's level,
 * or if they share the same level but the new gladiator's ID is smaller.
 * Returns -1 if the given gladiator's level is bigger than the new gladiator's level,
 * or if they share the sam elevel but the new gladiator's ID is bigger.
 * Returns 0 otherwise (equality)
 */
class CompareLevel : public Comparator<Gladiator*> {
public:
    CompareLevel(){};
    ~CompareLevel(){};
    int operator()(Gladiator* g1, Gladiator* g2) {
        int result1 = g1->getLevel() - g2->getLevel();
        if(result1 < 0) { return -1; }
        else if(result1 > 0) { return 1; }
        else {
            int result2 = g1->getID() - g2->getID();
            if(result2 < 0) { return 1; }
            else if(result2 > 0) { return -1; }
        }
        return 0;
    }
};

class Squad {
    SplayTree<Gladiator*>* id_tree; //The tree that stores gladiators, where the keys are the IDs
    CompareID* compare_id_tree; //Comparator for the ID tree: the keys are the IDs.
    //The tree that stores (the same) gladiators, where the keys are a combination of level and ID (main & secondary sort)
    SplayTree<Gladiator*>* level_tree;
    CompareLevel* compare_level_tree; //Comparator for the level tree: the keys are both the levels and the IDs.
    int num_gladiators; //The amount of gladiators in the squad (which is the amount of gladiators in one of the trees)
    Gladiator* best_gladiator; //Pointer to the best gladiator in the squad (highest level, and if several exist: lowest ID between those)
public:
    Squad();
    // Squad(const Squad& squad) is the default by-value copy c'tor
    ~Squad();
    int getSize() const;
    int getBest() const;
    Gladiator* findGladiator(int id);
    void addGladiator(Gladiator* g);
    Gladiator* addGladiator(int gladiator_id, int level, Trainer* trainer);
    void removeGladiator(Gladiator* g);
    void scanAllByID(Procedure<Gladiator*>& operation);
    void scanAllByLevel(Procedure<Gladiator*>& operation);
    void destroyLevelTree();
    void concatGladiator(Gladiator* g);
    void updateMax();
    void removeAll();
};


#endif //COLOSSEUM_SQUAD_H