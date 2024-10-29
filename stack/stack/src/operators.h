/**
 * \file operators.h
 * \author František Pártl (fpartl@kiv.zcu.cz)
 * \version 1.0
 * \date 2020-09-18
 */

#ifndef OPERATORS_H
#define OPERATORS_H

/**
 * \brief Definice datového typu, se kterým funkce pracují. Pokud chcete vidět, jak se toto dělá
 *        lépe a radostněji, tak přijďte na předposlední cvičení.
 */
typedef int calc_num_type;

/** \brief Definice datového typu ukazatele na funkci vykonávající aritmetickou operaci. */
typedef calc_num_type (*calc_handler_type)(calc_num_type, calc_num_type);

/** \brief Struktura, která obaluje operátor a k němu přidruženou obslužnou funkci. */
struct calc_oper_type {
    char operator;                  /*!< Znak, kterým je operace popsána. */
    calc_handler_type handler;     /*!< Přidružená aritmetická operace. */
};

/**
 * \brief Funkce vrátí ukazatel na obslužnou funkci podle zadaného operátoru. Operátory a k nim
 *        přidružené funkce musejí být uvedeny v poli OPERATORS, které je definováno v souboru .c.
 * 
 * \param operator Operátor, ke kterému je hledána jeho obslužná funkce.
 * \return calc_handler_type Ukazatel na obslužnou funkci, nebo konstanta NULL pokud operátor nebyl nalezen v poli OPERATORS.
 */
calc_handler_type get_operator_handler(char operator);

/* Odtud začínají deklarace funkcí, které vykonávají aritmetické operace. */
/**
 * \brief Funkce vrátí součet zadaných parametrů.
 * \param a Sčítanec.
 * \param b Sčítanec.
 * \return calc_num_type Součet.
 */
calc_num_type sum(calc_num_type a, calc_num_type b);

/**
 * \brief Funkce vrátí rozdíl zadaných parametrů.
 * \param a Menšenec.
 * \param b Menšenec.
 * \return calc_num_type Rozdíl.
 */
calc_num_type sub(calc_num_type a, calc_num_type b);

/**
 * \brief Funkce vrátí součin zadaných parametrů. 
 * \param a Činitel.
 * \param b Činitel.
 * \return calc_num_type Součin.
 */
calc_num_type mul(calc_num_type a, calc_num_type b);

/**
 * \brief Funkce vrátí podíl zadaných parametrů.
 * \param a Dělenec.
 * \param b Dělitel.
 * \return calc_num_type Podíl.
 */
calc_num_type divide(calc_num_type a, calc_num_type b);

#endif