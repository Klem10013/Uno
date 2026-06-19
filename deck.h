#pragma once

/*
Special Card:
+2
Skip

*/

enum Color { VERT, ROUGE, JAUNE, BLUE, JOCKER};

#define CAR_NUMBER   7


#define CHANGE_SENS 10
#define PLUS_DEUX   11
#define SAUTE       12

#define PLUS_FOUR    1000
#define COLOR_CHANGE 1001


typedef struct {
    int id;
    int value;
    Color color;
    Card *next;
} Card;

typedef struct {
    Card* head;
} Deck;

// check if a card with the id exist and return card selected;
Card* deck_contain(Deck* deck, int id);

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
