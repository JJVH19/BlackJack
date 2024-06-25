# BlackJack

## Authors: Jose Juan Velasquez and Jackson Burns

This program is a simplified version of the popular card game, BlackJack.

## Contents:
* A standard 52 deck of cards
* A user
* The computer simulating the house.

## How it works:
* This program asks for user input at the beginning of the game to see how many coins they would like to start playing
* Each player is given 100 coins by default every time the game starts
* After the bet is done, the program will deal two cards to the user and computer from the already generated and shuffled deck
* A message will pop up saying what the current sum of the cards is and an option to either stay with your current hand
or receive one more random card. 
* Based on the sum of cards between the user and computer players, if one sum is greater than the other, that player with
the highest sum wins the amount of coins they bet at the beginning and will be asked to play again.
* If user says yes, the game will start another round, reshuffling the cards and dealing them to the players
* If user says no, the game will end.
* If user loses all their coins, the game automatically ends and closes the program.

## How testing was done:
* We had a method to print out all the cards from the deck to verify if each card was done correctly.
* We had a simulation in which we only tested if the coins were distributed correctly based on the result of the game.
* We used valgrind to test out if any memory leaks were present.
* We used print statements to verify of each card was removed from the deck to be dealt to the players by printing the deck again and verify the card was no longer in the deck, but within the player's current hand.

### Note: This program was intended to be used with ASCII characters for a better visual experience
