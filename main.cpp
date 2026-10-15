/****************************************************************************
 * File:      main.cpp
 * Author:    Cameron Long
 * Purpose:   Complete Project. Texas Holdem
 * Version:   1.0 October 8, 2024
 * Resources: regex in c++
 *******************************************************************************/

#include <iostream>
#include <regex>
#include <vector>
#include <limits>
#include <algorithm>

#include "Player.h"
#include "Card.h"
#include "Table.h"
#include "funcs.h"
using namespace std;


int main() {
    string errorDataType = "";
    string continuePlay = "yes";
    bool potHandled = false;
    bool restart = false;
    bool incrementCommunity = false;

    int communityCardsCounter = 0;


    // Clear previous file contents
    ofstream file;
    file.open("testing_comp_output.txt", ofstream::out | ofstream::trunc);
    file.close();

    Table *table1 = (Table*) malloc( sizeof(Table));
    string username;
    cout << "Welcome to Texas Holdem! Buy in in $100" << endl;
    cout << "Please enter your name: ";
    cin >> username;

    // Exceptions
    regex validUsernameRegex("^[A-Za-z]+$");    // Checks username to make sure it is just alphabet characters and at least one character
    // Ask for username until legal username is provided
    while (!regex_match(username, validUsernameRegex)) {
        try {
            cin.clear();
            cin.ignore(100, '\n');
            throw "bad";
        } catch (...) {
            cout << "Invalid input. Please try again: ";
            cin >> username;
        }
    }

    // Initialize Players
    Player **players = new Player*[5];  // Create array to hold 5 player objects
    Player *playerptr = (Player*)malloc(sizeof(Player));
    // Loop through the array, create players and store them in the array
    for(int i = 0; i <= 3; i++){
        string tempName = "Comp" + to_string(i+1);
        players[i] = new Player(tempName, 100);
        // players[i]->
        cout << players[i]->getPlayerName() << " has joined the game." << endl;
    }
    // Player* players[5] = {player1ptr, Player*("Comp2", 100), Player("Comp3", 100), Player("Comp4", 100), Player(username)};
    players[4] = new Player(username, 100);
    cout << players[4]->getPlayerName() << " has joined the game." << endl;
    // Initialize the table(players, file)
    // Open the file with decisions and bets for reading
    
    fstream tableFile("testing_comp_output.txt");
    
    Table table(players, tableFile);
    *table1 = table;
    cout << endl;
    tableFile.close();

    // Initialize deck
    Card **deck = new Card*[52];
    string suits[4] = {"H", "C", "S", "D"};
    int card = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 13; j++) {
            deck[card] = new Card(j+1, suits[i]);
            // deck[card]->getCardInfo();
            // cout << endl;
            card++;
        }
    }
    Card **refill = new Card*[52];
    // Card communityCards[5];

    int rounds = 1, dealerRotator = 0, smallBlindRotator = 1, bigBlindRotator = 2;

    // Start the game loop
    if (continuePlay == "yes") {
        // Generate the community cards
        table1->generateCommunityCards(deck);
        int userBank = 100;
        // Deal hands if this is the start of each game
        bool dealHands = true;
        // Continue to play the game while the player has money
        while (userBank > 0 && continuePlay == "yes") {
            fstream myfile("testing_comp_output.txt");
            // shuffle the refill and Refill the deck for the next round
            // random_shuffle(begin(refill), end(refill));
            refillDeck(deck, refill);
            
            // Setup the dealer, smallBlind, and biglind
            setBigThree(table1, dealerRotator, smallBlindRotator, bigBlindRotator, myfile);


            // If we are restarting a game, reset the communityCardsCounter to 0
            if(restart == true){
                communityCardsCounter = 0;
                restart = false;
            }

            // Display the active players
            cout << endl << "Active players this round: " << endl;
            table1->getPlayers();

            // Set the pot to start at 0
            table1->setPot(0);

            // Deal cards to comps and write those hands to the file
            if(table1->getPlayer(4)->getFolded()){
                for(int i = 0; i < 4; i++){
                    table1->getPlayer(i)->setHand(deck);
                    table1->getPlayer(i)->writeHand(i, myfile);
                }
                table1->getPlayer(4)->setFolded(false);
                table1->getPlayer(4)->setHand(deck);
                table1->getPlayer(4)->writePlayerHand(username, myfile);
                // Output the community cards to the testing_community file
                myfile << "Community cards: ";
                myfile << endl << "Pot: $" << table1->getPot() << endl;
                myfile.close();
            }


            // Read lines from the file into the vector
            vector<string> lines = readIntoVector("testing_comp_output.txt");

            cout << endl;
            // Check to see if the community cards array is empty
            if (communityCardsCounter == 0) {
                cout << "Community Cards: None" << endl;
            } else {
                table1->getAllCommunityCards(communityCardsCounter, lines);
                cout << endl;
            }

            // Print out the pot value
            // cout << "Pot: $" << table1->getPot() << endl;



            /*
             *
             *
             * Setup gameplay
             * 
             */

            int actionLineNumber = 0, temp = 0, pot = 0, userBet = 0, previousBet = 0, tempIndex = 0;
            string userAction = "", betDecision = "";

            // Start the gameplay by displaying money and hand information to the player
            cout << "You currently have $" << table1->getPlayer(4)->getBalance() << endl;
            cout << "Your hand: ";
            table1->getPlayer(4)->getHand();
            // Start main loop where player takes actions
            if(!table1->getPlayer(4)->getFolded()){
                cout << "You can either bet, call or fold (bet, check, fold): ";
                cin >> betDecision;
                
                // Error handling for bad actions
                while (betDecision != "bet" && betDecision != "check" && betDecision != "fold") {
                    cout << "Invalid selection. Please try again (bet, check, fold): ";
                    cin >> betDecision;
                }

                // Handle the player actions
                if (betDecision == "bet") {
                    cout << "How much would you like to bet? ";
                    cin >> userBet;
                    // Set the player's bet for the round
                    table1->getPlayer(4)->setRoundBet(userBet);

                    // Error handling with bet amount
                    while (cin.fail()) {
                        cin.clear();
                        cin.ignore(100, '\n');
                        cout << "Invalid input. Please try again: ";
                        cin >> userBet;
                    }
                    // Set the userbet with the userBet function which does some error handling
                    table1->getPlayer(4)->userBet(userBet);
                    userAction = "bet";
                    table1->getPlayer(4)->setFolded(false);
                } else if (betDecision == "check") {
                    cout << "You checked." << endl;
                    userAction = "check";
                    userBet = 0;
                } else if (betDecision == "fold") {
                    userAction = "fold";
                    userBet = 0;
                    // Handle fold action
                    cout << "first bet action" << endl;
                    // Player loses money already bet, round must proceed without the player
                    table1->getPlayer(4)->handleFold();
                }
            }
            // Run function to get computer actions
            fstream myFile("testing_comp_output.txt");
            getHandRanks(table1, communityCardsCounter, myFile, userAction);
            myFile.close();

            // Setup a string vector to store the lines in the file
            lines = readIntoVector("testing_comp_output.txt");

            string actionLine = "Action for " + username;
            // Go through the vector
            for (string &text : lines) {
                // Increment a variable to represent the index
                temp++;
                // If a string in the vector contains the action for the player, set the index to the temp index
                if (text.find(actionLine) != string::npos) {
                    actionLineNumber = temp;
                }
            }

            // Get the string at the permanent index and modify it with the username, action, and bet amount

            string updatedUserAction = "";
            int updatedBet = 0;
            // Check to see if all users made check action
            if(table1->getCheckCounter() != 4 && userAction == "check"){
                // If the player decided to check, but all computers did not, ask for another action
                while(updatedUserAction != "bet" && updatedUserAction != "fold"){
                    cout << "All players did not check. You may either fold or bet: ";
                    cin >> updatedUserAction;
                    if(updatedUserAction == "bet"){
                        cout << "How much would you like to bet? ";
                        cin >> updatedBet;
                        players[4]->userBet(updatedBet);
                    } else if (updatedUserAction == "fold") {
                        // cout << "You have decided to fold." << endl;
                        table1->getPlayer(4)->handleFold();
                    } else {
                        cout << "Invalid option. Try again." << endl;
                    }
                }
            }
            cout << endl;

            // Writing player actions to file
            lines[actionLineNumber - 1] = "Action for " + username + ": " + userAction + ", bet: $" + to_string(userBet);
            table1->getPlayer(4)->setPrevAction(userAction);
            if(updatedUserAction == "fold"){
                table1->getPlayer(4)->setFolded(true);
            }
            // Value for the highest bet
            int high = 0, bet = 0, compLineIterator = 4;
            // Go through the vector
            

            // Get bets from the computers stored in the text file
            for(int i = 0; i <= 4; i++){
                regex rgx("\\$(.*)");
                smatch match;

                // regex grabs the bet amount and compares the bet to the highest bet
                if (regex_search(lines[compLineIterator], match, rgx)) {
                    bet = stoi(match[1]);
                    // This gets the highest bet and evens the pot
                    if (bet > high) {
                        high = bet;
                    }
                    pot += high;
                }
                // Makes the computers match the highest bet if they decided to bet this round
                if(!table1->getPlayer(i)->getFolded()){
                    table1->getPlayer(i)->setPrevBet(high);
                }
                // Skips over the hands designated for computer hands
                compLineIterator += 2;
            }
            table1->setPot(pot);

            // Writes actions for the computers to the text file
            compLineIterator = 4;
            for(int i = 0; i <= 3; i++){
                lines[compLineIterator] = "Action for Comp" + to_string(i+1) + ": bet, bet: $" + to_string(high);
                compLineIterator += 2;
            }
            // Asks the user to match the highest bet if the player decided to bet
            string updatedBetAction;
            if(userBet < high && !table1->getPlayer(4)->getFolded()){
                cout << "You must match the highest bet to stay in the round. Your bet was: $" << userBet << " and needs to be: $" << high << endl;
                cout << "Would you like to bet the difference? (yes, no): ";
                cin >> updatedBetAction;
                if(updatedBetAction == "yes"){
                    table1->getPlayer(4)->setBalance(table1->getPlayer(4)->getBalance() - (high - userBet));
                } else if(updatedBetAction == "no"){
                    table1->getPlayer(4)->handleFold();
                    restart = true;
                }
            }
            if(!restart){
                lines[14] = "Pot: $" + to_string(table1->getPot());
                lines[12] = "Action for " + username + ": " + userAction + ", bet: $" + to_string(userBet);

                // Write the modified lines back to the file
                fstream outputFile("testing_comp_output.txt");
                // If the file does not exist spit out an error
                if (outputFile) {
                    // cerr << "Could not open the file for writing: " << inFile << std::endl;
                    // Write the array to the file
                    for (const string l : lines) {
                        outputFile << l << endl;
                        // cout << l << endl;
                    }
                }
                // Close the file
                outputFile.close();

                // Increment the number of community cards displayed per round
                // If there is already 5 cards on the table, we want to start a new "game"
                string potWinnerName = "", potWinnerHandRank = "14", potWinnerHandColor = "";
                // Check to see if there are 5 cards, if there are figure out who won the pot
                // !NEEDS TO BE CHANGED. PLAYERS CAN TAKE ONE ADDITIONAL ACTION AFTER THE FIFTH CARD IS LAID DOWN!
                if (communityCardsCounter == 5) {
                    // Handle to see who wins the pot
                    for (int i = 0; i <= 3; i++) {
                        try{
                            cout << table1->getResults(i, 0) << " = " << table1->getResults(i, 1) << " | Hand Ranking: " << table1->getResults(i, 3) << " | Hand color category: " << table1->getResults(i, 4) << endl;
                            throw "bad";
                        } catch (...){
                            cout << "Something happened" << endl;
                        }
                        if (stoi(table1->getResults(i, 3)) < stoi(potWinnerHandRank)) {     
                            potWinnerName = table1->getResults(i, 0);
                            potWinnerHandRank = table1->getResults(i, 3);
                            potWinnerHandColor = table1->getResults(i, 4);
                        } else if (stoi(table1->getResults(i, 3)) == stoi(potWinnerHandRank)) {
                            if (stoi(table1->getResults(i, 4)) < stoi(potWinnerHandColor)) {
                                potWinnerName = table1->getResults(i, 0);
                                potWinnerHandRank = table1->getResults(i, 3);
                                potWinnerHandColor = table1->getResults(i, 4);
                            }
                        }
                    }
                    cout << "The pot winner is " << potWinnerName << endl;
                    potHandled = table1->handlePot(potWinnerName);
                }

                // Check to see if the user still has money, if they do they can play another round without restarting
                if (userBank > 0 && potHandled) {
                    cout << "Continue to play? (yes, no): ";
                    cin >> continuePlay;
                    restart = true;
                    potHandled = false;
                    dealHands = true;
                    if (continuePlay == "no") {
                        cout << "Thanks for playing!" << endl;
                    }
                }

                // Check to see if there are cards on the table, if there are no cards on the table and the player has not folded, the game has ended and needs to be reset
                if(communityCardsCounter == 0 && !table1->getPlayer(4)->getFolded()){
                    communityCardsCounter = 3;
                    incrementCommunity = true;
                } else if(incrementCommunity) {     // If cards are on the table and the player has not folded, add more cards to the table
                    communityCardsCounter++;
                } else if(table1->getPlayer(4)->getFolded()){       // If the player folds we want to set the card counter to 0 so that it gets caught on the next loop
                    communityCardsCounter = 0;
                    table1->getPlayer(4)->setFolded(true);
                }

                // This sets the previous bet so that we can use it for stuff later
                table1->getPlayer(4)->setPrevBet(userBet);
            }
        }
        dealerRotator++;
        smallBlindRotator++;
        bigBlindRotator++;
    }
}
