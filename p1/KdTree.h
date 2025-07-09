/* * * * * * * * * * * * * * * * */
/*  */
/*              */
/* * * * * * * * * * * * * * * * */
#ifndef __KdTree_h
#define __KdTree_h

#include "KNN.h"
#include "Utils.h"
#include <functional>
#include <utility>

namespace tcii::p1
{ // begin namespace tcii::p1

    using namespace cg;

    template <size_t D, IsReal R, IsPointArray<Vec, R, D> A>
    class PointHolder
    {
    public:
        using Point = Vec<D, R>;

        PointHolder(A&& points) :
            _points{ std::move(points) }
        {
            // do nothing
        }

        auto& points() const
        {
            return _points;
        }

    private:
        A _points;

    }; // PointHolder

    template <size_t D, typename R, typename A>
    class KdTree : public PointHolder<D, R, A>
    {
    public:
        using Base = PointHolder<D, R, A>;
        using Bounds = cg::Bounds<D, R>;
        using Point = typename Base::Point;
        using PointFunc = std::function<bool(const A&, unsigned)>;
        using KNN = p1::KNN<R>;

        const struct Params
        {
            unsigned maxPointsPerNode;
            unsigned minPointsPerNode;
            unsigned maxDepth;

        } params;

        static constexpr Params dflParams()
        {
            return { 20, 5, 8 };
        }

        KdTree(A&& points, const Params& params = dflParams());

        KdTree(const KdTree&) = delete;
        KdTree& operator =(const KdTree&) = delete;

        KdTree(KdTree&& other) noexcept :
            Base{ std::move(other) },
            _root{ std::exchange(other._root, nullptr) },
            _nodeCount{ other._nodeCount },
            _leafCount{ other._leafCount },
            params{ other.params }
        {
            // do nothing
        }

        ~KdTree()
        {
            //delete _root;
            deleteNode(_root);
        }

        auto& bounds() const
        {
            return _root->bounds;
        }

        auto nodeCount() const
        {
            return _nodeCount;
        }

        auto leafCount() const
        {
            return _leafCount;
        }

        KNN findNeighbors(const Point& point,
            unsigned k,
            PointFunc filter = {}) const;

        auto findNeighbors(unsigned index, unsigned k, PointFunc filter = {}) const
        {
            assert(index < this->points().size());
            return findNeighbors(this->points()[index], k, filter);
        }

        void forEachNeighbor(const Point& point,
            R radius,
            PointFunc f,
            PointFunc filter = {}) const;

        void forEachNeighbor(unsigned index,
            R radius,
            PointFunc f,
            PointFunc filter = {}) const
        {
            assert(index < this->points().size());
            forEachNeighbor(this->points()[index], radius, f, filter);
        }

    private:
        struct Node
        {
            Bounds bounds;
            bool isLeaf;
            unsigned depth;

            union
            {
                Node* child0; // both children
                struct
                {
                    size_t first;
                    size_t count;
                } leaf;
            };

            Node(const Bounds& bounds, unsigned depth) :
                bounds{ bounds },
                isLeaf{ true },
                depth{ depth },
                leaf{ 0, 0 }
            {
            }
        }; // Node

        Node* _root{};
        unsigned _nodeCount{};
        unsigned _leafCount{};
        unsigned _a0{};
        std::vector<unsigned> _indices;

        void split(Node* node,
            size_t first,
            size_t count);
        void deleteNode(Node* node);
        void findNeighborsRecursive(Node* node, const Point& point, PointFunc filter, KNN& knn) const;
        void forEachNeighborRecursive(Node* node, const Point& point, R radius, PointFunc f, PointFunc filter) const;
    }; // KdTree

    template <size_t D, typename R, typename A>
    KdTree<D, R, A>::KdTree(A&& points, const Params& params) :
        Base{ std::move(points) },
        params{ params }
    {
        _root = new Node{ computeBounds<D, R>(this->points()), 0 };
        _nodeCount = _leafCount = 1;

        _indices.resize(this->points().size());
        for (unsigned i = 0; i < this->points().size(); ++i) {
            _indices[i] = i;
        }

        auto min = _root->bounds.min();
        auto max = _root->bounds.max();
        R maxSize = -1.0;
        for (unsigned i = 0; i < D; i++)
        {
            if (max[i] - min[i] > maxSize) {
                maxSize = max[i] - min[i];
                _a0 = i;
            }
        }

        split(_root, 0, this->points().size());
    }

    template <size_t D, typename R, typename A>
    auto
        KdTree<D, R, A>::findNeighbors(const Point& point,
            unsigned k,
            PointFunc filter) const -> KNN
    {
        KNN knn{ k };
        findNeighborsRecursive(_root, point, filter, knn);
        return knn;
    }

    template <size_t D, typename R, typename A>
    void
        KdTree<D, R, A>::findNeighborsRecursive(Node* node,
            const Point& point,
            PointFunc filter,
            KNN& knn
        ) const
    {
        if (node->isLeaf)
        { //calculate distance to all points in the leaf  
            for (auto it = _indices.begin() + node->leaf.first;
                it != _indices.begin() + node->leaf.first + node->leaf.count;
                ++it)
            {
                auto idx = *it;
                if (!filter || filter(this->points(), idx))
                {
                    R dist = distance(point, this->points()[idx]);
                    knn.addNeighbor(dist, idx);
                }
            }
            return;
        }

        auto d0 = distance(point, node->child0[0].bounds);
        auto d1 = distance(point, node->child0[1].bounds);

        if (d0 < d1)
        {
            if (knn.size() == knn.getK() && d0 > knn.maxDistance())
                return;
            findNeighborsRecursive(node->child0, point, filter, knn);

            if (knn.size() == knn.getK() && d1 > knn.maxDistance())
                return;
            findNeighborsRecursive(node->child0 + 1, point, filter, knn);
        }
        else
        {
            if (knn.size() == knn.getK() && d1 > knn.maxDistance())
                return;
            findNeighborsRecursive(node->child0 + 1, point, filter, knn);

            if (knn.size() == knn.getK() && d0 > knn.maxDistance())
                return;
            findNeighborsRecursive(node->child0, point, filter, knn);
        }
        return;
    }

    template <size_t D, typename R, typename A>
    void
        KdTree<D, R, A>::forEachNeighbor(const Point& point,
            R radius,
            PointFunc f,
            PointFunc filter) const
    {
        if (radius < 0)
            return; // no neighbors within zero radius  
        forEachNeighborRecursive(_root, point, radius, f, filter);
    }

    template <size_t D, typename R, typename A>
    void
        KdTree<D, R, A>::forEachNeighborRecursive(Node* node,
            const Point& point,
            R radius,
            PointFunc f,
            PointFunc filter) const
    {
        constexpr R EPSILON = R(1e-6);

        if (node->isLeaf)
        { //calculate distance to all points in the leaf  
            for (auto it = _indices.begin() + node->leaf.first;
                it != _indices.begin() + node->leaf.first + node->leaf.count;
                ++it)
            {
                auto idx = *it;
                if (!filter || filter(this->points(), idx))
                {
                    R dist = distance(point, this->points()[idx]);
                    if (dist < radius + EPSILON) // dist <= radius
                    {
                        if (!f(this->points(), idx))
                            return; // stop iteration
                    }
                }
            }
            return;
        }
        auto d0 = distance(point, node->child0[0].bounds);
        auto d1 = distance(point, node->child0[1].bounds);

        if (d0 < d1)
        {
            if (d0 < radius + EPSILON) // d0 <= radius
                forEachNeighborRecursive(node->child0, point, radius, f, filter);

            if (d1 < radius + EPSILON) // d1 <= radius
                forEachNeighborRecursive(node->child0 + 1, point, radius, f, filter);
        }
        else
        {
            if (d1 < radius + EPSILON)
                forEachNeighborRecursive(node->child0 + 1, point, radius, f, filter);
            if (d0 < radius + EPSILON)
                forEachNeighborRecursive(node->child0, point, radius, f, filter);
        }
        return;
    }

    template <size_t D, typename R, typename A>
    void
        KdTree<D, R, A>::split(Node* node,
            size_t first,
            size_t count)
    {
        if (count <= params.minPointsPerNode || node->depth >= params.maxDepth)
        { // Stay as leaf node
            node->leaf.first = first;
            node->leaf.count = count;
            _leafCount++;
            return;
        }
        // Become a non-leaf node
        node->isLeaf = false;

        unsigned al = (_a0 + node->depth) % D;
        size_t m = (count + 1) / 2;

        std::nth_element(_indices.begin() + first,
            _indices.begin() + first + m,
            _indices.begin() + first + count,
            [this, al](unsigned a, unsigned b) {
                return this->points()[a][al] < this->points()[b][al];
            });

        auto leftCount = m;
        auto rightCount = count - m;
        R splitValue = this->points()[_indices[first + m]][al];

        // Set bounds for the two children
        auto parentMin = node->bounds.min();
        auto parentMax = node->bounds.max();

        auto leftMax = parentMax;
        leftMax[al] = splitValue;

        Bounds lBounds{};
        lBounds.inflate(parentMin);
        lBounds.inflate(leftMax);

        auto rightMin = parentMin;
        rightMin[al] = splitValue;

        Bounds rBounds{};
        rBounds.inflate(rightMin);
        rBounds.inflate(parentMax);

        _nodeCount += 2;
        node->child0 = new Node[2]{
          Node{lBounds, node->depth + 1},
          Node{rBounds, node->depth + 1}
        };

        split(node->child0, first, leftCount);
        split(node->child0 + 1, first + m, rightCount);
    }

    template <size_t D, typename R, typename A>
    void
        KdTree<D, R, A>::deleteNode(Node* node)
    {
        if (!node) return;

        if (!node->isLeaf && node->child0)
        {
            deleteNode(&node->child0[0]);
            deleteNode(&node->child0[1]);

            delete[] node->child0;
        }

        if (node == _root)
        {
            delete _root;
            _root = nullptr;
        }
    }

    template <typename R, typename A> using KdTree3 = KdTree<3, R, A>;

} // end namespace tcii::p1

#endif // __KdTree_h
