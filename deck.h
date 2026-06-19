#pragma once

/*
Special Card:
+2
Skip

*/

enum Color { 
    JOCKER = 0,
    VERT = 1, 
    ROUGE = 2, 
    JAUNE = 3, 
    BLEU = 4
};

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

void card_free(Card* card);
Card* card_init(Color color, int value);
void deck_free(Deck* deck);
Deck* deck_init();

// return an array with the json encoding of the card
char* card_to_json(Card* card);
// print deck;
void deck_print(Deck* deck);

// check if a card with the id exist and return card selected;
Card* deck_contain(Deck* deck, int id);

// remove the card with id: return null if it doesnt exist;
Card* deck_remove(Deck int id);

// add a card into the deck
void deck_add_front(Deck* deck, Card* card);
void deck_add_back(Deck* deck, Card* card);

// draw the top card of the player;
Card* deck_draw(Deck* deck);

// shuffle the deck
void deck_shuffle(Deck* deck);

// === UNO ===

// init a uno Deck
Deck* init_uno();
