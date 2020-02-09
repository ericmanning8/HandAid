//
// Created by Eric Manning on 2020-02-08.
//

#include "User.h"

/***********************************

         USER Methods

***********************************/

//
// Created by Eric Manning on 2020-02-08.
//

User::User() {
    numGamesPlayed = 0;
    numPresses = 0;
    currScore = 0;
    currMisses = 0;
    recomDiff = 0;
    f1Weight = 0.25, f2Weight = 0.25, f3Weight = 0.25, f4Weught = 0.25;
    avgReactionTime = 0;
    avgAccuracy = 0;
    avgScore = 0;
}

/*
User::User(&ifstream pastData) {
    if (pastData.fail()) {
        serial.println("");
        User();
        return;
    }
} */

void User::viewData() {
    Serial.println("Total Number of Games Played: ");
    Serial.println(numGamesPlayed);
    Serial.println("Your average score is: ");
    Serial.println(avgScore);
    Serial.println("The most recent score was: ");
    Serial.println(currScore);
    Serial.println("Difficulty: ");
    Serial.println(recomDiff);
    Serial.println("");
    Serial.println(avgReactionTime);
    Serial.println("");
    Serial.println(avgAccuracy);
}

void User::setNumberUses() {
    this->numGamesPlayed++;
}

void User::setAvgScore(int score) {
    this->avgScore = (this->avgScore * (this->numGamesPlayed - 1) + score) / this->numGamesPlayed;
}

void User::setAvgAccuracy(int score, int numPresses) {
    double temp = (score)/(numPresses);
    this->updateAvgAccuracy(temp);
}

void User::updateAvgAccuracy(double a) {
    this->avgAccuracy = (this->avgAccuracy * (this->numGamesPlayed - 1) + a) / this->numGamesPlayed;
}

void User::setCurrentScore(int score) {
    this->currScore = score;
    this->currMisses = this->numPresses - score;
    this->scores.insert(score, &score);

    this->setAvgScore(score);
    this->setAvgAccuracy(this->currScore, this->numPresses);
}

void User::setNumPresses(int n) {
    this->numPresses = n;
}

// A utility function to swap two elements
void swap(double* a, double* b)
{
    int t = *a;
    *a = *b;
    *b = t;
}

/* This function takes last element as pivot, places
the pivot element at its correct position in sorted
array, and places all smaller (smaller than pivot)
to left of pivot and all greater elements to right
of pivot */
int partition (double *arr, int low, int high) {
    int pivot = arr[high]; // pivot
    int i = (low - 1); // Index of smaller element

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}

void quickSort(double *arr, int low, int high) {
    if (low < high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void setReactionTimes(Vector<double> times, Vector<double> fingers) {
    double temp = 0, tempP = 0, tempR = 0, tempM = 0, tempI = 0;
    int P = 0, R = 0, M = 0, I = 0;

    for (int i = 0; i < times.Size(); i++) {
        if (finger[i] == pinky) {
            tempP += times[i];
            P++;
        }
        else if (finger[i] == ring) {
            tempR += times[i];
            R++;
        }
        else if (finger[i] == middle) {
            tempM += times[i];
            M++;
        }
        else {
            tempI += times[i];
            I++;
        }
        temp += times[i];
    }
    this->totalAvgReactiontime = temp/times.Size(); // total
    this->pinkyAvgReactionTime = tempP/P; // pinky
    this->ringAvgReactionTime = tempR/R; // ring
    this->middleAvgReactionTime = tempM/M; // middle
    this->indexAvgReactionTime = tempI/I; // index

    return;
}