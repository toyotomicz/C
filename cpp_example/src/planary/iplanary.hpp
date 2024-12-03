#pragma once

#include <iostream>

namespace ps {

/**
 * \brief Třída IPlanary
 *
 * C++ nezná pojem "rozhraní". Tato třída obsahuje pouze deklarace ryze virtuálních metod
 * (pure virtual). Pokud se pokusíme vytvořit objekt této třídy, tak překlad skončí chybou
 * protože kompilátor nenalezne definice metod (členských funkcí).
 */
template <class T>
class IPlanary {
public:
    /**
     * \brief Pokud třída obsahuje virtuální metody, tak musí obsahovat i virtuální destruktor.
     *        Při použití polymorfismu je toto nezbytné -- jinak by se mohlo stát, že při
     *        dealokaci objektu přes ukazatel na jeho předka dojde k úniku paměti. Takto je
     *        zajištěno zavolání správného destruktoru potomka.
     */
    virtual ~IPlanary() = default;

    /**
     * \brief perimeter Vypočítá obvod rovinné oblasti.
     * 
     * Díky klíčovému slovu `virtual` bude mít tato metoda záznam v tabulce virtuálních metod.
     * Tento záznam mohou potomci třídy přepsat (`override`), čímž odkážou na svou vlastní
     * implementaci metody. Ukazatel na tabulku virtuálních metod je dle standardu C++ umístěn
     * na začátku instance třídy. Tedy i když referenci na instanci potomka přetypujete na
     * referenci na instanci předka, díky ukazateli na korektní tabulku virtuálních metod
     * budou provolány implementace potomka. Toto je základní princip polymorfismu. Je tedy
     * jasné, že když klíčové slovo `virtual` neuvedete, kompilátor záznam do tabulky virtuálních
     * metod nepřidá a tedy ani potomek nemůže metodu "přepsat" (může ji jenom "překrýt").
     * \return Obvod rovinné oblasti.
     */
    virtual T perimeter() const = 0;

    /**
     * \brief area Vypočítá obsah rovinné oblasti.
     * \return Obsah rovinné oblasti.
     */
    virtual T area() const = 0;

    /**
     * \brief Přetížení operátoru bitového posunu, aby bylo možné vypisovat objekty pomocí std::cout.
     * \param os Výstupní stream.
     * \param p Vypisovaný objekt.
     * \return std::ostream& Výstupní stream. 
     */
    friend std::ostream& operator<<(std::ostream& os, const IPlanary<T>& p) {
        p.print(os); // Voláme virtuální metodu, která vypíše obsah objektu.
        return os;
    };

private:
    /**
     * \brief Vypíše svůj obsah do zadaného proudu (streamu).
     * \param os Výstupní stream.
     */
    virtual void print(std::ostream &os) const = 0;
};

}
