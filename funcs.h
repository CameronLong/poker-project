#ifndef FUNCS_H
#define FUNCS_H
#include <iostream>
#include "Table.h"
#include "Player.h"
#include "Card.h"

using namespace std;
void getHandRanks(Table *table1, int numCommunityCards, fstream &myFile, string userAction);
void setBigThree(Table *table1, int dealerRotator, int smallBlindRotator, int bigBlindRotator, fstream &myFile);
// void dealHandsFunc(Table *table1, Card **deck, string username);
void refillDeck(Card** deck, Card** refill);
vector<string> readIntoVector(string filename);
void customSort(Card **unsortedArray, int length);
void setupCompBets(Table *table1, string action, string userAction);

#endif
