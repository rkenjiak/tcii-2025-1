#include "GC.h"

namespace tcii::ex
{ // begin namespace tcii::ex

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 202319040134
//////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////
//
// Allocator implementation
// =========
Allocator* Allocator::_instance;

// insert your code here

void 
Allocator::initialize(unsigned heapSize)
{
  if (_instance == nullptr)
    _instance = new Allocator(heapSize);
}

Allocator::Allocator(unsigned heapSize): 
  _heap(new char[heapSize]),
  _freeList(nullptr),
  _freeSize(heapSize)
{
  // cabe�alho do heap
  _freeList = reinterpret_cast<BlockInfo*>(_heap); // header
  _freeList->flag = 0; // Free block
  _freeList->size = heapSize - 2 * sizeof(BlockInfo); //
  _freeList->prev = _freeList;
  _freeList->next = _freeList;
  // rodap� do heap
  char* footer = _heap + heapSize - sizeof(BlockInfo);
  BlockInfo* footerInfo = reinterpret_cast<BlockInfo*>(footer);
  footerInfo->flag = 0; // Free block
  footerInfo->size = 0; // Not used
  footerInfo->prev = _freeList;
  footerInfo->next = nullptr; // Not used

  _freeSize -= 2 * sizeof(BlockInfo);
}

Allocator::~Allocator()
{
  delete[] _heap;
  _instance = nullptr;
}

void
Allocator::free(void* p)
{
  char* ptr = reinterpret_cast<char*>(p);
  ptr -= sizeof(BlockInfo); // move to header
  BlockInfo* block = reinterpret_cast<BlockInfo*>(ptr);

  char* footer = ptr + block->size + sizeof(BlockInfo);
  BlockInfo* blockF = reinterpret_cast<BlockInfo*>(footer);

  char* esqFooter = ptr - sizeof(BlockInfo);
  char* dirHeader = footer + sizeof(BlockInfo);
  BlockInfo* esqF = reinterpret_cast<BlockInfo*>(esqFooter);
  BlockInfo* dirH = reinterpret_cast<BlockInfo*>(dirHeader);

  BlockInfo* esqH = esqF->prev;
  char* dirFooter = reinterpret_cast<char*>(dirH) + dirH->size + sizeof(BlockInfo);
  BlockInfo* dirF = reinterpret_cast<BlockInfo*>(dirFooter);


  if(esqF->flag == 0 && dirH->flag == 0) {
    // merge with left and right
    dirF->prev = esqH;
    esqH->next = dirH->next;

    dirH->next->prev = esqH; //?
    esqH->next = dirH->next;
    esqH->size += block->size + dirH->size + 4 * sizeof(BlockInfo);

    _instance->_freeSize += block->size + dirH->size + 4 * sizeof(BlockInfo);

  } else if(dirH->flag == 0) {
    // merge with right
    block->flag = 0; // Free block
    block->next = dirH->next;
    block->prev = dirH->prev;
    dirH->prev->next = block;
    dirH->next->prev = block;
    block->size += dirH->size + 2 * sizeof(BlockInfo);

    _instance->_freeSize += block->size + 2 * sizeof(BlockInfo);
  }else if(esqF->flag == 0) {
    // merge with left
    blockF->flag = 0; // Free block
    blockF->prev = esqH;
    esqH->size += block->size + 2 * sizeof(BlockInfo);

    _instance->_freeSize += block->size + 2 * sizeof(BlockInfo);
  } else {
    // no merge
    block->flag = 0; // Free block
    blockF->flag = 0; // Free block
    blockF->prev = block;

    // Encontra o local correto para inserir o bloco na lista circular
    BlockInfo* current = _instance->_freeList;
    while (reinterpret_cast<char*>(current) < ptr && current->next != _instance->_freeList) {
      current = current->next;
    }
    block->next = current->next;
    block->prev = current;
    current->next->prev = block;
    current->next = block;   

    _instance->_freeSize += block->size;
  }
}

void
Allocator::printMemoryMap()
{
  if (_instance == nullptr) {
    std::cout << "Allocator not initialized." << std::endl;
    return;
  }
  int count{ 1 };
  char* current = _instance->_heap;
  while (current < _instance->_heap + Allocator::heapSize) {
    std::cout << "Block " << count++ << ":\n";
    BlockInfo* block = reinterpret_cast<BlockInfo*>(current);
    std::cout << "Address: " << static_cast<void*>(current)
      << ", Flag: " << block->flag
      << ", Size: " << block->size << std::endl;
    current += block->size + 2 * sizeof(BlockInfo);
  }
  std::cout << "Total free size: " << _instance->_freeSize << " byte(s)\n==================================================" << std::endl;
}

void
Allocator::exit()
{
  if (_instance != nullptr) {
    delete _instance;
    _instance = nullptr;
  }
}

} // end namespace tcii::ex
