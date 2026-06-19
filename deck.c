#include "deck.h"

#include <stddef.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>



Deck* deck_init() {
    return calloc(sizeof(Deck), 1);
}

void deck_free_rec(Card* card) {
    if (!card) { return; }
    
    if (card->next != NULL) {
        deck_free_rec(card->next);
    } 
    card_free(card);
}

void deck_free(Deck* deck) {
    deck_free_rec(deck->head);
    free(deck);
}

int card_cmp(Card* a, Card* b) {
    return a->value == b->value && a->color == b->color;
}

int new_card_id = 1;
Card* card_init(Color color, int value) {
    Card* card = calloc(sizeof(Card), 1);
    
    card->id = new_card_id;
    new_card_id++;

    card->value = value;
    card->color = color;
    card->next = NULL;
}

void card_free(Card* card) {
    free(card);
};


char* card_to_json(Card* card) {
    char* buffer = calloc(sizeof(char), 1024);
    sprintf(buffer, "{ \"id\":%d, \"color\":%d, \"value\":%d }");
    return buffer;
}

void deck_print(Deck* deck) {
    Card* head = deck->head;
    while (head) {
        char* s = card_to_json(head);
        printf("%s\n", s);
        free(s);
    }
}

// check if a card with the id exist;
Card* deck_contain(Deck* deck, int id) {
    Card* head = deck->head;
    while (head) {
        if card_cmp(head->id == id) {
            return head;
        }

        head = head->next;
    }

    return NULL;
}

// remove the card with id: return null if it doesnt exist;
Card* deck_remove(Deck* deck, int id) {
    Card** head = &deck->head;
    while (*head) {
        if card_cmp(*head->id == id) {
            Card* tmp = *head;
            
            *head = &tmp->next;
            tmp->next = NULL;
            return tmp;
        }

        head = &head->next;
    }

    return NULL;
}

// add a card into the deck
void deck_add_front(Deck* deck, Card* card) {
    card->next = deck->head;
    deck->head = card;
}

void deck_add_back(Deck* deck, Card* card) {
    Card* head = deck;
    if (head == NULL) {
        deck->head = card;
    }
    while (head->next) {
        head = head->next;
    }

    card->next = NULL;
    head->next = card;
}

// draw the top card of the player;
Card* deck_draw(Deck* deck) {
    if (deck->head == NULL) {
        return NULL;
    }
    else {
        Card* tmp = deck->head;
        deck->head = tmp->next;
        return tmp;
    }
}

// shuffle the deck
void deck_shuffle(Deck* deck) {
    srandom(time(NULL));

    size_t len = 0;
    Card* head = deck->head;
    while (head) {
        len++;
        head = head->next;
    }

    Card** vector = calloc(sizeof(Card*), len);
    Card** result = calloc(sizeof(Card*), len);

    for (size_t i = 0; i < len; i++) {
        vector[i] = deck_draw(deck);
    }

    for (size_t i = 0; i < len; i++) {
        long l = random() % (len - i);
        result[i] = vector[l];

        vector[l] = vector[len - i - 1];
    }

    for (size_t i = 0; i < len; i++) {
        deck_add_front(result[i]);
    }
}

// === UNO ===

// init a uno Deck
Deck* init_uno() {
    Deck* deck = deck_init();

    for (size_t j = 1; j < 5; j++) {
        Card* a = card_init(j, 0);
    }

    for (size_t i = 1; i < 13; i++) {
        for (size_t j = 1; j < 5; j++) {
            Card* a = card_init(j, i);
            Card* b = card_init(j, i);
        }
    }

    for (int i = 0; i < 4; i++) {
        Card* a = card_init(0, PLUS_FOUR);
        Card* b = card_init(0, COLOR_CHANGE);
    }
    
    return deck;
}