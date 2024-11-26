/**
 * \file game.c
 * \author František Pártl (fpartl@ntis.zcu.cz)
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <matches_engine/matches.h>
#include "md5/md5.h"

/** \brief Maximální délka načítaného řetězce (hesla). */
#define MAX_PASSWORD_LENGTH 128

/** \brief Validní heslo uložené v otevřené formě. Toto skutečně není dobrý nápad. */
#define VALID_PASSWORD "letmein"

/** \brief Kryptografický otisk validního hesla. Pozor na použití md5. */
#define VALID_PASSWORD_HASH "$1$asdgrewd$emWxmKfI1QN8MwJV8N59i."

/**
 * \brief Použitá knihovna pro výpočet otisku md5 neobsahuje hlavičkový soubor. Je tedy použít 
 *        tuto dopřednou deklaraci, abych se vyhnuli případným varováním kompilátoru.
*/
extern char *md5_crypt (const char *key, const char *salt);

/**
 * \brief Zkontroluje, zda se zadané heslo shoduje s `VALID_PASSWORD`.
 * \param password Zadané heslo.
 * \return int 1, pokud hesla shodují, jinak 0.
 */
int check_pass_code(const char *password) {
    return strncmp(password, VALID_PASSWORD, MAX_PASSWORD_LENGTH) == 0;
}

/**
 * \brief Zkontroluje, zda se md5 kryptografický otisk zadaného hesla shoduje s `VALID_PASSWORD_HASH`.
 * \param password Zadané heslo.
 * \return int 1, pokud se otisk hesla shoduje s uloženým, jinak 0.
 */
int check_pass_code_hash(const char *password) {
    char *hash;
    int valid;

    hash = md5_crypt(password, "");
    valid = strcmp(hash, VALID_PASSWORD_HASH) == 0;

    free(hash);
    return valid;
}

/**
 * \brief Aplikace ověří, zda uživatel zadal správné heslo. Pokud ano, spustí hru "Zápalky".
 * \return int `EXIT_SUCCESS` pokud uživatel zadal správné heslo, jinak `EXIT_FAILURE`.
 */
int main() {
    char password[MAX_PASSWORD_LENGTH];

    printf("Pro hrani hry zadejte heslo: ");
    fgets(password, MAX_PASSWORD_LENGTH, stdin);
    password[strlen(password) - 1] = '\000';

    if (check_pass_code(password)) {    /* Zde možná změna na `check_pass_code_hash(password)` */
        printf("Heslo je spravne! Jdeme na to!\n\n");
        run_matches();            /* Volání knihovní funkce, jejíž zdrojový kód neznáme (a ani nás nezajímá). */
        return EXIT_SUCCESS;
    }
    else {
        printf("Nespravne heslo!\n");
        return EXIT_FAILURE;
    }
}
