#ifndef CARD_H
#define CARD_H

#include <iostream>
#include <cstring>
using namespace std;

class Card {
    private:
        int value;
        string suit;
    public:
        Card() {
            this->value = 0;
            this->suit = "NULL";
        }
        Card(int value, string suit) {
            this->value = value;
            this->suit = suit;
        }
        ~Card(){
            delete(this);
        }
        void getCardInfo() {
            cout << this->value << this->suit;
        }

        void setCardInfo(int value, string suit) {
            this->value = value;
            this->suit = suit;
        }
        Card getCard(int value, string suit) {
            return Card(value, suit);
        }

        int getValue() {
            return this->value;
        }
        string getSuit() {
            return this->suit;
        }

        Card* operator=(Card *cardToCopy){
            value = cardToCopy->getValue();
            suit = cardToCopy->getSuit();
            return this;
        }
};

#endif
