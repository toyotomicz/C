#pragma once

#include "iplanary.hpp"

namespace ps {

/**
* \brief Třída Rectangle
*
* Představuje úplně obyčejný obdélník zadaný délkami stran.
*/
template <typename T>
class Rectangle : public IPlanary<T> {
public:
    /**
    * \brief Rectangle Konstruktor třídy. Všimněte se initializer listu v C++.
    * \param a Rozměr strany a.
    * \param b Rozměr strany b.
    */
    explicit Rectangle(T a, T b);

    /**
    * \brief ~Rectangle Destruktor třídy.
    */
    virtual ~Rectangle() override;

    /**
     * \brief Getter, který vrací délku strany a.
     * \return const T& Délka strany a.
     */
    const T& a() const;

    /**
     * \brief Getter, který vrací délku strany b.
     * \return const T& Délka strany b.
     */
    const T& b() const;

    /**
     * \brief Metoda vrací obvod obdélníku.
     * \return T Obvod obdélníků.
     */
    virtual T perimeter() const override final;

    /**
     * \brief Metoda, která vypočítá obsah plochy.
     * \return T Obsah plochy obdélníku.
     */
    virtual T area() const override final;

private:
    T m_a;     //!< Délka strany a.
    T m_b;     //!< Délka strany b.

    /**
     * \brief Vypíše svůj obsah do zadaného proudu (streamu).
     * \param os Výstupní stream.
     */
    virtual void print(std::ostream &os) const override final;

};

#include "rectangle.tpp"

}