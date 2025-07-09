#ifndef __String_h
#define __String_h

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif // _MSC_VER

#include <cassert>
#include <string>
#include <iostream>

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 
//////////////////////////////////////////////////////////

namespace tcii::ex
{ // begin namespace tcii::ex


//////////////////////////////////////////////////////////
//
// String class
// ======
class String
{
public:
  ~String();

  String();
  String(const char* const);
  String(const String&);
  String(String&&) noexcept;

  void clear()
  {
    if (!isShort())
      delete[] _data;
    _size = 0;
    _data = _buffer;
    _buffer[0] = '\0';
  }

  auto size() const
  {
    return _size;
  }

  auto capacity() const
  {
    return isShort() ? maxBuffer - 1 : _capacity;
  }

  auto empty() const
  {
    return _size == 0;
  }

  const char* c_str() const
  {
    return _data;
  }

  String& operator =(const char* const s)
  {
    clear();
    copy(s, (unsigned)strlen(s));
    return *this;
  }
  String& operator =(const String& s)
  {
    if (this != &s)
    {
      clear();
      copy(s._data, s._size);
    }
    return *this;
  }
  String& operator =(String&& s) noexcept
  {
    if (this != &s)
    {
      clear();
      move(s);
    }
    return *this;
  }

  String operator +(const char* const s) const
  {
    unsigned newSize = _size + (unsigned)strlen(s);
    char* temp = new char[newSize + 1];
    strcpy(temp, c_str());
    strcat(temp, s);
    String ret(temp);
    delete[] temp;
    return ret;
  }
  String operator +(const String& s) const
  {
    return operator +(s.c_str());
  }

  String& operator +=(const char* const s) // sempre chama o op+
  {
    *this = *this + s;
    return *this;
  }

  String& operator +=(const String& s)
  {
    *this = *this + s.c_str();
    return *this;
  }

  auto& operator [](unsigned i)
  {
    assert(i <= _size);
    return _data[i];
  }

  auto operator [](unsigned i) const
  {
    assert(i <= _size);
    return _data[i];
  }

  bool operator ==(const char* const s) const
  {
    return strcmp(c_str(), s) == 0;
  }
  bool operator ==(const String& s) const
  {
    return operator ==(s.c_str());
  }

  auto operator !=(const char* const s) const
  {
    return !operator ==(s);
  }

  auto operator !=(const String& s) const
  {
    return !operator ==(s);
  }

private:
  static constexpr auto maxBuffer = 16u;

  unsigned _size;
  char* _data;
  union
  {
    unsigned _capacity;
    char _buffer[maxBuffer];
  };

  bool isShort() const
  {
    return _size < maxBuffer;
  }

  void copy(const char* const, unsigned);
  void move(String&) noexcept;

}; // String

inline std::ostream& 
operator <<(std::ostream& os, const String& s)
{
  os << s.c_str();
  return os;
}

inline String operator+(const char* l, const String& r)
{  
  return String(l) + r;
}

} // end namespace tcii::ex

#endif // __String_h
