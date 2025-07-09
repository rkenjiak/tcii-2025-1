/* * * * * * * * * * * * * * * * */
/*  */
/*               */
/* * * * * * * * * * * * * * * * */
#ifndef __KNN_h
#define __KNN_h

#include "Concepts.h"
#include <utility>
#include <vector>
#include <queue>
#include <algorithm>

namespace tcii::p1
{ // begin namespace tcii::p1

    template <IsReal R>
    class KNN
    {
    public:
        using Neighbor = std::pair<R, unsigned>;
        using NeighborsQueue = std::priority_queue<Neighbor, std::vector<Neighbor>, std::less<Neighbor>>;

        KNN(unsigned k) : _k(k)
        {
        }

        R maxDistance() const
        {
            if (_neighbors.empty())
                return R{};

            return _neighbors.top().first;
        }

        unsigned getK() const
        {
            return _k;
        }

        void addNeighbor(R distance, unsigned index)
        {
            if (_neighbors.size() < _k)
            {
                _neighbors.push({ distance, index });
            }
            else if (distance < _neighbors.top().first)
            {
                _neighbors.pop();
                _neighbors.push({ distance, index });
            }
        }

        unsigned size() const
        {
            return (unsigned)_neighbors.size();
        }

        std::vector<Neighbor> getNeighbors()
        {
            std::vector<Neighbor> result;
            result.reserve(_neighbors.size());
            while (!_neighbors.empty())
            {
                result.push_back(_neighbors.top());
                _neighbors.pop();
            }
            std::reverse(result.begin(), result.end());
            return result;
        }

    private:
        unsigned _k;
        NeighborsQueue _neighbors;

    }; // KNN

} // end namespace tcii::p1

#endif // __KNN_h
