/**
 * \file matches.h
 * \author František Pártl (fpartl@ntis.zcu.cz)
 * \brief Implementace hry matches ("zápalky"). Na začátku je 11 zápalek. Hráči se střídají v odebírání jedné nebo dvou
 *        zápalek. Kdo první odebere všechny zápalky vyhrává.
 */

#ifndef MATCHES_H
#define MATCHES_H

#include <stdlib.h>
#include <stdio.h>

/** \brief Maximální délka vstupu od uživatele. */
#define MAX_INPUT_LENGTH 4

/** \brief Spustí hru "zápalky". */
void run_matches();

#endif
