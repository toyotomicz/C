#include "matches.h"

static int _matches_pc_move(int current_matches) {
    const int moves[] = { 1, 2 }, moves_count = 2;      /* Možné tahy počítače. */
    const int core[] = { 9, 6, 3, 0}, core_count = 4;   /* Jádro orientovaného grafu hry. */
    int i, j;

    /* FYI: tohle je napraseno schválně. */

    for (i = 0; i < moves_count; ++i) {
        for (j = 0; j < core_count; ++j) {
            if (current_matches - moves[i] == core[j]) {
                printf("Pocitac odebral %d zapalek.\n", moves[i]);
                return moves[i];
            }  
        }
    }

    printf("Ted se deje neco opravu divneho...");
    return 1;   /* K tomuto by nikdy nemělo dojít... */
}

static int _matches_player_move() {
    char input[MAX_INPUT_LENGTH];
    int to_remove;

    printf("Kolik chcete odebrat vy? (1 nebo 2):");

    for (;;) {
        fgets(input, MAX_INPUT_LENGTH, stdin);
        to_remove = input[0] - '0';

        if (to_remove == 1 || to_remove == 2) {
            return to_remove;  
        }
        else {
            printf("Staci zadat 1 nebo 2. Zkuste to jeste jednou: ");
        }
    }
}

void run_matches() {
    unsigned char human_current_player = 0;     /* Aktuální hráč na tahu (0 = pc nebo 1 = hrac). */
    int matches_count = 11;                     /* Aktuální počet zápalek ve hře. */

    printf("Vitejte ve hre \"zapalky\".\nNa zacatku je 11 zapalek. Hraci postupne odebiraji 1 nebo 2 zapalky.\nVyhrava ten, kdo odebere posledni zapalku.\nProtoze si cenime Vasi \"financni pomoci\", tak zacina pocitac.\nJdeme na to! \n\n");
    while (matches_count > 0) {
        printf("Na stole je nyni %d zapalek.\n", matches_count);

        if (human_current_player) {
            matches_count -= _matches_player_move();
        }
        else {
            matches_count -= _matches_pc_move(matches_count);
        }

        human_current_player = !human_current_player;
    }

    if (!human_current_player) {
        printf("Prohrali jste. Ale neveste hlavu, pocitac v teto hre nemuze prohrat. \n");
    }
    else {
        printf("Gratuluji, porazili jste nepremozitelny pocitac.\n");
    }
}
