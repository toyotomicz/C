#pragma once

#include <iostream>


namespace uts {

/**
 * \brief Třída Wiffle
 *
 * Délková jednotka odpovídající 89 mm. Jedná se o průměr míčku ke hře Wiffleball.
 * Další info zde (https://en.wikipedia.org/wiki/List_of_humorous_units_of_measurement#Wiffle).
 */
class Wiffle {
public:
    /**
     * \brief Wiffle Bezparametrový konstruktor nastaví hodnotu na 0.
     */
    Wiffle();

    /**
     * \brief Wiffle Konstruktor třídy. Všimněte si, že klíčové slovo `explicit` není uvedeno.
     * \param u Rozměr ve Wifflech.
     */
    Wiffle(double u);

    /**
     * \brief operator double Přetížení operátoru přetypování na double.
     */
    operator double() const;

    /**
     * \brief operator + Přetížení operátoru sčítání.
     * \param b Druhý sčítanec.
     * \return Výsledný součet.
     */
    Wiffle operator+(const Wiffle& b) const;

    /**
     * \brief operator * Přetížení operátoru násobení.
     * \param b Druhý činitel při násobení.
     * \return Výsledný součin.
     */
    Wiffle operator*(const Wiffle& b) const;

    /**
     * \brief operator << Přetížení operátoru binárního posunu. Všimněte si klíčového
     *                    slova friend -- toto je snad jeho jediná smysluplná varianta
     *                    užití. Pozn.: možná by klíčové slovo mělo být raději
     *                    girlfriend/boyfriend.
     * \param os Datový proud, do kterého bude objekt zapsán.
     * \param w Co bude zapsáno.
     * \return Předaná reference na datový proud -- vidíte praktický význam návratové hodnoty?
     */
    friend std::ostream& operator<<(std::ostream& os, const Wiffle& w);

private:
    double m_u;     //!< Rozměr ve Wifflech.
};

}

/**
 * \brief Dokonce mohu definovat svůj vlastní literál. Při užití například konstrukce `1.0_Wf`
 *        bude použito toto přetížení.
 */
uts::Wiffle operator"" _Wf(long double u);
