#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <fstream>
#include "Card.h"
using namespace std;

class Player {
    private:
        string firstName;
        int balance;
        Card* hand[2];
        string previousAction;
        int previousBet;
        int totalRoundBet;
        bool folded;
    public:
        Player() {}
        Player(string newFirst, int newBalance = 100, int newPreviousBet = 0) : firstName(newFirst), balance(newBalance), previousBet(newPreviousBet) {
            folded = true;
        }
        ~Player(){
            cout << this->firstName << " has left." << endl;
            delete(this);
        }
        void getPlayerInfo();
        string getPlayerName();
        int getPlayerBalance();
        void getHand();
        int getRoundBet();
        Card* returnHand(int index);
        bool getFolded();
        int getPrevBet();

        void writePlayerHand(string username, fstream &myFile);
        int getBalance();
        void setBalance(int newBalance);
        void writeHand(int comp, fstream &myFile);
        void setHand(Card **deck);
        void userBet(int userBet);
        void handleFold();

        void setRoundBet(int roundBet);
        void setPrevAction(string prevAction);
        void setFolded(bool newFolded);
        void setPrevBet(int oldBet);
};

#endif
