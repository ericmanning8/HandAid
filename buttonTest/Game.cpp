//
// Created by Eric Manning on 2020-02-08.
//

#include "Game.h"

/***********************************

         GAME Methods

***********************************/

Game::Game() {
    correctCount = 0;
    wrongCount = 0;
    totalPresses = 0;
}

void Game::wrongAnswer() {
    this->wrongCount++;
    this->updateTotal();
}

void Game::correctAnswer() {
    this->correctCount++;
    this->updateTotal();
}

void Game::updateTotal() {
    this->totalPresses = correctCount + wrongCount;
}

void Game::insertFinger(int x) {
    orderOfFingersUsed.PushBack(x);
}

void Game::insertTime(double t) {
    reactionTimes.PushBack(t);
}

Vector<double> Game::getReactionTimes() {
    return this->reactionTime;
}

Vector<int> Game::getFingersUsed() {
    return this->orderOfFingersUsed;
}

int Game::getScore() {
    return correctCount;
}

int getTotalPresses() {
    return totalPresses;
}