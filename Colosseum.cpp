//
// Created by sahar on 03/12/2017.
//
#include <cstdlib>
#include "Colosseum.h"
#include "Exceptions.h"
class updateLevelsTrainers : public Procedure<Trainer*>{
public:
    virtual void operator()(Trainer* val) {
        val->getSquad()->updateMax();
    }
};
/*
 * Classic Merge: merges two sorted arrays into a third array, keeping the original sorted order.
 */
void
merge_gladiators(Gladiator *arr1[], int size1, Gladiator *arr2[], int size2,
                 Gladiator *arr3[]) {
    int i = 0, j = 0, k = 0, temp_level = 0, temp_id = 0;
    while (i < size1 && j < size2) {
        temp_level = arr2[j]->getLevel() - arr1[i]->getLevel();
        if (temp_level > 0) {
            arr3[k++] = arr1[i++];
        } else if (temp_level < 0) {
            arr3[k++] = arr2[j++];
        }
        else {
            temp_id = arr2[j]->getID() - arr1[i]->getID();
            if (temp_id > 0) {
                arr3[k++] = arr2[j++];
            }
            else {
                arr3[k++] = arr1[i++];
            }
        }
    }
    //Fill the leftovers from one of the arrays
    for (; i < size1; i++) {
        arr3[k++] = arr1[i];
    }
    for (; j < size2; j++) {
        arr3[k++] = arr2[j];
    }
}

/*
 * A functor that inherits from the abstract "Procedure" class, as defined inside the SplayTree class.
 * The functor counts the amount of gladiators in the tree whose IDs divide by a given number n.
 */
class countDividableElementsFunction : public Procedure<Gladiator *> {
    int n;
    int count;
public:
    countDividableElementsFunction(int n) {
        this->n = n;
        count = 0;
    }

    void operator()(Gladiator *g) {
        if (g->getID() % n == 0) {
            count++;
        }
    }

    int getCount() {
        return count;
    }
};

/*
 * A functor that inherits from the abstract "Procedure" class, as defined inside the SplayTree class.
 * The functor fills an array of pointers of gladiators from the tree, in the order of display from the scan function.
 */
class toArrayFunction : public Procedure<Gladiator *> {
    Gladiator **array; //USER FREES THIS
    int iterator;
public:
    toArrayFunction(Gladiator **array) {
        this->array = array;
        iterator = 0;
    }

    void operator()(Gladiator *g) {
        array[iterator] = g;
        iterator++;
    }
};

/*
 * A functor that inherits from the abstract "Procedure" class, as defined inside the SplayTree class.
 * The functor fills an array of IDs of gladiators from the tree, in reverse order to the display order from the scan function.
 */
class toReverseArrayFunction : public Procedure<Gladiator *> {
    int **array; //USER FREES THIS
    int size;
    int iterator;
public:
    toReverseArrayFunction(int **array, int size) {
        this->array = array;
        this->size = size;
        iterator = 0;
    }

    void operator()(Gladiator *g) {
        iterator++;
        (*array)[size - iterator] = g->getID();
    }
};

/*
 * A functor that inherits from the abstract "Procedure" class, as defined inside the SplayTree class.
 * The functor fills two arrays of pointers of gladiators from the tree.
 * Every gladiator whose ID is divisible by a given number n will be stored in one array, and the others in the second one.
 */
class twoArrayPredicateFunction : public Procedure<Gladiator *> {
    Gladiator **suitables;
    Gladiator **not_suitables;
    int iterator1;
    int iterator2;
    int n;
public:
    twoArrayPredicateFunction(Gladiator **suitables, Gladiator **not_suitables,
                              int n) {
        this->suitables = suitables;
        this->not_suitables = not_suitables;
        iterator1 = 0;
        iterator2 = 0;
        this->n = n;
    }

    void operator()(Gladiator *g) {
        if (g->getID() % n == 0) {
            suitables[iterator1++] = g;
        } else {
            not_suitables[iterator2++] = g;
        }
    }
};

/*
 * A functor that inherits from the abstract "Procedure" class, as defined inside the SplayTree class.
 * The functor frees the level tree of all the trainers in the tree.
 */
class destroyTrainersLevelTreeFunction : public Procedure<Trainer *> {
public:
    void operator()(Trainer *t) {
        t->getSquad()->destroyLevelTree();
    }
};

/*
 * The class's C'tor. Allocates a new tree of trainers, and a squad of gladiators.
 */
Colosseum::Colosseum() {
    compare_trainers_tree = new compareTrainerID();
    trainers = new SplayTree<Trainer *>(*compare_trainers_tree);
    main_squad = new Squad();
}

/*
 * The class's D'tor. Frees all the memory stored (tree of trainers and a squad of gladiators).
 * The trees' D'tor do NOT free the gladiators and trainers, so the D'tor scans the trees in-order and frees them first,
 * before de-allocating the entire structure.
 */
Colosseum::~Colosseum() {
    trainers->inOrderDel();
    main_squad->removeAll();
    delete (main_squad);
    delete (trainers);
    delete (compare_trainers_tree);
}

/*
 * Adds a new trainer to the system. Allocates a new trainer and inserts it to the trainers' tree.
 * Parameters:
 * @ trainerID: the ID of the trainer we wish to add to the system.
 * An exception is thrown in the following cases:
 * - the given ID is illegal (not a positive number): INVALID_INPUT
 * - there already exists a trainer with the given ID: FAILURE
 * - allocation error.
 */
StatusType Colosseum::AddTrainer(int trainerID) {
    if(trainerID<1){
        return INVALID_INPUT;
    }
    Trainer *new_trainer=NULL;
    try {
        new_trainer = new Trainer(trainerID);
        trainers->insert(new_trainer);
    } catch (const std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (const failureException &e) {
        if(new_trainer) {
            delete (new_trainer);
        }
        return FAILURE;
    }
    return SUCCESS;

}

/*
 * Adds a new gladiator to the system. Allocates a new gladiator and inserts it
 * into the main squad, and to its corresponding trainer's squad.
 * Parameters:
 * @ gladiatorID: the ID of the gladiator we wish to add to the system.
 * @ trainerID: the ID of the trainer who's responsible for this gladiator.
 * @ level: the level of the gladiator.
 * An exception is thrown in the following cases:
 * - either gladiator's ID, the trainer's ID or the level are illegal (not positive numbers): INVALID_INPUT
 * - there is no trainer with the given ID: FAILURE
 * - there's already a gladiator with the given ID: FAILURE
 * - allocation error.
 */
StatusType Colosseum::BuyGladiator(int gladiatorID, int trainerID, int level) {
    try {
        if (gladiatorID <= 0 || trainerID <= 0 || level <= 0) {
            return INVALID_INPUT;
        }
        Trainer temp_trainer(
                trainerID); //We temporarily create a trainer with the given ID in order to search for it in the tree
        Trainer *desired_trainer = trainers->find(&(temp_trainer));
        Gladiator *new_gladiator;
        new_gladiator = main_squad->addGladiator(gladiatorID, level,
                                                 desired_trainer);
        desired_trainer->getSquad()->addGladiator(new_gladiator);

    } catch (const std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (const invalidInputException &e) {
        return INVALID_INPUT;
    }
    catch (const failureException &e) {
        return FAILURE;
    }
    return SUCCESS;
}

/*
 * Removes a gladiator from the system. Frees all the tree nodes that took part in its storage.
 * Parameters:
 * @ gladiatorID: the ID of the gladiator we wish to remove from the system.
 * An exception is thrown in the following cases:
 * - the given ID is illegal (not a positive number): INVALID_INPUT
 * - there is no gladiator with the given ID in the system: FAILURE
 * - allocation error.
 */
StatusType Colosseum::FreeGladiator(int gladiatorID) {
    try {
        if (gladiatorID <= 0) {
            throw badLevelException();
        }
        Gladiator *to_remove = main_squad->findGladiator(gladiatorID);
        main_squad->removeGladiator(to_remove);
        to_remove->getTrainer()->getSquad()->removeGladiator(to_remove);
        delete (to_remove);
    } catch (const invalidInputException &e) {
        return INVALID_INPUT;
    }
    catch (const failureException &e) {
        return FAILURE;
    }
    return SUCCESS;
}

/*
 * Raises the level of a given gladiator in the system.
 * Parameters:
 * @ gladiatorID: the ID of the gladiator whose level we wish to update.
 * @ levelIncrease: the amount of levels we wish to increase the gladiator's level by.
 * An exception is thrown in the following cases:
 * - Either the given ID is illegal or the level increase (not positive numbers): INVALID INPUT
 * - there is no gladiator with the given ID in the system: FAILURE
 * - allocation error.
 */
StatusType Colosseum::LevelUp(int gladiatorID, int levelIncrease) {
    try {
        if (levelIncrease <= 0) {
            throw badLevelException();
        }
        Gladiator *to_update = main_squad->findGladiator(gladiatorID);
        main_squad->removeGladiator(to_update);
        to_update->getTrainer()->getSquad()->removeGladiator(to_update);
        to_update->increaseLevel(levelIncrease);
        main_squad->addGladiator(to_update);
        to_update->getTrainer()->getSquad()->addGladiator(to_update);
    } catch (const std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (const invalidInputException &e) {
        return INVALID_INPUT;
    }
    catch (const failureException &e) {
        return FAILURE;
    }
    return SUCCESS;
}

/*
 * Finds the gladiator of a certain trainer who has the highest level. If multiple gladiators
 * share this level value - the one with the lowest ID is returned.
 * In the case where the trainer ID is a negative number - the same process would be
 * executed, but out of all the gladiators in the system instead.
 * The gladiator found will be stored inside a given parameter.
 * parameters:
 * @ trainerID: the ID of the trainer who owns the gladiators we want to search.
 * @ gladiatorID: a pointer that, at the end of this function's run, will point at
 * the ID of the desired gladiator.
 * An exception is thrown in the following cases:
 * - Either the trainer ID is illegal (equals zero), or the gladiator ID points to NULL: INVALID INPUT
 * - The trainer ID is a positive number, but the trainer does not exist: FAILURE.
 * - allocation error.
 */
StatusType Colosseum::GetTopGladiator(int trainerID, int *gladiatorID) {
    try {
        if (trainerID == 0) {
            throw badIdException();
        }
        if (gladiatorID == NULL) {
            throw failureException();
        }
        Squad *desired_squad = main_squad;
        if (trainerID > 0) {
            Trainer temp_trainer(trainerID);
            Trainer *desired_trainer = trainers->find(&(temp_trainer));
            desired_squad = desired_trainer->getSquad();
        }
        *gladiatorID = desired_squad->getBest();
    } catch (const std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (const invalidInputException &e) {
        return INVALID_INPUT;
    }
    catch (const failureException &e) {
        return FAILURE;
    }
    return SUCCESS;
}

/*
 * Returns all of the gladiators owned by a trainer in an array, in the following order:
 * the array is sorted by level (HIGHEST to SMALLEST), and then by ID (SMALLEST to HIGHEST).
 * If the trainer ID is negative, the array will store all of the gladiators in this order.
 * In addition, the number of gladiators is stored in a given parameter.
 * parameters:
 * @ trainerID: the ID of the trainer.
 * @ gladiators: pointer to an array that will be allocated in the function, and freed
 * elsewhere by the user. The array will store all of the gladiators (either in the
 * entire system or of a given trainer), sorted in the previously described order.
 * @ numOfGladiators - pointer that will store the number of gladiators for this trainer
 * or in the entire system.
 * An exception is thrown in the following cases:
 * - Either the trainer ID is illegal (equals zero), or one of the pointers points to NULL: INVALID INPUT
 * - The trainer ID is a positive number, but the trainer does not exist: FAILURE.
 * - allocation error.
 */
StatusType Colosseum::GetAllGladiatorsByLevel(int trainerID, int **gladiators,
                                              int *numOfGladiator) {
    try {
        if (trainerID == 0) {
            throw badIdException();
        }
        if(gladiators == NULL || numOfGladiator == NULL) {
            throw invalidInputException();
        }
        Squad *desired_squad = main_squad;
        if (trainerID > 0) {
            Trainer temp_trainer(trainerID);
            Trainer *desired_trainer = trainers->find(&(temp_trainer));
            desired_squad = desired_trainer->getSquad();
        }
        int num = desired_squad->getSize();
        *numOfGladiator = num;
        *gladiators = (int *) malloc(sizeof(int) * num); //USER'S RESPONSIBILITY TO FREE THIS!
        toReverseArrayFunction *operation = new toReverseArrayFunction(
                gladiators, num);
        desired_squad->scanAllByLevel(*operation);
        delete (operation);

    } catch (const std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (const invalidInputException &e) {
        return INVALID_INPUT;
    }
    catch (const failureException &e) {
        return FAILURE;
    }
    return SUCCESS;
}

/*
 * Changes the ID of a given gladiator in the system.
 * Parameters:
 * @ gladiatorID: the ID of the gladiator whose level we wish to update.
 * @ levelIncrease: the amount of levels we wish to increase the gladiator's level by.
 * An exception is thrown in the following cases:
 * - either the given ID or the new ID are illegal (not positive numbers): INVALID INPUT
 * - there is no gladiator with the given ID in the system: FAILURE
 * - the current ID and the new ID of the gladiator are both equal: FAILURE
 * - allocation error.
 */
StatusType Colosseum::UpgradeGladiator(int gladiatorID, int upgradedID) {
    try {
        if (gladiatorID == upgradedID) {
            throw failureException();
        }
        if (gladiatorID <= 0) {
            throw badLevelException();
        }
        Gladiator* to_update = main_squad->findGladiator(gladiatorID);
        try {
            /*
             * We need to check if there is already a gladiator in the system with the
             * desired ID - we'll simply try to find it in the main squad, and if
             * and exception is thrown: then it does not exist (which is good),
             * otherwise, if no exception is thrown, that means the gladiator already
             * exists in the system, so we return FAILURE as requested.
             */
            main_squad->findGladiator(upgradedID);
            return FAILURE;
        } catch (const failureException &e) { }
        main_squad->removeGladiator(to_update);
        to_update->getTrainer()->getSquad()->removeGladiator(to_update);
        to_update->setID(upgradedID);
        main_squad->addGladiator(to_update);
        to_update->getTrainer()->getSquad()->addGladiator(to_update);
    } catch (const std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (const invalidInputException &e) {
        return INVALID_INPUT;
    }
    catch (const failureException &e) {
        return FAILURE;
    }
    return SUCCESS;
}

/*
 * Updates the levels of all the gladiators in the system whose ID is dividable by a given
 * number. The new levels of the gladiators will be the previous level multiplied by a given factor.
 * Parameters:
 * @ stimulantCode: the predicate: the number for which we will find gladiators with ID that divide by this.
 * @ stimulantFactor: the factor by which we will multiply the levels of the suitable gladiators.
 * An exception is thrown in the following cases:
 * - either one of the parameters is non-positive : INVALID INPUT
 * - allocation error.
 */
StatusType Colosseum::UpdateLevels(int stimulantCode, int stimulantFactor) {
    try {
        if (stimulantCode < 1 || stimulantFactor < 1) {
            throw invalidInputException();
        }
        if (stimulantFactor == 1) {
            return SUCCESS; //No gladiators will be changed, even those who fulfil the requirement
        }
        countDividableElementsFunction divides_by_code(stimulantCode);
        main_squad->scanAllByLevel(divides_by_code);
        int m = divides_by_code.getCount();
        if (m == 0) {
            return SUCCESS; //No gladiators fulfil the requirement: return success.
        }
        Gladiator **suitable = new Gladiator *[m];
        int n = main_squad->getSize();
        if (m == n) {
            toArrayFunction allGladiators(suitable);
            main_squad->scanAllByLevel(allGladiators);
            for (int i = 0; i < m; i++) {
                suitable[i]->increaseLevel(
                        suitable[i]->getLevel() * (stimulantFactor - 1));
            }
            delete[] suitable;
            return SUCCESS; //All gladiators fulfil the requirement and have been updated. Original order is intact. Return success.
        }
        Gladiator **not_suitable = new Gladiator *[n - m];
        twoArrayPredicateFunction scan_suitable(suitable, not_suitable,
                                                stimulantCode);
        main_squad->scanAllByLevel(scan_suitable);
        for (int i = 0; i < m; i++) {
            suitable[i]->increaseLevel(
                    suitable[i]->getLevel() * (stimulantFactor - 1));
        }
        Gladiator **sorted_gladiators = new Gladiator *[n];
        merge_gladiators(suitable, m, not_suitable, n - m, sorted_gladiators);
        /*
         * The sorted array has been created. This array stores the values of all the
         * gladiators in the system AFTER the changes apply to them, sorted by level(smallest to biggest)
         * and then by ID(biggest to smallest). Since we've changed the fields of the gladiators directly
         * through the pointers stored in the array - ALL of the trees in the system that are sorted with
         * levels as keys are not BSTs anymore, and have to be destroyed.
         */
        delete[] suitable;
        delete[] not_suitable;
        main_squad->destroyLevelTree();
        destroyTrainersLevelTreeFunction remove_illegal_trees;
        trainers->inOrder(remove_illegal_trees);
        // All the level trees in the system have been destroyed. We'll restore them (in O(n)!!!)
        Squad *system_squad = main_squad;
        Squad *trainer_squad = NULL;
        for (int i = 0; i < n; i++) {
            trainer_squad = sorted_gladiators[i]->getTrainer()->getSquad();
            system_squad->concatGladiator(sorted_gladiators[i]);
            trainer_squad->concatGladiator(sorted_gladiators[i]);
        }
        delete[] sorted_gladiators;
        system_squad->updateMax();
        updateLevelsTrainers update;
        trainers->inOrder(update);
    } catch (const std::bad_alloc &e) {
        return ALLOCATION_ERROR;
    }
    catch (const invalidInputException &e) {
        return INVALID_INPUT;
    }
    catch (const failureException &e) {
        return FAILURE;
    }
    return SUCCESS;
}