#include "Player.h"
using namespace std;

void Player::writeHand(int comp, fstream &myFile) {
    if(myFile.is_open()){
        myFile << "Hand for Comp" << comp+1 << ": " << hand[0]->getValue() << hand[0]->getSuit() << " | " << hand[1]->getValue() << hand[1]->getSuit() << endl;
        myFile << "Action for Comp" << comp+1 << ": none, bet: $0" << endl;
    } else {
        cout << "file not open" << endl;
    }
}

void Player::setHand(Card **deck) {
    hand[0] = new Card(0, "NULL");
    hand[1] = new Card(0, "NULL");
    srand(time(0));
    int card1 = rand() % 52;
    int card2 = rand() % 52;
    if(deck[card1]->getValue() == 0 || deck[card2]->getValue() == 0){
        while(deck[card1]->getValue() == 0 || deck[card2]->getValue() == 0){
            if(deck[card1]->getValue() == 0){
                card1 = rand() % 52;
            } else if (deck[card2]->getValue() == 0){
                card2 = rand() % 52;
            }
        }
    }
    hand[0]->setCardInfo(deck[card1]->getValue(), deck[card1]->getSuit());
    hand[1]->setCardInfo(deck[card2]->getValue(), deck[card2]->getSuit());
    // cout << "hand for comp: " << hand[0]->getValue() << hand[0]->getSuit() << " | " << hand[1]->getValue() << hand[1]->getSuit() << endl;
    
    deck[card1]->setCardInfo(0, "NULL");
    deck[card2]->setCardInfo(0, "NULL");
}

void Player::userBet(int userBet){
    while(userBet > this->balance){
        cout << "Insufficient funds. Please try again: " << endl;
        cin >> userBet;
    }
    cout << "You have decided to bet $" << userBet << endl;
    this->balance -= userBet;
}

void Player::getPlayerInfo() { 
    cout << "Player name: " << this->firstName << ", balance: $" << this->balance << endl; 
}

string Player::getPlayerName(){
    return this->firstName;
}

int Player::getPlayerBalance() {
    return this->balance;
}

void Player::getHand() {
    cout << this->hand[0]->getValue() << this->hand[0]->getSuit() << " | " << this->hand[1]->getValue() << this->hand[1]->getSuit() << endl;
}

Card* Player::returnHand(int index) {
    return hand[index];
}

void Player::writePlayerHand(string username, fstream &myFile){
    myFile << "Hand for " << username << ": " << hand[0]->getValue() << hand[0]->getSuit() << " | " << hand[1]->getValue() << hand[1]->getSuit() <<endl;
    myFile << "Action for " << username << ": none, bet: $0" << endl;
}

int Player::getBalance() {
    return balance;
}

void Player::setBalance(int newBalance) {
    this->balance = newBalance;
}

int Player::getRoundBet(){
    return totalRoundBet;
}

bool Player::getFolded(){
    return folded;
}

int Player::getPrevBet(){
    return previousBet;
}

int Player::getCurrentBet(){
    return currentBet;
}

void Player::setRoundBet(int roundBet){
    totalRoundBet += roundBet;
}

void Player::setPrevAction(string prevAction){
    previousAction = prevAction;
}

void Player::set_current_action(string current_action){
    this->current_action = current_action;
}

void Player::setFolded(bool newFolded){
    folded = newFolded;
}

void Player::setPrevBet(int oldBet){
    previousBet = oldBet;
}

void Player::handleFold(){
    // cout << "You have decided to fold." << endl;
    hand[0]->setCardInfo(0, "NULL");
    hand[1]->setCardInfo(0, "NULL");
    balance -= previousBet;
    folded = true;
}
