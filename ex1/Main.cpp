#include "GC.h"
#include <iostream>

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 
//////////////////////////////////////////////////////////

inline void
allocatorTest()
{
  using namespace tcii::ex;

  Allocator::initialize();

  Allocator::printMemoryMap();

  int* arr = Allocator::allocate<int>(5000);  
  float* arr2 = Allocator::allocate<float>(1000);
  Allocator::printMemoryMap();

  Allocator::free(arr);
  Allocator::printMemoryMap();

  Allocator::free(arr2);
  Allocator::printMemoryMap();

  int* arr3 = Allocator::allocate<int>(12345);
  float* arr4 = Allocator::allocate<float>(23456);
  double* arr5 = Allocator::allocate<double>(34567);
  Allocator::printMemoryMap();

  Allocator::free(arr5);
  Allocator::printMemoryMap();

  Allocator::free(arr4);
  Allocator::printMemoryMap();

  Allocator::free(arr3);
  Allocator::printMemoryMap();

  // Allocating more memory than available

  puts("Press any key to try allocating more memory than available...");
  (void)getchar();  
  //float* arr6 = Allocator::allocate<float>(300000);
  //Allocator::printMemoryMap();

  Allocator::exit();
}

//
// Main function
//
int
main()
{
  allocatorTest();
  puts("Press any key to exit...");
  (void)getchar();
  return 0;
}
