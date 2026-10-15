#include "Table.h"
#include <vector>
using namespace std;

void Table::getCommunityCards(int index) {
    communityCards[index]->getCardInfo();
    if (index < 4 && index > 1) {
        cout << " | ";
    }
}

void Table::getAllCommunityCards(int numCards, vector<string> &lines) {
    string tempVectorLine = "";

    lines[13] = "Community cards: ";
    cout << "Community cards: ";
    for (int i = 0; i < numCards; i++) {
        cout << communityCards[i]->getValue() << communityCards[i]->getSuit();
        lines[13] += to_string(communityCards[i]->getValue()) + communityCards[i]->getSuit() + " | ";
        cout << " | ";
    }

    // cout << endl << lines[14] << endl;
    fstream outputFile("testing_comp_output.txt");
    int temp = 0;
    for(const string vectorLine : lines){
        outputFile << vectorLine << endl;
        temp++;
    }
    outputFile.close();
}

Card* Table::getCommunityCard(int index){
    return communityCards[index];
}


int Table::getPot() {
    return this->potValue;
}


Player* Table::getDealer() {
    return dealer;
}

Player* Table::getSmallBlind() {
    return smallBlind;
}


Player* Table::getBigBlind() {
    return bigBlind;
}

void Table::getPlayers() {
    for (int i = 0; i <= 4; i++) {
        this->activePlayers[i]->getPlayerInfo();
    }
}
int Table::getValue(int index) {
    return communityCards[index]->getValue();
}

string Table::getSuit(int index) {
    return communityCards[index]->getSuit();
}

 
Player* Table::getPlayer(int index) {
    return activePlayers[index];
}

int Table::getCheckCounter(){
    return checkCounter;
}

string Table::getResults(int i, int j){
    return results[i][j];
}

bool Table::handlePot(string potWinnerName){
    cout << "Pot value is: $" << potValue << " and has been awarded to: " << potWinnerName << endl;
    for(int i = 0; i <= 3; i++){
        int newBalance = 0;
        if(results[i][0] != potWinnerName){
            for(int j = 0; j <= 3; j++){
                if(results[i][0] == activePlayers[j]->getPlayerName()){
                    newBalance = activePlayers[j]->getBalance() - stoi(results[i][2]);
                    if(newBalance < 0){
                        newBalance = 0;
                    }
                    activePlayers[j]->setBalance(newBalance);
                    cout << "New balance for " << results[i][0] << " is $" << newBalance << endl;
                }
            }
        } else if (results[i][0] == potWinnerName){
            getPlayer(i)->setBalance(getPlayer(i)->getBalance() + potValue);
        }
    }
    cout << "You have ended the game with $" << this->activePlayers[4]->getBalance() << endl;
    return true;
}

int Table::isCards(){
    if(communityCards[0]->getSuit() == "NULL"){
        return 0;
    } else {
        return 1;
    }
}
void Table::setTable(int potValue) {
    this->potValue = potValue;
}

void Table::setPot(int value) {
    this->potValue = value;
}
void Table::setDealer(Player* dealer, fstream &myFile) { 
    this->dealer = dealer;
    myFile << "Dealer: " << dealer->getPlayerName() << "\n";
}
void Table::setSmallBlind(Player* smallBlind, fstream &myFile) {
    this->smallBlind = smallBlind;
    myFile << "Small blind: " << smallBlind->getPlayerName() << "\n";
}
void Table::setBigBlind(Player* bigBlind, fstream &myFile) {
    this->bigBlind = bigBlind;
    myFile << "Big blind: " << bigBlind->getPlayerName() << "\n";
    }
void Table::setPlayers(Player* players[5]) {
    memcpy(activePlayers, players, 5 * sizeof(Player));
}
void Table::setCurrentBet() {}

void Table::setCommunityCard(Table *thisTable, int cardIndex, int cardVal, string cardSuit){
    thisTable->communityCards[cardIndex]->setCardInfo(cardVal, cardSuit);
}

void Table::setResults(int i, int j, string value){
    results[i][j] = value;
}

void Table::incrementCheckCounter(){
    checkCounter++;
}

void Table::generateCommunityCards(Card **deck) {
    srand(time(0));
    for (int i = 0; i <= 4; ++i) {
        int deckIndex = rand() % 52;
        Card *tempCard = deck[deckIndex];
        if(deck[deckIndex]->getValue() == 0 || deck[deckIndex]->getValue() > 14){
            deckIndex += 1;
        }
        this->communityCards[i] = new Card(deck[deckIndex]->getValue(), deck[deckIndex]->getSuit());
        // cout << communityCards[i]->getValue() << communityCards[i]->getSuit() << endl;
        deck[deckIndex]->setCardInfo(0, "NULL");
        // cout << communityCards[i]->getValue() << communityCards[i]->getSuit() << endl;
    }
}
