#ifndef TABLE_H
#define TABLE_H

#include <iostream>
#include "Card.h"
#include "Player.h"
using namespace std;

class Table {
    private:
        int potValue;
        Player* dealer;
        Player* smallBlind;
        Player* bigBlind;
        int currentBet;
        Card** communityCards = new Card*[5];
        Player* activePlayers[5];
        string results[5][5];
        int checkCounter;
    public:
        Table(Player** newPlayers) {
            for(int i = 0; i <= 4; i++){
                activePlayers[i] = newPlayers[i];
            }
            this->potValue = 0;
        }
        ~Table() {
            cout << "The game is over." << endl;
            delete(this);
        }

        
        // Getters
        int getPot();
        int isCards();
        Player* getDealer();
        Player* getSmallBlind();
        Player* getBigBlind();
        void getPlayers();
        void getCurrentBet();
        Player* getPlayer(int index);
        void getCommunityCards(int index);
        Card* getCommunityCard(int index);
        int getCommunityCardLength();
        void getAllCommunityCards(int numCards);
        int getValue(int index);
        string getSuit(int index);
        string getResults(int index, int value);
        int getCheckCounter();

        // Setters
        void setTable(int potValue);
        void setPot(int value);
        void setDealer(Player* dealer);
        void setSmallBlind(Player* smallBlind);
        void setBigBlind(Player* bigBlind);
        void setPlayers(Player* players[5]);
        void setCurrentBet();
        void setCommunityCard(Table& thisTable, int cardIndex, int cardVal, string cardSuit);
        void setResults(int i, int j, string value);
        void incrementCheckCounter();

        void generateCommunityCards(Card **deck);
        
        friend ostream& operator<<(ostream& os, Table thisTable){
            cout << "Players at the table: ";
            for(int i = 0; i <= 4; i++){
                if(i != 4){
                    cout << thisTable.activePlayers[i]->getPlayerName() << ", ";
                } else if (i == 4) {
                    cout << thisTable.activePlayers[i]->getPlayerName() << endl;
                }
            }
            cout << "Current pot value: " << thisTable.getPot() << endl;
            return os;
        }

        bool handlePot(string potWinnerName);
};

#endif
