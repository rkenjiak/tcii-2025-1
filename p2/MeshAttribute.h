#ifndef __MeshAttribute_h

#include "TriangleMesh.h"
#include "util/SoA.h"
#include "util/SharedObject.h"

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 
//////////////////////////////////////////////////////////

namespace tcii::cg
{ // begin namespace tcii::cg

  struct Allocator
  {
    template<typename T>
    static T* allocate(size_t size)
    {
      return new T[size];
    }

    template<typename T>
    static void free(T* ptr)
    {
      delete[] ptr;
    }

  }; // Allocator

  using MeshIndex = typename TriangleMesh::index_t;

  template<typename... Fields>
  using ElementAttribute = SoA<Allocator, MeshIndex, Fields...>;

  template<typename VA, typename TA>
  class MeshAttribute;

  template<typename VA, typename TA>
  class MeshAttribute : public SharedObject
  {
  public:
    static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh)
    {
      return new MeshAttribute( mesh );
    }

    template<typename VA2>
    static ObjectPtr<MeshAttribute> New(MeshAttribute<VA2, TA>&& other)
    {
      return new MeshAttribute(std::move(other));
    }

    template<typename TA2>
    static ObjectPtr<MeshAttribute> New(MeshAttribute<VA, TA2>&& other)
    {
      return new MeshAttribute(std::move(other));
    }
    
    template<size_t I>
    auto vertexAttribute(MeshIndex i)
    {
      return _va.template get<I>(i);
    }
    
    template<size_t I, typename Field>
    void setVertexAttribute(MeshIndex i, Field&& field)
    {
      _va.template get<I>(i) = std::forward<Field>(field);
    }

    template <typename...Fields>
    void setVertexAttributes(MeshIndex i, Fields&&... fields)
    {
      _va.set(i, std::forward<Fields>(fields)...);
    }

    template<size_t I>
    auto triangleAttribute(MeshIndex i)
    {
      return _ta.template get<I>(i);
    }

    template<size_t I, typename Field>
    void setTriangleAttribute(MeshIndex i, Field&& field)
    {
      _ta.template get<I>(i) = std::forward<Field>(field);
    }

    template <typename...Fields>
    void setTriangleAttributes(MeshIndex i, Fields&&... fields)
    {
      _ta.set(i, std::forward<Fields>(fields)...);
    }

    auto vertexCount() const
    {
      return _mesh->data().vertexCount();
    }

    auto triangleCount() const
    {
      return _mesh->data().triangleCount();
    }

  private:
    ObjectPtr<TriangleMesh> _mesh;
    VA _va;
    TA _ta;

    MeshAttribute(const TriangleMesh& mesh) :
      _mesh{ &mesh },
      _va{ mesh.data().vertexCount() },
      _ta{ mesh.data().triangleCount() }
    {
      // do nothing
    }        
    
    template<typename VA2>
    MeshAttribute(MeshAttribute<VA2, TA>&& other) :
      _mesh{ other._mesh },
      _va{ other.vertexCount() },
      _ta{ std::move(other._ta) }
    {
      other._mesh = nullptr;
    }

    template<typename TA2>
    MeshAttribute(MeshAttribute<VA, TA2>&& other) :
      _mesh{ other._mesh },
      _va{ std::move(other._va) },
      _ta{ other.triangleCount() }
    {
      other._mesh = nullptr;
    }

    template<typename, typename> friend class MeshAttribute;

  }; // MeshAttribute

  template<typename VA>
  class MeshAttribute<VA, void> : public SharedObject
  {
  public:
    static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh)
    {
      return new MeshAttribute{ mesh };
    }

    template<size_t I>
    auto vertexAttribute(MeshIndex i)
    {
      return _va.template get<I>(i);
    }

    template<size_t I, typename Field>
    void setVertexAttribute(MeshIndex i, Field&& field)
    {
      _va.template get<I>(i) = std::forward<Field>(field);
    }

    template <typename...Fields>
    void setVertexAttributes(MeshIndex i, Fields&&... fields)
    {
      _va.set(i, std::forward<Fields>(fields)...);
    }

    auto vertexCount() const
    {
      return _mesh->data().vertexCount();
    }

    auto triangleCount() const
    {
      return _mesh->data().triangleCount();
    }

  private:
    ObjectPtr<TriangleMesh> _mesh;
    VA _va;

    MeshAttribute(const TriangleMesh& mesh) :
      _mesh{ &mesh },
      _va{ mesh.data().vertexCount() }
    {
      // do nothing
    }    

    template<typename, typename> friend class MeshAttribute;

  }; // MeshAttribute<VA,void>

  template<typename TA>
  class MeshAttribute<void, TA> : public SharedObject
  {
  public:
    static ObjectPtr<MeshAttribute> New(const TriangleMesh& mesh)
    {
      return new MeshAttribute{ mesh };
    } 

    template<size_t I>
    auto triangleAttribute(MeshIndex i)
    {
      return _ta.template get<I>(i);
    }

    template<size_t I, typename Field>
    void setTriangleAttribute(MeshIndex i, Field&& field)
    {
      _ta.template get<I>(i) = std::forward<Field>(field);
    }

    template <typename...Fields>
    void setTriangleAttributes(MeshIndex i, Fields&&... fields)
    {
      _ta.set(i, std::forward<Fields>(fields)...);
    }

    auto vertexCount() const
    {
      return _mesh->data().vertexCount();
    }

    auto triangleCount() const
    {
      return _mesh->data().triangleCount();
    }

  private:
    ObjectPtr<TriangleMesh> _mesh;
    TA _ta;

    MeshAttribute(const TriangleMesh& mesh) :
      _mesh{ &mesh },
      _ta{ mesh.data().triangleCount() }
    {
      // do nothing
    }

    template<typename, typename> friend class MeshAttribute;

  }; // MeshAttribute<void,TA>

} // end namespace tcii::cg


#endif