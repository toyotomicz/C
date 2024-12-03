#pragma once

template <typename T>
ps::Rectangle<T>::Rectangle(T a, T b) : m_a{a}, m_b{b} {
    std::cout << "Konstrukce objektu tridy ps::Rectangle(" << m_a << ", " << m_b << ")." << std::endl;
}

template <typename T>
ps::Rectangle<T>::~Rectangle() {
    std::cout << "Destrukce objektu tridy ps::Rectangle(" << m_a << ", " << m_b << ")." << std::endl;
}

template <typename T>
const T& ps::Rectangle<T>::a() const {
    return m_a;
}

template <typename T>
const T& ps::Rectangle<T>::b() const {
    return m_b;
}

template <typename T>
T ps::Rectangle<T>::perimeter() const {
    return (m_a + m_b) * T(2);
}

template <typename T>
T ps::Rectangle<T>::area() const {
    return m_a * m_b;
}

template <typename T>
void ps::Rectangle<T>::print(std::ostream &os) const {
    os << "Rectangle(" << m_a << ", " << m_b << ")";
}