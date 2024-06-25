//
// Created by Jackson Burns and Jose Juan Velasquez on 9/30/2023.
//
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>
using namespace std;

//The structure that will hold the card nodes
typedef struct cards {
    char suit;
    int face;
    struct cards* next;
} cards;

//The structure in charge of the root node of a tree.
typedef struct card_deck{
    cards* root; //Will be the pointer pointing to the root node of a tree.
    int size;
}card_deck;


//Function will allocate memory for the root of the Linked List containing the nodes
//It will return a pointer to the root of the Linked List
card_deck* allocateDeck(){
    auto* list = (card_deck*)calloc(1, sizeof(card_deck));
    return list;
}

//Function will allocate memory for each individual node representing one card of the standard 52 card deck
//Will return the allocated node containing the corresponding number/symbol and suit
cards* cards_node_create(int face, char suit){
    auto* node = (cards*)calloc(1,sizeof(cards));
    node->suit = suit;
    node->face = face;
    node->next = nullptr;
    return node;
}

//Function will de-allocate memory of each node recursively once the round or game ends
void clearCard(cards* card){
    if(card== nullptr){
        free(card);
        return;
    } else{
        clearCard(card->next);
    }
    free(card);
}

//Function will free the allocated memory from the root pointing to the Linked List
//Will call clearCard function to recurse through all the nodes and free them one by one
void clearDeck(card_deck* deck){
    if(deck->root != nullptr){
        clearCard(deck->root);
    }
    free(deck);
}

//Function will add the cards into the linked list recursively
void cards_add(cards* node, int face, char suit){
    if(node->next != nullptr){
        cards_add(node->next, face, suit);
    } else {
        cards* newNode = cards_node_create(face,suit);
        node->next = newNode;
    }
}

//Function will create a root node if the list is empty or call
//cards_add to add the card nodes into the linked list.
void LL_add_cards(card_deck * tree, int face, char suit){
    if(tree->root == nullptr){
        tree->root = cards_node_create(face,suit);
        tree->size++;
    } else{
        cards_add(tree->root,face,suit);
        tree->size++;
    }
}

//TESTING function used to verify that all cards are made correctly
void print_deck(cards* node){
    if(node->next != nullptr){
        cout << node->face << node->suit << " ";
        print_deck(node->next);
    }else{
        cout<<endl;
    }
}

//Function will call LL_add_cards with parameters being passed within this for loop
void createDeck(card_deck* deck){
    for (int suit = 3; suit <= 6; suit++) {
        for (int face = 1; face <= 13; face++) {
            LL_add_cards(deck ,face,suit);
        }
    }
}

//Function will swap cards based on what the shuffle function
void SwapCards(cards *cardOne, cards *cardTwo) {
    cards temp;
    temp.suit = cardOne->suit;
    temp.face = cardOne->face;

    cardOne->suit = cardTwo->suit;
    cardOne->face = cardTwo->face;
    cardTwo->suit = temp.suit;
    cardTwo->face = temp.face;
}

//This function will shuffle the deck of cards in a random order each time the game starts
//or if a new round starts.
void shuffleDeck(cards *deck) {
    //Create pointers to go through the deck of cards
    cards* card1, * card2;
    srand( time(nullptr));

    //Will shuffle all the 52 cards
    for (int i = 0; i < 52; i++){

        //Will find first card to shuffle
        int randCard1 = rand() % 52;
        card1 = deck;
        for (int j = 0; j < randCard1; j++) {
            card1 = card1->next;
        }

        //Will find second card to shuffle
        int randCard2 = rand() % 52;
        card2 = deck;
        for (int k = 0; k < randCard2; k++) {
            card2 = card2->next;
        }

        //Once the cards are randomly selected, swap them
        SwapCards(card1, card2);
    }
}

//Class will hold the current player objects
class Player{
    card_deck* playerDeck;
    card_deck* theCards;
    int sum;
    int money;
    bool firstDeal;

public:
    //Constructor method will populate and receive the deck of cards and
    //add that to this class so that the cards are in sync with the players.
    explicit Player(card_deck* obj){
        this->playerDeck = allocateDeck();
        this->theCards = obj;
        this->sum = 0;
        this->money = 100;
        this->firstDeal = true;
    }

    //Method will deal two initial cards to the players with certain conditions
    void dealCards() {
        cards* temp = theCards->root;

        //Deal two cards ti tge player
        for(int i = 0; i < 2; i++) {
            //If face is either a King, Queen or Jack, only add +10 to the sum
            if(temp->face > 10){
                this->sum+=10;
            } else{
                this->sum+=temp->face;
            }
            LL_add_cards(this->playerDeck, temp->face, temp->suit);
            temp = temp->next;
        }

        //If the sum of the two cards is greater than 21, dispose of old hand and give user new cards
        if(this->sum > 21 && this->firstDeal){
            this->playerDeck->root = this->playerDeck->root->next->next;
            temp = theCards->root;
            for(int i = 0; i < 2; i++) {
                if(temp->face > 10){
                    this->sum+=10;
                } else{
                    this->sum+=temp->face;
                }
                LL_add_cards(this->playerDeck, temp->face, temp->suit);
                temp = temp->next;
            }
        }
        //Have temp point to the current head of the linked list
        temp = this->theCards->root;
        //Move the head pointer to its next element
        this->theCards->root = this->theCards->root->next;
        //Free from the deck the card that was dealt
        free(temp);

        temp = this->theCards->root;
        this->theCards->root = this->theCards->root->next;
        free(temp);
        this->firstDeal=false;
    }

    //Method will deal one card to either player each time it is called
    void addOneCard(){
        cards *temp = this->theCards->root;
        if(temp->face > 10){
            this->sum+=10;
        } else{
            this->sum+=temp->face;
        }
        LL_add_cards(this->playerDeck, temp->face, temp->suit);
        temp = this->theCards->root;
        this->theCards->root = this->theCards->root->next;
        free(temp);
    }

    //A set of functions that will print out the card layouts for better HCI
    void printCardsTop(cards* temp){
        for (int j = 0; j < this->playerDeck->size; j++) {
            if (temp->face == 11) {//If the face value is a 0eleven, print the Jack symbol and its
                // corresponding suit
                cout << "|J"<<temp->suit << "       |\t";
            }
            else if (temp->face == 12) {//If the face value is a twelve, print the Queen symbol and its corresponding suit
                cout << "|Q"<<temp->suit << "       |\t";
            }
            else if (temp->face == 13) {//If the face value is a thirteen, print the King symbol and its corresponding suit
                cout << "|K"<<temp->suit << "       |\t";
            }
            else if (temp->face == 1) {//If the face value is a one, print the Ace symbol and its corresponding suit
                cout << "|A"<<temp->suit << "       |\t";
            }
            else if (temp->face == 10) {//If the face value is a ten, print the number ten with its adjusted spaces and its corresponding suit
                cout << "|"<<temp->face<<temp->suit << "      |\t";
            }
            else {//If the face value is between two and nine, print the numbers with their adjusted
                // spaces and its corresponding suit
                cout << "|"<<temp->face<<temp->suit << "       |\t";
            }
            //After loop makes one cycle, make the list (temp) point to the next card
            temp = temp->next;
        }
        cout << endl;
    }

    void printCardsBottom(cards* temp2){
        //Open for loop for the last part of the cards with the face and suit
        for (int j = 0; j < this->playerDeck->size; j++) {

            if (temp2->face == 11) {//If the face value is an eleven, print the Jack symbol and its corresponding suit
                cout<< "|_______J"<< temp2->suit<<"|\t";
            }
            else if (temp2->face == 12) {//If the face value is a twelve, print the Queen symbol and its corresponding suit
                cout<<"|_______Q"<< temp2->suit<<"|\t";
            }
            else if (temp2->face == 13) {//If the face value is a thirteen, print the King symbol and its corresponding suit
                cout<<"|_______K"<< temp2->suit<<"|\t";
            }
            else if (temp2->face == 1) {//If the face value is a one, print the Ace symbol and its corresponding suit
                cout<<"|_______A"<< temp2->suit<<"|\t";
            }
            else if (temp2->face == 10) {//If the face value is a ten, print the number ten with its adjusted spaces and its corresponding suit
                cout<<"|______" <<temp2->face<<temp2->suit<<"|\t";
            }
            else {//If the face value is between two and nine, print the numbers with their adjusted spaces and its corresponding suit
                cout<<"|_______"<< temp2->face<< temp2->suit<<"|\t";
            }
            //Make the third pointer (temp2) point to the next card
            temp2 = temp2->next;
        }
        cout << endl;
    }

    //Method will print the body of the card
    void printLines(){
        for (int j = 0; j < this->playerDeck->size; j++) {
            cout << "|         |\t";
        }
        cout << endl;
    }

    //Main printer method that will print out the cards
    void printCards() {
        cards *temp = this->playerDeck->root;
        for (int j = 0; j < this->playerDeck->size; j++) {
            cout << " _________\t";
        }
        cout << endl;
        printCardsTop(temp);
        printLines();
        printLines();
        temp = this->playerDeck->root;
        for (int j = 0; j < this->playerDeck->size; j++) {
            cout << "|    " << temp->suit << "    |\t";
            temp = temp->next;
        }
        cout << endl;
        printLines();
        printLines();
        temp = this->playerDeck->root;
        printCardsBottom(temp);
    }

    int getSum() const {
        return this->sum;
    }

    int getMoney() const {
        return this->money;
    }

    //bets money. if there is enough money, the bet will be returned.
    // if not enough money, it will return -1
    int bet(int val){
        if(this->money<val){
            return -1;
        }
        else{
            this->money-=val;
            return val;
        }
    }

    void addMoney(int val){
        this->money+=val;
    }

    //After each round ends, this method will de-allocate the cards from the deck
    //and allocate new ones for the next round
    void resetPlayer(card_deck * deck){
        clearDeck(playerDeck);
        this->playerDeck = allocateDeck();
        this->sum = 0;
        this->theCards = deck;
    }

    //Method will de-allocate players after the user decides to end the game
    void clearPlayer() {
        clearDeck(playerDeck);
    }
};

int main() {
    srand(time(nullptr));

    //Create deck of cards and shuffle them
    card_deck *deck = allocateDeck();
    createDeck(deck);
    shuffleDeck(deck->root);
    //print_deck(deck->root);

    //Create user and computer player objects
    Player user(deck);
    Player comp(deck);
    string str;
    cout<<"********** Welcome to Blackjack! **********"<<endl;
    string bet;
    //main program loop
    bool firstRun = true;
    while(true){
        if(!firstRun){
            clearDeck(deck);
            deck = allocateDeck();
            createDeck(deck);
            shuffleDeck(deck->root);
            user.resetPlayer(deck);
            comp.resetPlayer(deck);
        }
        cout<<"***** You have "<<user.getMoney()<<" coins. *****"<<endl;
        cout<<"***** Enter number to bet. *****"<<endl;
        cin>>bet;
        while(user.bet(stoi(bet))==-1){
            cout<<"***** You only have "<<user.getMoney()<<" coins to bet, try again. *****"<<endl;
            cin>>bet;
        }
        cout<<"** You have bet " <<bet<< " coins. **";

        cout << "\n\n* giving cards to user... *\n";
        user.dealCards(); //give 2 cards
        user.printCards();

        cout << "\nuser current sum = " << user.getSum() << endl;

        cout <<"\n* giving cards to computer... *\n";
        comp.dealCards(); //give 2 cards

        while(true){
            cout<<"hit(h/H) or stay(s/S)"<<endl;
            cin>>str;
            while(str!="h"&&str!="H"&&str!="s"&&str!="S"){
                cout<<"invalid input, try again"<<endl;
                cin>>str;
            }
            if(str=="h"||str=="H"){
                cout <<"\n* giving user one card... *\n";
                user.addOneCard();
                user.printCards();
                cout << "user current sum = " << user.getSum() << endl;
                if(user.getSum()>21){
                    cout<<"*** overdrew, you lose "<<bet<<" coins ***"<<endl;
                    if(user.getMoney() == 0){
                        cout<<"*** YOU LOST ALL YOUR COINS. GOODBYE ***"<<endl;
                        return 0;
                    }
                    break;
                }else if (user.getSum()==21){ //exactly 21
                    cout << "Computer's hand:\n";
                    comp.printCards();
                    cout << "computer's final sum = " << comp.getSum() << endl;
                    cout<<"*** You win "<<bet<< " coins! ***"<<endl;
                    user.addMoney(2*(stoi(bet)));
                    //firstRun=false;
                    break;
                }
                else{
                    continue;
                }
            }
            else { //stay
                while(comp.getSum()<17){
                    cout<<"* computer is drawing *"<<endl;
                    comp.addOneCard();
                }
                //win condition
                if(comp.getSum()>21||comp.getSum()<user.getSum()){
                    cout << "Computer's hand:\n";
                    comp.printCards();
                    cout << "computer's final sum = " << comp.getSum() << endl;
                    cout<<"*** YOU WIN "<<bet<<" COINS ***"<<endl;
                    user.addMoney(2*stoi(bet));
                }
                else if (comp.getSum()==user.getSum()){
                    cout << "Computer's hand:\n";
                    comp.printCards();
                    cout << "computer's final sum = " << comp.getSum() << endl;
                    cout<<"*** TIE. returning your money ***"<<endl;
                    user.addMoney(stoi(bet));
                }
                else{
                    cout << "Computer's hand:\n";
                    comp.printCards();
                    cout << "computer's final sum = " << comp.getSum() << endl;
                    cout<<"*** YOU LOST "<<bet<<" COINS ***"<<endl;
                    if(user.getMoney() == 0){
                        cout<<"*** YOU LOST ALL YOUR COINS. GOODBYE ***"<<endl;
                        return 0;
                    }
                }
                break;
            }

        }
        cout<<"play again? (y/n)"<<endl;
        cin>>str;
        while(str!="n"&&str!="N"&&str!="y"&&str!="Y"){
            cout<<"You entered an invalid option";
            cin>>str;
        }
        if(str=="n"||str=="N"){
            cout<<"***** Goodbye. *****"<<endl;
            clearDeck(deck);
            user.clearPlayer();
            comp.clearPlayer();
            return 0;
        }
        else{
            firstRun=false;
            continue;
        }
    }

}