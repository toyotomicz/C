#pragma once

template <typename T>
ps::Circle<T>::Circle(T r) : m_r{r} {
    std::cout << "Konstrukce objektu tridy ps::Circle(" << m_r << ")." << std::endl;
}

template <typename T>
ps::Circle<T>::~Circle() {
    std::cout << "Destrukce objektu tridy ps::Circle(" << m_r << ")." << std::endl;
}

template <typename T>
const T& ps::Circle<T>::r() const {
    return m_r;
}

template <typename T>
T ps::Circle<T>::perimeter() const {
    return 2.0 * std::numbers::pi * m_r;
}

template <typename T>
T ps::Circle<T>::area() const {
    return std::numbers::pi * m_r * m_r;
}

template <typename T>
void ps::Circle<T>::print(std::ostream &os) const {
    os << "Circle(" << m_r << ")";
}
