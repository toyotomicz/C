#pragma once

#include <numbers>

#include "iplanary.hpp"

namespace ps {

/**
* \brief Třída Circle
* Představuje úplně obyčejný kruh.
*/
template <typename T>
class Circle : public IPlanary<T> {
public:
    /**
    * \brief Circle Konstruktor třídy.
    * \param r Poloměr kruhu.
    */
    explicit Circle(T r);

    /**
    * \brief ~Rectangle Destruktor třídy.
    */
    virtual ~Circle() override;

    /**
     * \brief Getter, který vrací velikost poloměru kruhu.
     * \return const T& Velikost poloměru kruhu.
     */
    const T& r() const;

    /**
     * \brief Metoda vrací obvod kruhu. 
     * \return T Obvod kruhu.
     */
    virtual T perimeter() const override final;

    /**
     * \brief Metoda, která vypočítá obsah kruhu.
     * \return T Obsah plochy kruhu.
     */
    virtual T area() const override final;

private:
    T m_r;     //!< Poloměr kruhu.

    /**
     * \brief Vypíše svůj obsah do zadaného proudu (streamu).
     * \param os Výstupní stream.
     */
    virtual void print(std::ostream &os) const override final;
};

#include "circle.tpp"

}