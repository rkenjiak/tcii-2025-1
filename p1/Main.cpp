/* * * * * * * * * * * * * * * * */
/*  */
/*              */
/* * * * * * * * * * * * * * * * */
#include "ParticleKdTree.h"
#include "Utils.h"
#include <iostream>

using namespace tcii::p1;

template <typename Tree>
inline void
test_1(Tree&& tree, bool noFilter)
{
    constexpr unsigned k{ 42 }; // n�mero de vizinhos a encontrar
    auto& points = tree.points();
    auto& bounds = tree.bounds();

    std::cout.precision(3);
    std::cout << "\nTree bounds: " << bounds
        << "\nPoints: " << points.size()
        << "\nNodes: " << tree.nodeCount()
        << "\nLeafs: " << tree.leafCount() << '\n';

    auto origin = typename Tree::Point{};

    /// Filtros /////////////////////////////////////////////////////////
    auto _1 = [&](const auto& points, unsigned idx) {
        constexpr float EPSILON = float(1e-6);
        return distance(origin, points[idx]) - 2.f > EPSILON; // maior que 2
        };
    auto _2 = [](const auto& points, unsigned idx) {
        return true;
        };
    std::function<bool(const decltype(points)&, unsigned)> filter;
    if (noFilter)
        filter = _2;
    else
        filter = _1;
    ///////////////////////////////////////////////////////////////////

    std::cout << "\nfindNeighbors(KNN):\n";
    auto v = tree.findNeighbors(
        origin,
        k,
        filter
    ).getNeighbors();
    std::cout << "Buscando " << k << " vizinhos\n";
    std::cout << "K: " << v.size() << " vizinhos encontrados\n";
    for (auto i = v.begin(); i != v.end(); ++i)
        std::cout << "Distancia: " << i->first
        << " Index: " << i->second
        << " Coordenada: " << tree.points()[i->second] << '\n';

    ////////////////////////////////////////////////////////////////////
    constexpr float radius{ 3.f };
    std::cout << "\nforEachNeighbor(Center: " << origin
        << "):\nRadius: " << radius << '\n';
    tree.forEachNeighbor(
        origin,
        radius,
        [origin](const auto& points, unsigned index)
        {
            std::cout << "Distancia: " << distance(points[index], origin)
                << " Index: " << index
                << " Coordenada: " << points[index] << '\n';
            return true;
        },
        filter
    );

}

template <typename Tree>
inline void
test_2(Tree& tree, bool noFilter)
{
    constexpr unsigned k{ 41 }; // n�mero de vizinhos a encontrar
    auto& points = tree.points();
    auto& bounds = tree.bounds();
    std::cout << "\n## ParticleKdTree test ##\n";

    std::cout.precision(3);
    std::cout << "\nTree bounds: " << bounds
        << "\nPoints: " << points.size()
        << "\nNodes: " << tree.nodeCount()
        << "\nLeafs: " << tree.leafCount() << '\n';

    auto origin = typename Tree::Point{};
    /// Filtros /////////////////////////////////////////////////////////
    auto _1 = [&](const auto& points, unsigned idx) { // diferente de vermelho (1,0,0)
        constexpr float EPSILON = float(1e-6);
        const auto& c = points.color(idx);
		//const auto& d = points.position(idx);
        return !(std::fabs(c[0] - 1.0f) < EPSILON &&
            std::fabs(c[1] - 0.0f) < EPSILON &&
            std::fabs(c[2] - 0.0f) < EPSILON);
        };
    auto _2 = [](const auto& points, unsigned idx) {
        return true;
        };
    std::function<bool(const decltype(points)&, unsigned)> filter;
    if (noFilter)
        filter = _2;
    else
        filter = _1;
    ///////////////////////////////////////////////////////////////////

    std::cout << "\nfindNeighbors(KNN):\n";
    auto v = tree.findNeighbors(
        origin,
        k,
        filter
    ).getNeighbors();
    std::cout << "Buscando " << k << " vizinhos\n";
    std::cout << "K: " << v.size() << " vizinhos encontrados\n";
    for (auto i = v.begin(); i != v.end(); ++i)
        std::cout << "Distancia: " << i->first
        << " Index: " << i->second
        << " Coordenada: " << points.position(i->second)
        << " Cor: " << points.color(i->second)
        << " Ligado: " << points.ligado(i->second) << '\n';

    ////////////////////////////////////////////////////////////////////

    constexpr float radius{ 4.f };

    auto f = [&](const auto& points, unsigned idx) {
        std::cout << "Distancia: " << distance(points.position(idx), origin)
            << "  Index: " << idx
            << "  Coordenada: " << points.position(idx)
            << "  Cor: " << points.color(idx)
            << "  Ligado: " << points.ligado(idx) << '\n';
        return true;
        };

    std::cout << "\nforEachNeighbor (Center: " << origin
        << "):\nRadius: " << radius << '\n';
    tree.forEachNeighbor(
        origin,
        radius,
        f,
        filter
    );
}

template <typename Tree>
inline void
test_3(Tree& tree, bool noFilter)
{
    constexpr unsigned k{ 50 }; // n�mero de vizinhos a encontrar
    auto& points = tree.points();
    auto& bounds = tree.bounds();
    std::cout << "\n## ParticleKdTree test ##\n";

    std::cout.precision(3);
    std::cout << "\nTree bounds: " << bounds
        << "\nPoints: " << points.size()
        << "\nNodes: " << tree.nodeCount()
        << "\nLeafs: " << tree.leafCount() << '\n';

    auto origin = typename Tree::Point{};
    /// Filtros /////////////////////////////////////////////////////////
    auto _1 = [&](const auto& points, unsigned idx) {
        constexpr float EPSILON = float(1e-6);
        const auto& c = points.color(idx);
        return !(std::fabs(c[0] - 1.0f) < EPSILON &&
            std::fabs(c[1] - 0.0f) < EPSILON &&
            std::fabs(c[2] - 0.0f) < EPSILON);
        };
    auto _2 = [](const auto& points, unsigned idx) {
        return true;
        };
    std::function<bool(const decltype(points)&, unsigned)> filter;
    if (noFilter)
        filter = _2;
    else
        filter = _1;
    ///////////////////////////////////////////////////////////////////

    std::cout << "\nfindNeighbors(KNN):\n";
    auto v = tree.findNeighbors(
        origin,
        k,
        filter
    ).getNeighbors();
    std::cout << "Buscando " << k << " vizinhos\n";
    std::cout << "K: " << v.size() << " vizinhos encontrados\n";
    for (auto i = v.begin(); i != v.end(); ++i)
        std::cout << "Distancia: " << i->first
        << " Index: " << i->second
        << " Coordenada: " << points.position(i->second)
        << " Cor: " << points.color(i->second)
        << " Ligado: " << points.ligado(i->second) << '\n';

    ////////////////////////////////////////////////////////////////////

    constexpr float radius{ 1.f };

    auto f = [&](const auto& points, unsigned idx) {

        std::cout << "Distancia: " << distance(points.position(idx), origin)
            << "  Index: " << idx
            << "  Coordenada: " << points.position(idx)
            << "  Cor: " << points.color(idx)
            << "  Ligado: " << points.ligado(idx) << '\n';

        return true;
        };

    std::cout << "\nforEachNeighbor (Center: " << origin
        << "):\nRadius: " << radius << '\n';
    tree.forEachNeighbor(
        origin,
        radius,
        f,
        filter
    );

}

int
main()
{
    constexpr unsigned np{ 10 };

    constexpr size_t D{ 2 };
    using R = double;
    using A = PointVector<D, R>;

    constexpr size_t D_2{ 3 };
    using R_2 = float;
    using A_2 = PointVector<D_2, R_2>;

    test_1(KdTree<D, R, A>{pointsOnSpheres<D, R>(np, 6)}, true); // 20*3=60 pontos
    puts("Press any key to continue...1");
    (void)getchar();

    test_1(KdTree<D_2, R_2, A_2>{pointsOnSpheres<D_2, R_2>(np, 6)}, false);
    puts("Press any key to continue...2");
    (void)getchar();

    auto ps = ParticleSystem<Vec3f, bool>::New();

    fillParticleBufferOnSpheres(*ps, np, 6);
    auto tree2 = ParticleKdTree<Vec3f, bool>{ *ps };

    test_2(tree2, true);
    puts("Press any key to continue...3");
    (void)getchar();

    test_2(tree2, false);
    puts("Press any key to continue...4");
    (void)getchar();
    destroy(ps);

    auto ps_2 = ParticleSystem<Vec3f, bool>::New();
    fillParticleBufferOnSpheres(*ps_2, 10000, 10);
    auto tree3 = ParticleKdTree<Vec3f, bool>{ *ps_2 };
    test_3(tree3, true);
    puts("Press any key to exit...");
    (void)getchar();
    destroy(ps_2);
    return 0;
}
