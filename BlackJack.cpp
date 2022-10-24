/*
=====================================================
Name        :BlackJack.cpp
Author      :Sean WANG 
Description :BlackJack basic game in C++
=====================================================
*/

#include <iostream>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <ctime>

using namespace std;

class Card{
    public:
        //enum for Rank is numerated
        enum Rank {ACE=1, TWO, THREE, FOUR, FIVE, SIX, SEVEN, EIGHT, NINE, TEN, JACK, QUEEN, KING};
        enum Type {CLUBS, DIAMONDS, HEARTS, SPADES};

        Card(Rank rank, Type type){
            this->myRank=rank;
            this->myType=type;
        }
        //all methods that can be used by the user, hence public
        
        //getValue returns an integer since the enum Rank is numbered from 1 to 10
        int getValue()
        {
            //return 11 for ace, since that's what benefits the user at first if he only has one ace card
            if (this->myRank==1){
                return 11;
            }else if (this->myRank<=TEN){
                return this->myRank;
            }else{
                return 10;
            }
            
        }
        //displayCard prints out to screen as a string the first letter of the Type and the numerical value 
        //of the rank if it's not a Face card
        void displayCard()
        {
            //DIVIDE THE DISPLAYING OF CARD IN TWO STEPS, FIRST FIND THE RANK
            //IF it's <=10, then you can print directly its number
            //IF it's >10, print out the first letter of the type manually for each condition
            //1.Check the Rank
            if (this->myRank<=10){
                cout<<this->myRank;
            }else if (this->myRank==QUEEN){
                cout<<"Q";
            }else if (this->myRank==JACK){
                cout<<"J";
            }else if (this->myRank==KING){
                cout<<"K";
            }
            //2. Check the type
            if (this->myType==CLUBS){
                    cout<<"C ";                
                }else if (this->myType==SPADES){
                    cout<<"S ";

                }else if (this->myType==DIAMONDS){
                    cout<<"D ";

                }else if (this->myType==HEARTS){
                    cout<<"H ";
                } 
        }
    //encapsulate rank and type of the cards from the user
    private:
    Rank myRank;
    Type myType;

};


    class Hand{
    public:
        Hand(){
            //using vectors to keep track of hand
            vector<Card> h1;
            this->myHand=h1; 
        }
        //using push_back method inside vector to add at the end of the vector the card specified
        void add(Card c1){
            this->myHand.push_back(c1);
        }
        //using clear method from vector to clear the vector of all Card objects
        void clear(){
           this->myHand.clear();
        }
        void display(){
        for (int i=0; i<this->myHand.size();i++){
            this->myHand.at(i).displayCard();
            
        }
        cout<<"end of hand"<<endl;
    }
        
        //By forlooping once, we count the number of aces we have in our hand
        //Then we calculate the total number of points we have
        //As long as we have more than 21 points, and at least 1 Ace, we can keep taking away 10 points from
        //the total since an Ace is either worth 11 or 1. 
        //getValue() gives 11 if it finds an ACE card
        int getTotal(){
            int sum=0;
            int aceNumbers=0;
            for (int i=0; i<this->myHand.size();i++){
                //this->myHand.at(i).displayCard();
                if (this->myHand.at(i).getValue()==11){
                    aceNumbers++;
                }
                sum+=this->myHand.at(i).getValue();
            }

            while (sum>21 && aceNumbers>0){
                sum-=10;
                aceNumbers--;
            }
            return sum;
        } 
    
    
        vector<Card> myHand;
};

class Deck {
    public:
        Deck(){
            vector<Card> d1; 
            this->myDeck=d1;
        }
    void populate(){
        for (int rank=Card::ACE;rank<=Card::KING;rank++){
            for (int type= Card::CLUBS; type<=Card::SPADES; type++){
                this->add(Card(Card::Rank(rank),Card::Type(type)));
            }
        }
    }
    //Since we are using vector implementation, we can use algorithm
    //using algorithm class's random_shuffle method to shuffle elements inside vector 
    void shuffle(){
        random_shuffle(this->myDeck.begin(),this->myDeck.end());
    }
    void display(){
        for (int i=0; i<this->myDeck.size();i++){
            this->myDeck.at(i).displayCard();
            
        }
        cout<<"end of deck"<<endl;
    }
    void deal(Hand &hand){
        Card dealtCard= this->myDeck.back();
        hand.add(dealtCard);
        this->myDeck.pop_back();
    }
    
    void add(Card c1){
        this->myDeck.push_back(c1);
    }
    private:
        vector<Card> myDeck;


};

class AbstractPlayer{
    public:
        AbstractPlayer():
            myHand(new Hand){
        }

        virtual ~AbstractPlayer(){
            delete myHand;
        }    
    virtual bool isDrawing() const= 0;

    bool isBusted(){
        return myHand->getTotal()>21;
    }
    Hand* getHand(){
            return this->myHand;
        }
    protected:
        Hand *myHand;
};

class HumanPlayer: public AbstractPlayer{
    public:
        HumanPlayer(){   
        }
        virtual bool isDrawing() const{
            char answer;
            cout << "Do you want to draw? (y/n): ";
            cin >> answer;
            return (answer == 'y' ? true : false);

        }
        //announce takes in the casino's score as an input since it can have access to the player's score already
        //by doing this, we can have a comparison between two scores
        void announce(int casinoScore){
            if (casinoScore>21){
                cout<<"Player wins."<<endl;
            }
            else if (casinoScore>this->getHand()->getTotal()){
                cout<<"Casino wins."<<endl;

            }else if( casinoScore<this->getHand()->getTotal() && this->getHand()->getTotal()< 22){
                cout<<"Player wins."<<endl;
            }else {
                cout<<"Push: No one wins."<<endl;
            }
        }

};
class ComputerPlayer: public AbstractPlayer{
    public:
        ComputerPlayer(){
        }

        virtual bool isDrawing() const{
            return (this->myHand->getTotal() <=16 ? true: false);
        }
        

};

class BlackJackGame{
    public:
        BlackJackGame(){
            Deck deck= Deck();
            this->m_deck=deck;

            ComputerPlayer casino= ComputerPlayer();
            HumanPlayer player = HumanPlayer();
            this->m_casino= casino;
            this->m_player=player;
        }
        void play(){
            char answer;
            int sumCasino=0;
            //create the deck of 52 cards
            m_deck.populate();
            m_deck.shuffle();

            //clear hand of both players
            m_player.getHand()->clear();
            m_casino.getHand()->clear();

            //deal to player 2 cards
            m_deck.deal(*(m_player.getHand()));
            m_deck.deal(*(m_player.getHand()));

            //deal a card to casino
            m_deck.deal(*(m_casino.getHand()));
            
            //Display what the casino has
            cout<<"Casino: ";
            m_casino.getHand()->myHand.at(0).displayCard();
            cout<< "["<<m_casino.getHand()->getTotal()<<"]"<<endl;
            
            //Diplay what the player has
            cout<< "Player: ";
            m_player.getHand()->myHand.at(0).displayCard();
            m_player.getHand()->myHand.at(1).displayCard();
            cout<< "["<<m_player.getHand()->getTotal()<<"]"<<endl; 
            
            bool wantsToDraw;
            //loop while the player isnt busted
            while (!m_player.isBusted()){
                wantsToDraw=m_player.isDrawing(); 
                //check if the player wants to keep drawing
                //this will open up the prompt to allow the player to choose to draw or not
                if (wantsToDraw){
                    m_deck.deal(*(m_player.getHand()));
                    cout<<"Player: "; 
                    //we iterate through the player's hand to display each card with displayCard()
                    for (int i=0; i<m_player.getHand()->myHand.size();i++){
                        m_player.getHand()->myHand.at(i).displayCard();
                    }
                    cout<<"["<<m_player.getHand()->getTotal()<<"]"<<endl;
                //if the player enters n on the prompt, then we come out of the loop and we check if the player busted
                }else {
                    break;
                }
            }
            
            //once out of the while loop, we check if the player has busted
            if (m_player.isBusted()){
                cout<< "Player busts."<<endl<<"Casino wins."<<endl;
            //if the player hasn't busted, then it's the casino's turn to draw until he has a total of 16 or more 
            }else{
                while(m_casino.isDrawing()){
                    m_deck.deal(*(m_casino.getHand()));
                    cout<<"Casino: "; 
                    for (int i=0; i<m_casino.getHand()->myHand.size();i++){
                        m_casino.getHand()->myHand.at(i).displayCard();
                    }
                    cout<<"["<<m_casino.getHand()->getTotal()<<"]"<<endl;
                }
                sumCasino= m_casino.getHand()->getTotal();
                //we use the humanPlayer announce method to compare the two sides
                m_player.announce(sumCasino);
            }
 
        }
    private:
        Deck m_deck;
        ComputerPlayer m_casino;
        HumanPlayer m_player;
};

int main(){
    std::srand(unsigned (std::time(0)));

    cout << "\tWelcome to the Blackjack game!" << endl << endl;
    BlackJackGame game;
    // The main loop of the game
    
    bool playAgain = true;
    char answer = 'y';
    while (playAgain)
    {
    
    game.play();
    // Check whether the player would like to play another round
    cout << "Would you like another round? (y/n): ";
    cin >> answer;
    cout << endl << endl;
    playAgain = (answer == 'y' ? true : false);
    }
    cout <<"Game over!";
    return 0;


}
