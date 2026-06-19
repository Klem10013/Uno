#include "deck.h"

int new_card_id = 1;

Card* new_card(Color color, ) {
    
}


// check if a card with the id exist;
int deck_contain(Deck* deck, int id) {

}

// remove the card with id: return null if it doesnt exist;
Card* deck_remove(Deck int id);

// add a card into the deck
void deck_add_front(Card* card);
void deck_add_back(Card* card);

// draw the top card of the player;
Card* deck_draw();

// shuffle the deck
void deck_shuffle(Deck* deck);

// === UNO ===

// init a uno Deck
Deck* init_uno();