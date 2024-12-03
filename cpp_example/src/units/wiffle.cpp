#include "wiffle.hpp"


uts::Wiffle::Wiffle() : m_u{0.0} { }

uts::Wiffle::Wiffle(double u) : m_u{u} { }

uts::Wiffle::operator double() const {
    return m_u;
}

uts::Wiffle uts::Wiffle::operator+(const uts::Wiffle& b) const {
    Wiffle temp{*this};
    temp.m_u += b.m_u;

    return temp;
}

uts::Wiffle uts::Wiffle::operator*(const uts::Wiffle& b) const {
    Wiffle temp{*this};
    temp.m_u *= b.m_u;

    return temp;
}

namespace uts {
std::ostream& operator<<(std::ostream& os, const uts::Wiffle& w) {
    os << w.m_u << " Wf";
    return os;
}
}

uts::Wiffle operator"" _Wf(long double u) {
    return uts::Wiffle{static_cast<double>(u)};
    //return static_cast<double>(u);
}
