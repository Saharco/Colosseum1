//
// Created by sahar on 02/12/2017.
//

#include <cstdlib>
#include "Squad.h"

/*
 * The squad's c'tor. Saves new instances of both of the comparator types (for IDs and levels),
 * and allocates an ID tree and a level tree. At the time of creation, the amount of gladiators
 * in a squad are 0, and the best gladiator does not exist (pointer points to null).
 */
Squad::Squad() {
    compare_id_tree = new CompareID();
    compare_level_tree = new CompareLevel();
    id_tree = new SplayTree<Gladiator*>(*compare_id_tree);
    level_tree = new SplayTree<Gladiator*>(*compare_level_tree);
    num_gladiators = 0;
    best_gladiator = NULL;
}

/*
 * The squad's d'tor. Simply de-allocates all of the data, namely: both of the comparators,
 * and both of the trees. Note that the trees do NOT free the data stored in them,
 * because the trees contain pointers.
 */
Squad::~Squad() {
    delete(id_tree);
    delete(level_tree);
    delete(compare_id_tree);
    delete(compare_level_tree);
}

/*
 * Removes all of the data stored in the squad's trees. Note that this function
 * does NOT free the trees' nodes, but rather the data that both of the trees share,
 * namely: the gladiators (class's d'tor is called).
 */
void Squad::removeAll(){
    id_tree->inOrderDel();
}

/*
 * Returns the amount of gladiators stored in the squad.
 */
int Squad::getSize() const {
    return num_gladiators;
}

/*
 * Returns the ID of the best gladiator in the squad. If the squad is empty, returns -1.
 */
int Squad::getBest() const {
    return (num_gladiators==0) ? -1 : best_gladiator->getID();
}

/*
 * Returns a pointer to the gladiator that corresponds to a given ID.
 * Parameters:
 * @ id: the ID of the desired gladiator.
 * Will throw exception of the gladiator does not exist in the squad.
 */
Gladiator* Squad::findGladiator(int id) {
    Gladiator temp_g(id, 1, NULL); //Dummy gladiator. D'tor is called at the end of the scope
    return id_tree->find(&temp_g);
}

/*
 * Adds a gladiator to the squad. The gladiator will be inserted into both of the squad's trees.
 * Additionally, if the insert was successful, raises the amount of gladiators in the squad by 1,
 * and finds the next best gladiator.
 * Parameters:
 * @ g: the gladiator we wish to insert to the squad.
 * Will throw exception if the gladiator already exists
 */
void Squad::addGladiator(Gladiator* g) {
    id_tree->insert(g);
    level_tree->insert(g);
    num_gladiators++;
    best_gladiator = level_tree->findMax();
}

/*
 * Allocates a new gladiator - creates
 */
Gladiator* Squad::addGladiator(int gladiator_id, int level, Trainer* trainer) {
    Gladiator *g = new Gladiator(gladiator_id, level, trainer);
    try {
        id_tree->insert(g);
        level_tree->insert(g);
    }catch(const ElementAlreadyExists& e){
        delete(g);
        throw failureException();
    }
    num_gladiators++;
    best_gladiator = level_tree->findMax();
    return g;
}

void Squad::removeGladiator(Gladiator* g) {
    removeElement(&id_tree, g);
    removeElement(&level_tree, g);
    num_gladiators--;
    best_gladiator = level_tree->findMax();
}

void Squad::scanAllByID(Procedure<Gladiator*>& operation) {
    id_tree->inOrderDel();
}

void Squad::scanAllByLevel(Procedure<Gladiator*>& operation) {
    level_tree->inOrder(operation);
}

void Squad::destroyLevelTree() {
    delete(level_tree);
    level_tree = new SplayTree<Gladiator*>(*compare_level_tree);
}

void Squad::updateMax() {
    best_gladiator = level_tree->findMax();
}

void Squad::concatGladiator(Gladiator* g) {
    concatLinear(&level_tree,g);
}