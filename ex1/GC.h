#ifndef __GC_h
#define __GC_h

#include <new>
#include <iostream>

namespace tcii::ex
{ // begin namespace tcii::ex

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//
// BlockInfo class
// =========
struct BlockInfo
{
  unsigned flag;
  unsigned size;
  BlockInfo* prev;
  BlockInfo* next;

}; // BlockInfo


//////////////////////////////////////////////////////////
//
// Allocator class
// =========
class Allocator
{
public:
  static constexpr auto minBlockSize = 16u;
  static constexpr auto heapSize = 1048576u;

  static void initialize(unsigned = heapSize); 
  template <typename T> static T* allocate(unsigned = 1);
  static void free(void*);
  static void printMemoryMap();
  static void exit();  

private:
  static Allocator* _instance;

  char* _heap;          //
  BlockInfo* _freeList; //
  unsigned _freeSize;   // 

  Allocator(unsigned = heapSize);
  ~Allocator();

}; // Allocator

template <typename T>
static T* 
Allocator::allocate(unsigned n)
{
  if (_instance == nullptr) {
    initialize();
  }
  // loop to find a suitable block, circular linked list
  T* ret = nullptr;
  BlockInfo* CURRENT = _instance->_freeList;
  unsigned sizeNeeded = n * sizeof(T) + 2 * sizeof(BlockInfo);

  if (CURRENT == nullptr)
    throw std::bad_alloc();

  do
  {
    if (CURRENT->flag == 0 && CURRENT->size >= sizeNeeded) {
      // found a suitable block
      if (CURRENT->size - sizeNeeded >= Allocator::minBlockSize) {
        char* footer = reinterpret_cast<char*>(CURRENT) + CURRENT->size + sizeof(BlockInfo);
        BlockInfo* FOOTER = reinterpret_cast<BlockInfo*>(footer);
        FOOTER->flag = 1;

        char* newFooter = footer - sizeNeeded;
        BlockInfo* NEWFOOTER = reinterpret_cast<BlockInfo*>(newFooter);
        NEWFOOTER->flag = 0; // Free block
        NEWFOOTER->prev = CURRENT;

        CURRENT->size -= sizeNeeded;

        char* newBlock = newFooter + sizeof(BlockInfo);
        BlockInfo* NEWBLOCK = reinterpret_cast<BlockInfo*>(newBlock);
        NEWBLOCK->flag = 1; // Allocated block
        NEWBLOCK->size = n * sizeof(T);

        _instance->_freeSize -= sizeNeeded;
        ret = reinterpret_cast<T*>(newBlock + sizeof(BlockInfo));

        // prev e next's are unchanged
      }
      else {// use the whole block          
        CURRENT->flag = 1; // Allocated block
        // adjust footer
        char* footer = reinterpret_cast<char*>(CURRENT) + CURRENT->size + sizeof(BlockInfo);
        BlockInfo* FOOTER = reinterpret_cast<BlockInfo*>(footer);
        FOOTER->flag = 1; // Allocated block
        _instance->_freeSize -= CURRENT->size + 2 * sizeof(BlockInfo);

        if (CURRENT->prev != CURRENT) {
          CURRENT->prev->next = CURRENT->next;
          CURRENT->next->prev = CURRENT->prev;
        }
        else {
          _instance->_freeList = nullptr; // do i lose the free list??
        }
        ret = reinterpret_cast<T*>(reinterpret_cast<char*>(CURRENT) + sizeof(BlockInfo));
      }
    }
    CURRENT = CURRENT->next;
  } while (CURRENT != _instance->_freeList);

  if (!ret)
    throw std::bad_alloc();
  return ret;
}

} // end namespace tcii::ex

#endif // __GC_h
