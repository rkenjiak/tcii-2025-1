#ifndef __ParticleBuffer_h
#define __ParticleBuffer_h

#include "graphics/Bounds3.h"
#include "util/SharedObject.h"
#include "util/SoA.h"
#include "Renderer.h"

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 
//////////////////////////////////////////////////////////

namespace tcii::physx
{ // begin namespace tcii::physx

    using namespace cg;

    template <typename... Fields> class ParticleSystem;

    template <typename... Fields>
    class ParticleBuffer : public SharedObject
    {
    public:
        auto particleCount() const
        {
            return _particleCount;
        }

        auto capacity() const
        {
            return _soa.size();
        }

        void add(const Vec3f& p, const Fields&... fields)
        {
            assert(_particleCount < _soa.size());
            _soa.set(_particleCount, p, fields...);
            ++_particleCount;
        }

        void clear()
        {
            _particleCount = 0;
        }

        auto& position(unsigned i) const
        {
            return _soa.template get<0>(i);
        }

        auto& speed(unsigned i) const
        {
            return _soa.template get<1>(i);
        }

        auto& color(unsigned i) const
        {
            return _soa.template get<2>(i);
        }

        auto& position(unsigned i)
        {
            return _soa.template get<0>(i);
        }

        auto& speed(unsigned i)
        {
            return _soa.template get<1>(i);
        }

        auto& color(unsigned i)
        {
            return _soa.template get<2>(i);
        }

        void setPosition(unsigned i, const Vec3f& pos) {
            _soa.template get<0>(i) = pos;
        }

        void setSpeed(unsigned i, const Vec3f& speed) {
            _soa.template get<1>(i) = speed;
        }

        void setColor(unsigned i, const Vec3f& color) {
            _soa.template get<2>(i) = color;
        }

        Bounds3f bounds() const;
        void render(Renderer&) const;

    public:
        struct ParticleView {
            Vec3f& position;
            Vec3f& speed;
            Vec3f& color;
        };

        class iterator {
        public:
            iterator(ParticleBuffer* buffer, unsigned idx)
                : _buffer(buffer), _idx(idx) {
            }

            ParticleView operator*() {
                return ParticleView{
                  _buffer->position(_idx),
                  _buffer->speed(_idx),
                  _buffer->color(_idx)
                };
            }

            iterator& operator++() { ++_idx; return *this; }
            bool operator!=(const iterator& other) const { return _idx != other._idx; }

        private:
            ParticleBuffer* _buffer;
            unsigned _idx;
        };
        iterator begin() { return iterator(this, 0); }
        iterator end() { return iterator(this, _particleCount); }


    private:
        struct MyAllocator
        {
            template <typename T>
            static auto allocate(unsigned count)
            {
                return new T[count];
            }
            template <typename T>

            static void free(T* data)
            {
                delete[] data;
            }
        };

        using MySoa = SoA<MyAllocator, unsigned, Vec3f, Fields...>;
        MySoa _soa;
        unsigned _particleCount{};

        ParticleBuffer(unsigned capacity) :
            _soa{ capacity },
            _particleCount{ 0 }
        {
        }

        friend ParticleSystem<Fields...>;

    }; // ParticleBuffer

    template <typename... Fields>
    Bounds3f
        ParticleBuffer<Fields...>::bounds() const
    {
        Bounds3f b;
        for (unsigned i = 0; i < _particleCount; ++i)
            b.inflate(position(i));
        return b;
    }

    template <typename... Fields>
    void
        ParticleBuffer<Fields...>::render(Renderer& renderer) const
    {
        std::cout << "ParticleBuffer: " << _particleCount << " particles\n";
    }

} // end namespace tcii::physx

#endif // __ParticleBuffer_h
