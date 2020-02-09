//
// Created by Eric Manning on 2020-02-08.
//

#ifndef VITERBIDROPOUTS_USER_H
#define VITERBIDROPOUTS_USER_H

#include <fstream>
#include <map>
#include <string>

#include "rbtree.h"
#include "Vector.h"

class User {
public:
    // Constructors
    User();
    //User(std::ifstream&);

    // public methods
    void viewData();
    void setNumberUses();
    void setAvgScore(int);
    void setAvgAccuracy(int, int);
    void updateAvgAccuracy(double);
    void setCurrentScore(int);
    void setNumPresses(int);
    void setReactionTimes(Vector<double> times, Vector<double> fingers);
private:
    // long term data
    int numGamesPlayed;

    // Data structures
    RBTree<int> scores; // red black tree of all user scores

    // This trial data
    int numPresses;
    int currScore;
    int currMisses;
    double totalAvgReactionTime;
    double pinkyAvgReactionTime;
    double ringAvgReactionTime;
    double middleAvgReactiontime;
    double indexAvgReactionTime;

    // user specific settings:
    unsigned int recomDiff;
    double f1Weight, f2Weight, f3Weight, f4Weught;

    // Average values
    double avgReactionTime;
    double avgAccuracy;
    double avgScore;
};

#endif //VITERBIDROPOUTS_USER_H
