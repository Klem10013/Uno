/*
 * =====================================================================
 *  Serveur TCP multijoueur en C – exemple pédagogique
 * =====================================================================
 *
 *  Architecture : boucle select() sur un seul thread (pas de pthreads).
 *  Chaque joueur se connecte, envoie des commandes en texte brut,
 *  et le serveur diffuse l'état du jeu à tous les clients.
 *
 *  Protocole ASCII (client → serveur)
 *  -----------------------------------
 *    JOIN <nom>              Rejoindre la partie
 *    MOVE <UP|DOWN|LEFT|RIGHT>  Déplacer son personnage
 *    CHAT <message>          Envoyer un message à tous
 *    STATE                   Demander l'état courant du jeu
 *    QUIT                    Déconnexion propre
 *
 *  Réponses (serveur → client)
 *  ----------------------------
 *    WELCOME ...             Message d'accueil
 *    OK <info>               Commande acceptée
 *    ERR <raison>            Erreur
 *    NOTICE <texte>          Notification broadcast (connexion/déco)
 *    MSG <nom>: <texte>      Message chat d'un joueur
 *    UPDATE <id> <x>,<y>    Nouvelle position d'un joueur
 *    STATE <id>:<nom>:<x>,<y> [...]  Snapshot complet
 *
 *  Compilation :
 *    gcc -Wall -Wextra -o game_server game_server.c
 *
 *  Test rapide (dans 3 terminaux) :
 *    Terminal 1 : ./game_server
 *    Terminal 2 : nc localhost 8080   →  JOIN Alice  →  MOVE RIGHT
 *    Terminal 3 : nc localhost 8080   →  JOIN Bob    →  CHAT Bonjour !
 * =====================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <unistd.h>
#include <errno.h>
#include <time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include "deck.h"

/* ── Constantes ─────────────────────────────────────────────────── */

#define PORT         8080
#define MAX_CLIENTS  2
#define BUF_SIZE     512
#define MAP_W        20
#define MAP_H        10

/* ── Structures ─────────────────────────────────────────────────── */



/* ── Helpers réseau ─────────────────────────────────────────────── */

/* Envoie une chaîne formatée à un fd précis */
static void send_to(int fd, const char *fmt, ...) {
    char buf[BUF_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    send(fd, buf, strlen(buf), MSG_NOSIGNAL);
}

/* Diffuse à tous les joueurs prêts, sauf exclude_fd (-1 = inclure tout le monde) */
static void broadcast(GameState *gs, int exclude_fd, const char *fmt, ...) {
    char buf[BUF_SIZE];
    va_list ap;
    va_start(ap, fmt);
    vsnprintf(buf, sizeof buf, fmt, ap);
    va_end(ap);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        Player *p = &gs->players[i];
        if (p->fd != -1 && p->ready && p->fd != exclude_fd)
            send(p->fd, buf, strlen(buf), MSG_NOSIGNAL);
    }
}

/* Envoie un snapshot complet de l'état du jeu à un fd */
static void send_state(const GameState *gs, int fd) {
    char buf[BUF_SIZE];
    char tmp[80];
    snprintf(buf, sizeof buf, "STATE");
    for (int i = 0; i < MAX_CLIENTS; i++) {
        const Player *p = &gs->players[i];
        if (p->fd != -1 && p->ready) {
            snprintf(tmp, sizeof tmp, " %d:%s:%d,%d:HP%d:SC%d",
                     p->id, p->name, p->x, p->y, p->hp, p->score);
            strncat(buf, tmp, sizeof buf - strlen(buf) - 1);
        }
    }
    strncat(buf, "\n", sizeof buf - strlen(buf) - 1);
    send(fd, buf, strlen(buf), MSG_NOSIGNAL);
}

/* ── Recherches dans la liste de joueurs ─────────────────────────── */

static Player *find_free_slot(GameState *gs) {
    for (int i = 0; i < MAX_CLIENTS; i++)
        if (gs->players[i].fd == -1) return &gs->players[i];
    return NULL;
}

/* ── Handlers de commandes ────────────────────────────────────────── */

static void cmd_join(GameState *gs, Player *p, char *args) {
    if (p->ready) {
        send_to(p->fd, "ERR Vous êtes déjà connecté sous le nom '%s'\n", p->name);
        return;
    }
    if (!args || !*args) {
        send_to(p->fd, "ERR Usage : JOIN <votre_pseudo>\n");
        return;
    }

    /* Nettoie le pseudo */
    args[strcspn(args, "\r\n ")] = '\0';
    strncpy(p->name, args, sizeof p->name - 1);
    p->name[sizeof p->name - 1] = '\0';

    p->ready = 1;
    Card *top_deck = gs->deck
    p->card = top_deck
    Card *top_hand = p->card
    for (int i = 0;i < CAR_NUMBER;i ++){
        Card *card = 
    }
    gs->count++;

    send_to(p->fd,
            "OK Bienvenue %s ! (ID=%d)  Position initiale : %d,%d  HP=%d\n",
            p->name, p->id, p->x, p->y, p->hp);

    broadcast(gs, p->fd,
              "NOTICE %s a rejoint la partie (%d joueur(s) en ligne)\n",
              p->name, gs->count);

    /* Envoie le snapshot au nouveau venu */
    send_state(gs, p->fd);

    printf("[+] Joueur %d '%s' connecté (fd=%d) @ %d,%d\n",
           p->id, p->name, p->fd, p->x, p->y);
}

static void cmd_play(GameState *gs, Player *p, char *args) {
    if (!p->ready) { send_to(p->fd, "ERR Faites d'abord JOIN <pseudo>\n"); return; }
    if (!args)     { send_to(p->fd, "ERR Usage : PLAY card id\n"); return; }
    if (gs->player != p->id) {send_to(p->fd,"Err Not your turn"); return;}

    args[strcspn(args, "\r\n")] = '\0';

    int id = atoi(args);
    if (id == 0) { send_to(p->fd, "Err Usage Bad card id")}

    Card* card_played = deck_contain(p->card, id)
    if (card_played == NULL) {}

    int nx = p->x, ny = p->y;
    if      (strcmp(args, "UP")    == 0) ny--;
    else if (strcmp(args, "DOWN")  == 0) ny++;
    else if (strcmp(args, "LEFT")  == 0) nx--;
    else if (strcmp(args, "RIGHT") == 0) nx++;
    else {
        send_to(p->fd, "ERR Direction invalide. Utilisez : UP DOWN LEFT RIGHT\n");
        return;
    }

    /* Vérification des limites */
    if (nx < 0 || nx >= MAP_W || ny < 0 || ny >= MAP_H) {
        send_to(p->fd, "ERR Hors des limites de la carte (%dx%d) !\n", MAP_W, MAP_H);
        return;
    }

    /* Détection de collision avec un autre joueur (bonus pédagogique) */
    for (int i = 0; i < MAX_CLIENTS; i++) {
        Player *other = &gs->players[i];
        if (other->fd != -1 && other->ready && other != p
                && other->x == nx && other->y == ny) {
            send_to(p->fd, "ERR La case (%d,%d) est occupée par %s !\n",
                    nx, ny, other->name);
            return;
        }
    }

    p->x = nx; p->y = ny;
    p->score++;                 /* +1 point par déplacement (exemple simple) */

    send_to(p->fd, "OK Position : %d,%d  Score : %d\n", p->x, p->y, p->score);
    broadcast(gs, p->fd, "UPDATE %d %d,%d\n", p->id, p->x, p->y);
}

static void cmd_chat(GameState *gs, Player *p, char *args) {
    if (!p->ready) { send_to(p->fd, "ERR Faites d'abord JOIN <pseudo>\n"); return; }
    if (!args || !*args) { send_to(p->fd, "ERR Usage : CHAT <message>\n"); return; }

    args[strcspn(args, "\r\n")] = '\0';
    printf("[CHAT] %s: %s\n", p->name, args);
    broadcast(gs, -1, "MSG %s: %s\n", p->name, args);
}

static void cmd_state(const GameState *gs, Player *p) {
    if (!p->ready) { send_to(p->fd, "ERR Faites d'abord JOIN <pseudo>\n"); return; }
    send_state(gs, p->fd);
}

/* ── Déconnexion propre ────────────────────────────────────────── */

static void disconnect_player(GameState *gs, Player *p) {
    if (p->ready) {
        printf("[-] Joueur %d '%s' déconnecté (score final : %d)\n",
               p->id, p->name, p->score);
        broadcast(gs, p->fd,
                  "NOTICE %s a quitté la partie (%d joueur(s) restant)\n",
                  p->name, gs->count - 1);
        gs->count--;
    } else {
        printf("[-] Client anonyme (fd=%d) déconnecté\n", p->fd);
    }
    close(p->fd);
    memset(p, 0, sizeof *p);
    p->fd = -1;                  /* marque le slot comme libre */
}

/* ── Dispatcher de commandes ────────────────────────────────────── */

static void handle_command(GameState *gs, Player *p, char *line) {
    /* Supprime \r et \n de fin */
    line[strcspn(line, "\r\n")] = '\0';
    if (!*line) return;

    /* Découpe "COMMANDE [arguments]" */
    char *cmd  = strtok(line, " ");
    char *args = strtok(NULL, "");   /* tout ce qui suit la commande */

    if      (!cmd)                        return;
    else if (strcmp(cmd, "JOIN")  == 0)   cmd_join(gs, p, args);
    else if (strcmp(cmd, "PLAY")  == 0)   cmd_play(gs, p, args);
    else if (strcmp(cmd, "CHAT")  == 0)   cmd_chat(gs, p, args);
    else if (strcmp(cmd, "STATE") == 0)   cmd_state(gs, p);
    else if (strcmp(cmd, "QUIT")  == 0)   { /* géré dans la boucle principale */ }
    else send_to(p->fd, "ERR Commande inconnue : '%s'. "
                         "Essayez JOIN, MOVE, CHAT, STATE, QUIT\n", cmd);
}



/* ── Point d'entrée ─────────────────────────────────────────────── */

int main(void) {
    int server_fd;
    struct sockaddr_in srv_addr;
    GameState gs;

    srand((unsigned)time(NULL));
    memset(&gs, 0, sizeof gs);
    for (int i = 0; i < MAX_CLIENTS; i++) gs.players[i].fd = -1;

    /* ── Création du socket serveur ── */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd < 0) { perror("socket"); exit(EXIT_FAILURE); }




    /* Réutilisation immédiate du port après arrêt */
    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof opt);

    memset(&srv_addr, 0, sizeof srv_addr);
    srv_addr.sin_family      = AF_INET;
    srv_addr.sin_addr.s_addr = INADDR_ANY;
    srv_addr.sin_port        = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&srv_addr, sizeof srv_addr) < 0) {
        perror("bind"); exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 10) < 0) {
        perror("listen"); exit(EXIT_FAILURE);
    }

    printf("╔══════════════════════════════════════════╗\n");
    printf("║   Serveur de jeu TCP – port %-5d        ║\n", PORT);
    printf("║   Carte : %dx%d  |  Max joueurs : %d        ║\n",
           MAP_W, MAP_H, MAX_CLIENTS);
    printf("╚══════════════════════════════════════════╝\n");
    printf("Commandes : JOIN <nom>  MOVE <dir>  CHAT <msg>  STATE  QUIT\n\n");

    /* ── Boucle principale : select() ── */
    while (1) {
        fd_set readfds;
        int    max_fd = server_fd;

        FD_ZERO(&readfds);
        FD_SET(server_fd, &readfds);

        for (int i = 0; i < MAX_CLIENTS; i++) {
            int fd = gs.players[i].fd;
            if (fd != -1) {
                FD_SET(fd, &readfds);
                if (fd > max_fd) max_fd = fd;
            }
        }

        int activity = select(max_fd + 1, &readfds, NULL, NULL, NULL);
        if (activity < 0) {
            if (errno == EINTR) continue;
            perror("select");
            break;
        }

        /* ── Nouvelle connexion entrante ── */
        if (FD_ISSET(server_fd, &readfds)) {
            struct sockaddr_in cli_addr;
            socklen_t cli_len = sizeof cli_addr;
            int cli_fd = accept(server_fd,
                                (struct sockaddr *)&cli_addr, &cli_len);
            if (cli_fd < 0) { perror("accept"); continue; }

            Player *slot = find_free_slot(&gs);
            if (!slot) {
                send(cli_fd,
                     "ERR Serveur complet, réessayez plus tard.\n", 42,
                     MSG_NOSIGNAL);
                close(cli_fd);
                printf("[!] Connexion refusée (serveur plein)\n");
            } else {
                slot->fd    = cli_fd;
                slot->id    = ++gs.next_id;
                slot->ready = 0;
                printf("[~] Connexion entrante : %s:%d  (fd=%d)\n",
                       inet_ntoa(cli_addr.sin_addr),
                       ntohs(cli_addr.sin_port), cli_fd);
                send_to(cli_fd,
                        "WELCOME Serveur de jeu v1.0 – "
                        "Tapez JOIN <pseudo> pour commencer\n");
            }
        }

        /* ── Données reçues d'un client connecté ── */
        for (int i = 0; i < MAX_CLIENTS; i++) {
            Player *p = &gs.players[i];
            if (p->fd == -1 || !FD_ISSET(p->fd, &readfds)) continue;

            char buf[BUF_SIZE];
            int  n = recv(p->fd, buf, sizeof buf - 1, 0);

            if (n <= 0) {
                /* Déconnexion ou erreur réseau */
                disconnect_player(&gs, p);
            } else {
                buf[n] = '\0';
                /*
                 * Un client peut envoyer plusieurs lignes d'un coup.
                 * On les traite toutes avec strtok_r.
                 */
                char *saveptr;
                char *line = strtok_r(buf, "\n", &saveptr);
                while (line) {
                    if (strncmp(line, "QUIT", 4) == 0) {
                        send_to(p->fd, "OK À bientôt !\n");
                        disconnect_player(&gs, p);
                        break;          /* fd invalide désormais */
                    }
                    handle_command(&gs, p, line);
                    line = strtok_r(NULL, "\n", &saveptr);
                }
            }
        }
    }

    close(server_fd);
    return EXIT_SUCCESS;
}