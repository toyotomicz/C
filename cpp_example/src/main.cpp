/**
 * \file main.cpp
 * @author František Pártl (fpartl@ntis.zcu.cz)
 * \brief Ukázka již známých konceptů z C v C++.
 * @version 1.0
 * \date 2020-11-21
 */

#include <iostream>
#include <memory>

#include "units/wiffle.hpp"
#include "examples/examples.hpp"

/**
 * Často můžete na internetových příkladech vidět následující řádku, která vám ušetří psaní `std::`, `ps::` a `uts::`
 * v dalším kódu. Nedělejte to! Zaděláváte si na průšvih ve formě konfliktů jmenných prostorů. Třeba úplně primitivní
 * `vector`... pokud budete užívat `vector` knihovny STL, a pak budete na něco potřebovat knihovnu Boost nebo OpenCV
 * -- Vaše jistota, že používáte právě `std::vector` je rázem pryč.
 */
// using namespace std; // fuj...

/**
 * \brief CONSTANT Takto se dělá konstanta v C++. Dokázali byste říci, co je na tomto řešení
 *                 lepší oproti klasickým preprocesorovým konstantám?
 */
constexpr double CONSTANT = 1.0;

/** \brief main Funkce `main` je vcelku klasika... */
int main() {
    uts::Wiffle a;
    // uts::Wiffle a(CONSTANT);
    // uts::Wiffle a{CONSTANT};
    // uts::Wiffle a = CONSTANT;
    // auto a = 10.0_Wf; // WTF?!

    std::cout << a << std::endl;

    smart_pointers();
    lambda_functions();
    polymorphism();
}
