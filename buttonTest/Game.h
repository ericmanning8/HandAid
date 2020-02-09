//
// Created by Eric Manning on 2020-02-08.
//

#ifndef VITERBIDROPOUTS_GAME_H
#define VITERBIDROPOUTS_GAME_H

class Game {
public:
    // constructor
    Game();
    //Game(User*);

    // public methods
    void correctAnswer();
    void wrongAnswer();
    void updateTotal();

    void insertFinger();
    void insertTime();

    Vector<double> getReactionTimes();
    Vector<int> getFingersUsed();
    int getScore();
    int getTotalPresses();
private:
    int correctCount;
    int wrongCount;
    int totalPresses;

    Vector<double> reactionTimes; // vector of reaction times
    Vector<int> orderOfFingersUsed; //
};

#endif //VITERBIDROPOUTS_GAME_H
