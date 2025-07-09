#include "ParticleSystem.h"
#include <cstdio>
#include <iostream>
#include <vector>

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 
//////////////////////////////////////////////////////////

template <size_t D, typename R>
using Vec = tcii::cg::Vec<D, R>;

template <typename R>
inline auto
srand()
{
    return (R)((double)rand() / RAND_MAX);
}

template <typename R>
inline auto
srand(R min, R max)
{
    return min + srand<R>() * (max - min);
}

template <size_t D, typename R>
inline auto
prand(R min, R max)
{
    Vec<D, R> p;

    for (size_t i = 0; i < D; ++i)
        p[i] = srand(min, max);
    return p;
}

template <size_t D, typename R>
using PointVector = std::vector<Vec<D, R>>;

template <size_t D, typename R>
inline auto
prand(size_t n, R min = 0, R max = 1)
{
    assert(n > 0);

    PointVector<D, R> v(n);

    for (size_t i = 0; i < n; ++i)
        v[i] = prand<D>(min, max);
    return v;
}

int
main()
{
    using namespace tcii::physx;

    auto ps = ParticleSystem<Vec3f, Vec3f>::New();

    ps->setParticleBuffer(1000000);

    auto pb = ps->particles();
    //adicionar os pontos
    auto pos = prand<3, float>(500000, 0.0f, 1.0f);
    auto speed = prand<3, float>(500000, 0.0f, 1.0f);
    auto color = prand<3, float>(500000, 0.0f, 1.0f);
    for (size_t i = 0; i < pos.size(); ++i)
    {
        pb->add(pos[i], speed[i], color[i]);
    }
    auto pc = pb->particleCount();

    printf("%s (capacity: %d)\n", ps->name(), pb->capacity());
    std::cout << "Particles: " << pc << "\nBounds: ";
    (pc ? std::cout << ps->bounds() : std::cout << "<empty>") << '\n';

    std::cout << "\nPrimeiras 5 particulas:\n";
    unsigned count = 0;
    for (auto it = pb->begin(); it != pb->end() && count < 5; ++it, ++count) {
        auto p = *it;
        std::cout << "Pos: " << p.position << "  Speed: " << p.speed << "  Color: " << p.color << '\n';
    }

    if (pb->particleCount() > 0) {
        pb->setPosition(0, Vec3f{ 1.0f, 2.0f, 3.0f });
        pb->setSpeed(0, Vec3f{ 0.0f, 0.0f, 0.0f });
        pb->setColor(0, Vec3f{ 1.0f, 0.0f, 0.0f });
        auto p = *pb->begin();
        std::cout << "\nApos modificacao da primeira particula:\n";
        std::cout << "Pos: " << p.position << "  Speed: " << p.speed << "  Color: " << p.color << '\n';
    }

    pb->clear();
    std::cout << "\nApos clear:\n";
    printf("%s (capacity: %d)\n", ps->name(), pb->capacity());
    std::cout << "Particles: " << pb->particleCount() << "\nBounds: ";
    (pc ? std::cout << ps->bounds() : std::cout << "<empty>") << '\n';

    destroy(ps);

    puts("Press any key to exit...");
    (void)getchar();

    return 0;
}
