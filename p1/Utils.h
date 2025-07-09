/* * * * * * * * * * * * * * * * */
/*  */
/*               */
/* * * * * * * * * * * * * * * * */
#ifndef __Utils_h
#define __Utils_h

#include "graphics/Bounds2.h"
#include "graphics/Bounds3.h"
#include "Concepts.h"
#include <vector>

namespace tcii::p1
{ // begin namespace tcii::p1

    using namespace cg;

    template <size_t D, IsReal R, IsPointArray<Vec, R, D> A>
    auto
        computeBounds(const A& points)
    {
        Bounds<D, R> b;

        for (size_t n = points.size(), i = 0; i < n; ++i)
            b.inflate(points[i]);
        return b;
    }

    template <size_t D, IsReal R>
    auto
        distance(const Vec<D, R>& p, const Bounds<D, R>& bounds)
    {
        R d{};

        for (size_t i = 0; i < D; ++i)
            if (auto t = p[i] - bounds[0][i]; t < 0)
                d += t * t;
            else if (auto t = p[i] - bounds[1][i]; t > 0)
                d += t * t;
        return (R)std::sqrt(d);
    }

    template <size_t D, IsReal R>
    inline auto
        distance(const Vec<D, R>& a, const Vec<D, R>& b)
    {
        return (R)(b - a).length();
    }

    template <IsReal R>
    inline auto
        srand()
    {
        return (R)((double)rand() / RAND_MAX);
    }

    template <IsReal R>
    inline auto
        srand(R min, R max)
    {
        return min + srand<R>() * (max - min);
    }

    template <size_t D, IsReal R>
    inline auto
        prand(R min, R max)
    {
        Vec<D, R> p;

        for (size_t i = 0; i < D; ++i)
            p[i] = srand(min, max);
        return p;
    }

    template <size_t D, IsReal R>
    using PointVector = std::vector<Vec<D, R>>;

    template <size_t D, IsReal R>
    inline auto
        prand(size_t n, R min = 0, R max = 1)
    {
        assert(n > 0);

        PointVector<D, R> v(n);

        for (size_t i = 0; i < n; ++i)
            v[i] = prand<D>(min, max);
        return v;
    }

    template <size_t D, IsReal R>
    inline auto
    pointsOnSpheres(unsigned qtd, unsigned camadas)
    {
        PointVector<D, R> pontos(qtd * camadas);
        Vec<D, R> centro{};
        size_t idx = 0;
        for (unsigned j = 1; j <= camadas; ++j) {
            R raio = static_cast<R>(j);
            for (unsigned i = 0; i < qtd; ++i) {
                auto d = prand<D, R>(0.f, 1.f).versor();
                pontos[idx++] = centro + raio * d;
            }
        }
        return pontos;
    }

    template <typename ParticleSystemT>
    inline void
    fillParticleBufferOnSpheres(ParticleSystemT& ps, unsigned qtd, unsigned camadas)
    {
        using Vec3 = Vec<3, float>;
        static const Vec3 cores[] = {
            Vec3{1,0,0}, // vermelho
            Vec3{0,1,0}, // verde
            Vec3{0,0,1}  // azul
        };
        ps.setParticleBuffer(qtd * camadas);
        for (unsigned j = 0; j < camadas; ++j) {
            float raio = static_cast<float>(j + 1);
            Vec3 cor = cores[j % 3];
            for (unsigned i = 0; i < qtd; ++i) {
                auto d = prand<3, float>(0.f, 1.f).versor();
                Vec3 pos = raio * d;
                bool ligado = j % 2 == 0;
                ps.particles()->add(pos, cor, ligado);
            }
        }
    }

    template <typename R> using Points3 = PointVector<3, R>;

} // end namespace tcii::p1

#endif // __Utils_h
