#include "MeshAttribute.h"

using namespace tcii::cg;

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 
//////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////
// 1st test: decorate mesh with 2 and 3 attributes
//
inline auto
decorate_1(const TriangleMesh& mesh)
{
  using Color = Vec3f;
  using _2d = Vec2f;
  using id = unsigned int;
  using broken = bool;

  using VA = ElementAttribute<Color,id>;
  using TA = ElementAttribute<Color,_2d, broken>;
  using MA = MeshAttribute<VA, TA>;

  auto nv = mesh.data().vertexCount();
  auto nt = mesh.data().triangleCount();
  auto ma = MA::New(mesh);

  for (decltype(nv) i = 0; i < nv; ++i)  
    ma->setVertexAttributes(i, Color{ 1,0,0 }, id{ i });  
  ma->setVertexAttribute<0>(0, Color{ 10,0,0 });
  ma->setVertexAttribute<1>(0, 10);      

  for (decltype(nv) i = 0; i < nt; ++i)
      ma->setTriangleAttributes(i, Color{ 0,2,0 }, _2d{ 0,2 }, broken{ i%2==0 });
  ma->setTriangleAttribute<0>(0, Color{ 0,20,0 });
  ma->setTriangleAttribute<1>(0, _2d{ 0,20 });
  ma->setTriangleAttribute<2>(0, false); // was true

  return ma;
}

/////////////////////////////////////////////////////////
// 2nd test: decorate only vertices with 2 attribute
//
inline auto
decorate_2(const TriangleMesh& mesh)
{
  using Color = Vec3f;
  using VA = ElementAttribute<Color, Color>;
  using MA = MeshAttribute<VA, void>;

  auto nv = mesh.data().vertexCount();
  auto ma = MA::New(mesh);

  for (decltype(nv) i = 0; i < nv; ++i)
      ma->setVertexAttributes(i, Color{ 0,2,0 }, Color{ 0,0,3 });
  ma->setVertexAttribute<0>(1, Color{ 0,20,0 });
  ma->setVertexAttribute<1>(1, Color{ 0,0,30 });

  return ma;
}

/////////////////////////////////////////////////////////
// 3rd test: decorate Vertices and then Triangles and then Vertices again
//
inline auto
decorateVertices(const TriangleMesh& mesh)
{
  using Color = Vec3f;
  using VA = ElementAttribute<Color>;
  using MA = MeshAttribute<VA, void>;

  auto nv = mesh.data().vertexCount();
  auto ma = MA::New(mesh);

  for (decltype(nv) i = 0; i < nv; ++i)
      ma->setVertexAttributes(i, Color{ 0,0,5 });
  ma->setVertexAttribute<0>(0, Color{ 0,0,50 });
  return ma;
}

inline auto
decorateTriangles(auto& ma)
{
  using Color = Vec3f;
  using id = unsigned int;
  using VA = ElementAttribute<Color>;
  using TA = ElementAttribute<Color,id>;
  using MA = MeshAttribute<VA, TA>;

  auto nt = ma->triangleCount();
  auto ma_ = MA::New(std::move(*ma));

  for (decltype(nt) i = 0; i < nt; ++i)
      ma_->setTriangleAttributes(i, Color{ 7,0,0 }, id{ (i+5)%10 });
  ma_->setTriangleAttribute<0>(0, Color{ 70,0,0 });
  return ma_;
}

inline auto
decorateVertices_2(auto& ma)
{  
  using Color = Vec3f;
  using id = unsigned int;
  using metric = double;
  constexpr auto pi = static_cast<metric>(3.14159);
  using VA = ElementAttribute<Color, id, metric>; // Descartou VA antigo
  using TA = ElementAttribute<Color, id>;
  using MA = MeshAttribute<VA, TA>;

  auto nv = ma->vertexCount();
  auto ma_ = MA::New(std::move(*ma));

  for (decltype(nv) i = 0; i < nv; ++i)
    ma_->setVertexAttributes(i, Color{ 0,0,3 }, id{ (i+2)%7 }, metric{ pi*(i+1)/(i+10) });
  ma_->setVertexAttribute<0>(0, Color{ 0,0,0 });
  ma_->setVertexAttribute<1>(0, 10);
  ma_->setVertexAttribute<2>(0, 0.001);
  return ma_;
}

/////////////////////////////////////////////////////////
//
int
main()
{
  auto filename = "../../meshes/f-16.obj";  
  auto mesh = readOBJ(filename);

  if (!mesh)
    printf("Could not read '%s'\n", filename);
  else
    //mesh->print(filename);
    printf("Read successful.\n");
  
  puts("Press any key to start first test...");
  (void)getchar();
  /////////////////////////////////////////////////////////
  // Decorate mesh with 2 and 3 attributes
  puts("Decorating mesh with 2 and 3 attributes:");
  auto ma = decorate_1(*mesh);
  for (unsigned i = 0; i < 5; ++i) 
    std::cout << "V-" << i << ": "
              << ma->vertexAttribute<0>(i) << " - " 
              << ma->vertexAttribute<1>(i) << std::endl; // Color, id
  for (unsigned i = 0; i < 5; ++i)
    std::cout << "T-" << i << ": "
              << ma->triangleAttribute<0>(i) << " - " 
              << ma->triangleAttribute<1>(i) << " - "
              << ma->triangleAttribute<2>(i) << std::endl; // Color, 2d, broken


  puts("Press any key to start second test...");
  (void)getchar();
  /////////////////////////////////////////////////////////
  // Decorate vertices with 2 attributes
  auto ma2 = decorate_2(*mesh);
  puts("Decorating only Vertices with 2 attributes:");
  for (unsigned i = 0; i < 5; ++i) {
    std::cout << "V-" << i << ": "
              << ma2->vertexAttribute<0>(i) << " - " 
              << ma2->vertexAttribute<1>(i) << std::endl; // Color, Color
  }
  // Triangles are not decorated, can't access triangle attributes
  //std::cout << ma2->triangleAttribute<0>(0) << std::endl; // does not compile

  
  puts("Press any key to start third test...");
  (void)getchar();
  /////////////////////////////////////////////////////////
  // Decorate Vertices and then Triangles and then Vertices again
  auto ma3 = decorateVertices(*mesh);
  puts("Vertices decorated, triangles not(pt.1):");
  for (unsigned i = 0; i < 5; ++i) {
    std::cout << "V-" << i << ": "
              << ma3->vertexAttribute<0>(i) << std::endl; // Color
  }

  // Triangles are not decorated yet, can't access triangle attributes
  //std::cout << ma3->triangleAttribute<0>(0) << std::endl; // does not compile

  auto ma4 = decorateTriangles(ma3);
  puts("\nDecorating Triangles(pt.2):");
  for( unsigned i = 0; i < 5; ++i) 
    std::cout << "V-" << i << ": "
              << ma4->vertexAttribute<0>(i) << std::endl; // Color
  for (unsigned i = 0; i < 5; ++i)
    std::cout << "T-" << i << ": "
              << ma4->triangleAttribute<0>(i) << " - "
              << ma4->triangleAttribute<1>(i) << std::endl; // Color, id
  

  auto ma5 = decorateVertices_2(ma4);
  puts("\nDecorating Vertices again(pt.3):");
  for (unsigned i = 0; i < 5; ++i) 
    std::cout << "V-" << i << ": "
              << ma5->vertexAttribute<0>(i) << " - "
              << ma5->vertexAttribute<1>(i) << " - "
              << ma5->vertexAttribute<2>(i) << std::endl; // Color, id, metric
  for (unsigned i = 0; i < 5; ++i)
    std::cout << "T-" << i << ": "
              << ma5->triangleAttribute<0>(i) << " - "
              << ma5->triangleAttribute<1>(i) << std::endl; // Color, id
  
  
  
  /////////////////////////////////////////////////////////
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
