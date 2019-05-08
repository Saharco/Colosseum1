//
// Created by sahar on 03/12/2017.
//

#ifndef COLOSSEUM_EXCEPTIONS_H
#define COLOSSEUM_EXCEPTIONS_H

class invalidInputException : public std::exception {};
class failureException : public std::exception {};

class badIdException : public invalidInputException {};
class badLevelException : public invalidInputException {};
class uninitializedColosseumException : public invalidInputException {};
class badStimulantCodeException : public invalidInputException {};
class badStimulantFactorException : public invalidInputException {};
class sameIdException : public failureException {};


#endif //COLOSSEUM_EXCEPTIONS_H
