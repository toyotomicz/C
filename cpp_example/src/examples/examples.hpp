#pragma once

#include <vector>
#include <algorithm>
#include <random>
#include <memory>

#include "../units/wiffle.hpp"
#include "../planary/rectangle.hpp"
#include "../planary/circle.hpp"

/** \brief Velikost ukázkového vektoru. Takové typově bezpečné makro z C. */
constexpr size_t SAMPLE_VECTOR_SIZE = 5;

/**
 * \brief Funktor, který je automaticky vygenerován na základě lambda funkce `generator_lambda` použité ve funkci
 *        `lambda_functions`.
 */
class GeneratorFunctor {
public:
    GeneratorFunctor(std::mt19937_64 &gen, std::uniform_real_distribution<double> &dist) : m_gen{gen}, m_dist{dist} {}

    /** \brief Přetížení operátoru funkčního volání. */
    double operator()() {
        return m_dist(m_gen);
    }

private:
    std::mt19937_64 &m_gen;
    std::uniform_real_distribution<double> &m_dist;
};

/** \brief Ukázka lambda funkcí. */
void lambda_functions();

/** \brief Ukázka chytrých ukazatelů. */
void smart_pointers();

/** \brief Ukázka dynamického polymorfismu. */
void polymorphism();
