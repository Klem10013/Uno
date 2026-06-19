#pragma once

typedef Rule {
    int coupe;             // peut-on couper ?
    int four_on_two;       // 
    int two_on_four;       // 
    int coupe_jocker;      //
    int multi_same_number; //
    int end_with_jocker;   // 
} Rule;

typedef struct {
    int  fd;            /* socket fd, -1 = slot libre              */
    int  id;            /* identifiant unique                       */
    char name[32];      /* pseudo du joueur                         */
    Deck *deck;         /* carte en main                            */
    int  ready;         /* 1 après un JOIN valide                   */
} Player;

typedef struct {
    Player players[MAX_CLIENTS];
    Card   *deck;
    int    player       /* id du joueur qui doit jouer              */
    int    count;       /* joueurs actuellement connectés           */
    int    next_id;     /* générateur d'ID unique                   */
} GameState;


void add_player(GameState gs*);

void remove_player(GameState gs*);
void play_game(GameState gs*, int action, int card_id);