#ifndef __Vec2_h
#define __Vec2_h

#include <cassert>
#include <cmath>
#include <iostream>

namespace tcii::cg
{ // begin namespace tcii::cg

#define ASSERT_REAL(T, msg) static_assert(std::floating_point<T>, msg)

    template <size_t D, typename real> struct Vec;

    template <typename real>
    struct Vec<2, real>
    {
        ASSERT_REAL(real, "Vec2: floating-point type expected");

        using value_type = real;

        real x;
        real y;

        auto& operator [](size_t i)
        {
            assert(i >= 0 && i < 2);
            return (&x)[i];
        }

        auto operator [](size_t i) const
        {
            return const_cast<Vec*>(this)->operator [](i);
        }

        real length() const;

        auto versor() const
        {
            return ((real)1 / length()) * *this;
        }

    }; // Vec2

    template <typename real> using Vec2 = Vec<2, real>;

    template <typename real>
    inline Vec2<real>
        operator +(const Vec2<real>& u, const Vec2<real>& v)
    {
        return { u.x + v.x, u.y + v.y };
    } // Vec2

    template <typename real>
    inline Vec2<real>
        operator -(const Vec2<real>& u, const Vec2<real>& v)
    {
        return { u.x - v.x, u.y - v.y };
    } // Vec2

    template <typename real>
    inline Vec2<real>
        operator *(real s, const Vec2<real>& v)
    {
        return { s * v.x, s * v.y };
    } // Vec2

    template <typename real>
    inline Vec2<real>
        operator *(const Vec2<real>& v, real s)
    {
        return s * v;
    } // Vec2

    template <typename real>
    inline real
        Vec<2, real>::length() const
    {
        return std::sqrt(x * x + y * y);
    } // Vec2

    template <typename T>
    std::ostream&
        operator <<(std::ostream& os, const Vec2<T>& v)
    {
        os << '(' << v.x << ',' << v.y << ')';
        return os;
    } // Vec2

    using Vec2f = Vec2<float>; // Vec2

} // end namespace tcii::cg

#endif // __Vec2_h