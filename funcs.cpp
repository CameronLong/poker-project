#include "Table.h"
#include "Player.h"
#include "Card.h"
#include <iostream>
#include <regex>
#include <vector>
#include <limits>
using namespace std;


vector<string> readIntoVector(string filename){
    fstream myFile("testing_comp_output.txt");
    vector<string> lines;
    string line = "";
    while (getline(myFile, line)) {
        lines.push_back(line);
    }
    return lines;
}

void customSort(Card **unsortedArray, int length){
    for(int i = 0; i < length; i++){
        for(int j = 0; j < length - 1; j++){
            if(unsortedArray[j]->getValue() > unsortedArray[j + 1]->getValue()){
                Card *temp = new Card(unsortedArray[j+1]->getValue(), unsortedArray[j+1]->getSuit());
                unsortedArray[j+1] = unsortedArray[j];
                unsortedArray[j] = temp;
            }
        }
    }
}

void setupCompBets(Table& table1, string action, string userAction){
    // Set up comp bets
    // Uses i from the getHandRanks function
    int tempPotValue = 0;
    for(int i = 0; i <= 3; i++){
        table1.setResults(i, 0, ("Comp" + to_string(i+1)));
        table1.setResults(i, 2, to_string(0));
        if(action == "bet"){
            // Generate a random bet
            int tempCompBet = 15 + rand() % (20 - 15 + 1), newBalance = 0, currentBalance = 0;
            // Set the action to bet
            table1.setResults(i, 1, "bet");

            for(int j = 0; j <= 3; j++){
                if(table1.getResults(i, 0) == table1.getPlayer(j)->getPlayerName()){
                    currentBalance = table1.getPlayer(j)->getBalance();

                    // If the current balance is not zero, then we want the computer to bet some amount
                    if(currentBalance != 0){
                        // If the generated bet is greater than the computers balance, we wan to re-generate
                        // the bet using their balance as the uppper bound
                        while(tempCompBet > currentBalance){
                            tempCompBet = 1 + rand() % (currentBalance - 1 + 1);
                        }
                    }
                    newBalance = currentBalance - tempCompBet;
                    int tempBalance = newBalance;
                    if(tempBalance < 0){
                        newBalance = currentBalance;
                    }
                    table1.getPlayer(j)->setBalance(newBalance);
                }
            }
            table1.setResults(i,2, to_string(tempCompBet));
        } else if (action == "fold"){
            table1.setResults(i, 1, "fold");
            table1.setResults(i, 2, to_string(0));
        } else if (action == "check"){
            table1.setResults(i, 1, "check");
            table1.setResults(i, 2, to_string(0));
        }
        tempPotValue += stoi(table1.getResults(i, 2));
        table1.setPot(tempPotValue);
    }

    int compIterator = 1, lineCounter = 0, betCounter = 0;

    for(int i = 0; i <= 3; i++){
        cout << table1.getResults(i, 0) << " has decided to " << table1.getResults(i, 1) << endl;
    }
}


void getHandRanks(Table& table1, int numCommunityCards, string userAction){
    // rank_count created to count ranks in a hand. There are 15 slots so that the index of the card is the value
    // Two slots are wasted in this case, but keeping the index and value the same makes sense here
    // combined_cards is created to store the community cards and the players hand for comparison and counting
    int rank_count[15];
    vector<int> combined_cards;
    // this creates a variable for hand counting, used in the pre-flop check
    int pre_flop_value = 0;

    // Pre-flop check
    if(numCommunityCards == 0){
        for(int i = 0; i <= 4; i++){
            pre_flop_value = table1.getPlayer(i)->returnHand(0)->getValue() + table1.getPlayer(i)->returnHand(1)->getValue();

            // Checks to see if the computer has pocket nines or better
            if(pre_flop_value > 18){
                table1.getPlayer(i)->set_current_action("bet");     // If the value is > 18 bet
            } else {    // If not then fold
                table1.getPlayer(i)->set_current_action("fold");
            }
        }
    }

    // This check should run anytime after the flop comes out
    if(numCommunityCards >= 3){
        // Create a loop that runs until j is the number of community cards since the length of community cards
        // can be 3, 4, or 5. This is done outside of the player llop so it does not get executed everytime i increments
        for(int j = 0; j <= numCommunityCards; j++){
            combined_cards.push_back(table1.getCommunityCard(j)->getValue());
        }

        // Loop through the players
        for(int i = 0; i <= 4; i++){
            // Get the first player's cards and add them to the vector
            int first_player_card = table1.getPlayer(i)->returnHand(0)->getValue();
            int second_player_card = table1.getPlayer(i)->returnHand(1)->getValue();
            combined_cards.push_back(first_player_card);
            combined_cards.push_back(second_player_card);

        }
    }
}

void setBigThree(Table& table1, int dealerRotator, int smallBlindRotator, int bigBlindRotator) {
    // If the rotators go out of scope, reset them to zero to allow for continual gameplay
    if (dealerRotator > 4) {
        dealerRotator = 0;
    } else if (smallBlindRotator > 4) {
        smallBlindRotator = 0;
    } else if (bigBlindRotator > 4) {
        bigBlindRotator = 0;
    }

    // Set the dealer, smallBlind, bigBlind
    table1.setDealer(table1.getPlayer(dealerRotator));
    table1.setSmallBlind(table1.getPlayer(smallBlindRotator));
    table1.setBigBlind(table1.getPlayer(bigBlindRotator));

    // Get the dealer, smallBlind, and bigBlind
    // table1->getDealer();     // players[0]
    // table1->getSmallBlind(); // players[1]
    // table1->getBigBlind();   // players[2]

    // Increment the dealerRotator for the next round
    dealerRotator++;
    smallBlindRotator++;
    bigBlindRotator++;
}


void refillDeck(Card** deck, Card** refill){
    for(int i = 0; i < 52; i++){
        refill[i] = deck[i];
    }
}
