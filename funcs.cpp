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

void setupCompBets(Table *table1, string action, string userAction){
    // Set up comp bets
    // Uses i from the getHandRanks function
    int tempPotValue = 0;
    for(int i = 0; i <= 3; i++){
        table1->setResults(i, 0, ("Comp" + to_string(i+1)));
        table1->setResults(i, 2, to_string(0));
        if(action == "bet"){
            // Generate a random bet
            int tempCompBet = 15 + rand() % (20 - 15 + 1), newBalance = 0, currentBalance = 0;
            // Set the action to bet
            table1->setResults(i, 1, "bet");

            for(int j = 0; j <= 3; j++){
                if(table1->getResults(i, 0) == table1->getPlayer(j)->getPlayerName()){
                    currentBalance = table1->getPlayer(j)->getBalance();

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
                    table1->getPlayer(j)->setBalance(newBalance);
                }
            }
            table1->setResults(i,2, to_string(tempCompBet));
        } else if (action == "fold"){
            table1->setResults(i, 1, "fold");
            table1->setResults(i, 2, to_string(0));
        } else if (action == "check"){
            table1->setResults(i, 1, "check");
            table1->setResults(i, 2, to_string(0));
        }
        tempPotValue += stoi(table1->getResults(i, 2));
        table1->setPot(tempPotValue);
    }


    vector<string> lines = readIntoVector("testing_comp_output.txt");
    int compIterator = 1, lineCounter = 0, betCounter = 0;

    lines[4] = "Action for Comp1: " + table1->getResults(0,1) + ", bet: $" + table1->getResults(0,2);
    lines[6] = "Action for Comp2: " + table1->getResults(1,1) + ", bet: $"  + table1->getResults(1,2);
    lines[8] = "Action for Comp3: " + table1->getResults(2,1) + ", bet: $" + table1->getResults(2,2);
    lines[10] = "Action for Comp4: " + table1->getResults(3,1) + ", bet: $" + table1->getResults(3,2);
    lines[14] = "Pot: $" + to_string(table1->getPot());

    // Write the computer actions to the file
    ofstream writeFile("testing_comp_output.txt");
    for (const string modifiedLine : lines) {
        writeFile << modifiedLine << endl;
    }
    writeFile.close();

    for(int i = 0; i <= 3; i++){
        cout << table1->getResults(i, 0) << " has decided to " << table1->getResults(i, 1) << endl;
    }
}


void getHandRanks(Table *table1, int numCommunityCards, fstream &myFile, string userAction){
    // 0 denotes a ten
    // B is 66
    // G is 71
    // Y is 89
    // R is 82
    // format = {card, card, ranking, color} color denotes playability
    int playableHands[91][4] = {{14, 14, 1, 66}, {14, 13, 1, 66}, {13, 12, 1, 66}, {12, 11, 1, 66}, {11, 10, 1, 66},
                                {10, 9, 1, 66}, {13, 13, 2, 66}, {14, 12, 2, 66}, {13, 11, 2, 66}, {12, 10, 2, 66},
                                {11, 9, 2, 66}, {12, 12, 3, 66}, {14, 11, 3, 66}, {13, 10, 3, 66}, {11, 11, 4, 66},
                                {14, 10, 4, 66}, {9, 9, 5, 66}, {8, 8, 6, 66}, {7, 7, 7, 66}, {14, 10, 8, 66},
                                {9, 8, 1, 71}, {10, 8, 2, 71}, {11, 8, 3, 71}, {12, 9, 3, 71}, {12, 8, 4, 71},
                                {13, 9, 4, 71}, {14, 9, 5, 71}, {14, 8, 6, 71}, {14, 7, 7, 71}, {14, 6, 8, 71},
                                {6, 6, 9, 71}, {5, 5, 10, 71}, {8, 7, 1, 89}, {7, 6, 1, 89}, {6, 5, 1, 89}, 
                                {5, 4, 1, 89}, {9, 7, 2, 89}, {8, 6, 2, 89}, {7, 5, 2, 89}, {10, 7, 3, 89}, 
                                {9, 6, 3, 89}, {11, 7, 4, 89}, {13, 8, 5, 89}, {13, 7, 6, 89}, {13, 6, 7, 89}, 
                                {13, 5, 8, 89}, {13, 4, 9, 89}, {14, 5, 9, 89}, {14, 4, 10, 89}, {13, 3, 10, 89}, 
                                {4, 4, 11, 89}, {14, 3, 11, 89}, {13, 2, 12, 89}, {3, 3, 11, 89}, {14, 2, 12, 89}, 
                                {2, 2, 13, 89}, {4, 3, 1, 92}, {3, 2, 1, 92}, {6, 4, 2, 92}, {5, 3, 2, 92}, 
                                {4, 2, 2, 92}, {8, 5, 3, 92}, {7, 4, 3, 92}, {6, 3, 3, 92}, {5, 2, 3, 92}, 
                                {10, 6, 4, 92}, {9, 5, 4, 92}, {8, 4, 4, 92}, {7, 3, 4, 92}, {6, 2, 4, 92}, 
                                {12, 7, 5, 92}, {11, 6, 5, 92}, {10, 5, 5, 92}, {9, 4, 5, 92}, {8, 3, 5, 92}, 
                                {7, 2, 5, 92}, {12, 6, 6, 92}, {11, 5, 6, 92}, {10, 4, 6, 92}, {9, 3, 6, 92}, 
                                {8, 2, 6, 92}, {12, 5, 7, 92}, {11, 4, 7, 92}, {10, 3, 7, 92}, {9, 2, 7, 92}, 
                                {12, 4, 8, 92}, {11, 3, 8, 92}, {10, 2, 8, 92}, {12, 3, 9, 92}, {11, 2, 9, 92}, 
                                {12, 2, 10, 92}};
    // cout << endl << "testing..." << endl;

    // Create an 2D array that is 5 rows for the 5 players, and two columns for each of the cards in their hand
    // Use the table to grab all the active players, get their cards, and store them in this array so we can
    // iterate through it
    Card *playerHands[5][2] = {
        {table1->getPlayer(0)->returnHand(0), table1->getPlayer(0)->returnHand(1)}, 
        {table1->getPlayer(1)->returnHand(0), table1->getPlayer(1)->returnHand(1)}, 
        {table1->getPlayer(2)->returnHand(0), table1->getPlayer(2)->returnHand(1)}, 
        {table1->getPlayer(3)->returnHand(0), table1->getPlayer(3)->returnHand(1)}, 
        {table1->getPlayer(4)->returnHand(0), table1->getPlayer(4)->returnHand(1)}
    };

    // Initialize a pair cointer to designate how many pairs a player has
    int pairCounter = 0;
    // Initialize another 2D array to store player and how many pairs they have
    string results[3][6];
    // results = [{name, action, betAmount, rank, color}]
    // cout << endl << "Testing community cards" << endl;
    // This first loop is used to go through the array of players

    // Evaluate just the players hands with no community cards
    int pairFound = 0, isPlayable = 0;
    string action = "";

    // Add the comp name and action to results to see if they folded in the previous round
    vector<string> newLines = readIntoVector("testing_comp_output.txt");

    // Test to see if the player or comp folded in the last round
    int fileLineIterator = 4;
    for(int i = 0; i <= 3; i++){
        // cout << "setResults function" << endl;
        // cout << "newLines[4].substr = " << newLines[fileLineIterator].substr(18,3) << endl;
        table1->setResults(i, 0, newLines[fileLineIterator].substr(11,5));
        table1->setResults(i, 1, newLines[fileLineIterator].substr(18,3));
        fileLineIterator+=2;
        if(table1->getResults(i, 1) == "fold"){
            cout << table1->getResults(i, 0) << " will not be active this round." << endl;
        }
    }

    /*
     * B is 66
     * G is 71
     * Y is 89
     * R is 92
     * This piece of code checks the comp hands against the playableHands array to see whether or not the comp should bet, check, or fold
     * */

    // results = [{name, action, betAmount, rank, color, handType}]
    // playableHands = [{cardValue, cardValue, rank, color}]
    srand(time(0));
    int betOrCheck = -1;

    // Loop through the computers
    for(int i = 0; i <= 3; i++){
        
        // Loop through the playable hands array
        for(int j = 0; j < 91; j++){

            // Look to see if the player hand exists in the array at all
            if((playerHands[i][0]->getValue() == playableHands[j][0] && playerHands[i][1]->getValue() == playableHands[j][1]) || (playerHands[i][0]->getValue() == playableHands[j][1] && playerHands[i][1]->getValue() == playableHands[j][0])){
                
                // If the hand does exist in the array, and the color ranking is Blue, the comp should bet
                // Blue is the best hand to have so the comps will always bet
                if(playableHands[j][3] == 66){
                    table1->setResults(i, 3, to_string(playableHands[j][2]));
                    table1->setResults(i, 4, to_string(playableHands[j][3]));
                    action = "bet";
                    isPlayable = 1;
                
                // If the color is Green, the hand is playable to an extent, so a "coin" is flipped
                // to decide whether or not the comp bets or checks. This is to give a somewhat human feel
                } else if(playableHands[j][3] == 71){
                    table1->setResults(i, 3, to_string(playableHands[j][2]));
                    table1->setResults(i, 4, to_string(playableHands[j][3]));
                    betOrCheck = rand() % 1 + 1;
                    if(betOrCheck == 0){
                        action = "check";
                        table1->incrementCheckCounter();
                    } else {
                        action = "bet";
                    }
                    isPlayable = 1;
                
                // 89 is Yellow and is somewhat playable, so the code does the coin flip again
                } else if(playableHands[j][3] == 89){
                    table1->setResults(i, 3, to_string(playableHands[j][2]));
                    table1->setResults(i, 4, to_string(playableHands[j][3]));
                    if(betOrCheck == 0){
                        action = "check";
                        table1->incrementCheckCounter();
                    } else {
                        action = "bet";
                    }
                    isPlayable = 1;

                // If the color is Red, the hand is not playable and the computer folds
                } else if(playableHands[j][3] == 92){
                    table1->setResults(i, 3, to_string(playableHands[j][2]));
                    table1->setResults(i, 4, to_string(playableHands[j][3]));
                    action = "fold";
                    table1->getPlayer(i)->handleFold();
                    isPlayable = 0;
                }
            }
        }


        /*
        * This piece of code tests for pairs in the players hand if no community cards are present
        * and in the community cards if there is cards present
        * */

        if(numCommunityCards == 0){
            if(playerHands[i][0]->getValue() == playerHands[i][1]->getValue() || playerHands[i][0]->getValue() == playerHands[i][1]->getValue()){
                pairCounter++;
                action = "bet";
                results[i][5] += " pair";
            }
        } else {
            for(int j = 0; j < numCommunityCards; j++){
                // If there is a match, print it to the screen and add one to the pair counter
                if(table1->getCommunityCard(j)->getValue() == playerHands[i][0]->getValue() || table1->getCommunityCard(j)->getValue() == playerHands[i][1]->getValue()){
                    pairCounter++;
                    action = "bet";
                    results[i][5] += " pair";
                }
                if(playerHands[i][0]->getValue() == playerHands[i][1]->getValue() || playerHands[i][0]->getValue() == playerHands[i][1]->getValue()){
                    pairCounter++;
                    action = "bet";
                    results[i][5] += " pair";
                }
            }
        }

        /*
        * This piece of code tests for straights present
        * */

        // Test for straight
        Card **straightHolder = new Card*[7];
        int straightCounter = 0;

        // There must be at least 3 community cards for a straight to exist
        if(numCommunityCards >= 3){
            int holderLooper = numCommunityCards + 2;
            straightHolder[0] = new Card(table1->getPlayer(i)->returnHand(0)->getValue(), table1->getPlayer(i)->returnHand(0)->getSuit());
            straightHolder[1] = new Card(table1->getPlayer(i)->returnHand(1)->getValue(), table1->getPlayer(i)->returnHand(1)->getSuit());
            // Loop through straightHolder array
            for(int j = 2; j < holderLooper; j++){
                // Get the first hand card, if it is within + or - 1 of a community card, increment a straight counter
                // If the straight counter reaches 5, the player has a straight
                // When the user has more than one card, the code must test +1 on the last card, or -1 on the first card
                straightHolder[j] = new Card(table1->getCommunityCard(j-2)->getValue(), table1->getCommunityCard(j-2)->getSuit());
            }

            // Sort the array so that we can test for straights
            customSort(straightHolder, holderLooper);

            for(int i = 0; i < holderLooper; i++){
                for(int j = 0; j < holderLooper - 1; j++){
                    if((straightHolder[j]->getValue() == straightHolder[j + 1]->getValue() + 1) || (straightHolder[j]->getValue() == straightHolder[j + 1]->getValue() + - 1)){
                        straightCounter++;
                    }
                }
            }

            if(straightCounter >= 5){
                results[i][5] += " straight";
            }
        }
    }
    setupCompBets(table1, action, userAction);
}

void setBigThree(Table *table1, int dealerRotator, int smallBlindRotator, int bigBlindRotator, fstream &myFile) {
    // If the rotators go out of scope, reset them to zero to allow for continual gameplay
    if (dealerRotator > 4) {
        dealerRotator = 0;
    } else if (smallBlindRotator > 4) {
        smallBlindRotator = 0;
    } else if (bigBlindRotator > 4) {
        bigBlindRotator = 0;
    }

    // Set the dealer, smallBlind, bigBlind
    table1->setDealer(table1->getPlayer(dealerRotator), myFile);
    table1->setSmallBlind(table1->getPlayer(smallBlindRotator), myFile);
    table1->setBigBlind(table1->getPlayer(bigBlindRotator), myFile);

    // Get the dealer, smallBlind, and bigBlind
    table1->getDealer();     // players[0]
    table1->getSmallBlind(); // players[1]
    table1->getBigBlind();   // players[2]

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
