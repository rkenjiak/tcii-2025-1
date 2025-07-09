#include "String.h"
#include <cassert>

//////////////////////////////////////////////////////////
// Aluno: 
// RGA: 
//////////////////////////////////////////////////////////

inline void
stringTest()
{
  using namespace tcii::ex;
  using namespace std;

  String s1("Hello, World!");            // Create a String from a const char*
  String s2(s1);                         // Create a copy of s1 using the copy constructor
  String s3(String("Goodbye, World!"));  // Create a String using the move constructor

  cout << "s1: " << s1
       << "\ns2: " << s2
       << "\ns3: " << s3 << endl;

  cout << "s1 size: " << s1.size() << endl;  // Check size of s1
  cout << "s1 capacity: " << s1.capacity() << endl;  // Check capacity of s1
  s1.clear();  // Clear s1
  cout << "After clearing s1: " << s1 << endl;
  cout << "s1 size after clear: " << s1.size() << endl;  // Check size of s1 after clearing
  cout << "s1 capacity after clear: " << s1.capacity() << endl;  // Check capacity of s1 after clearing

  String s4("Long String Example That Exceeds Short Buffer");
  cout << "\ns4: " << s4 << endl;  // Display s4
  cout << "\ns4 size: " << s4.size() << endl;  // Check size of s4
  cout << "s4 capacity: " << s4.capacity() << endl;  // Check capacity of s4
  s4.clear();  // Clear s4
  cout << "After clearing s4: " << s4 << endl;
  cout << "s4 size after clear: " << s4.size() << endl;  // Check size of s4 after clearing
  cout << "s4 capacity after clear: " << s4.capacity() << endl;  // Check capacity of s4 after clearing

  cout << "\ns2: " << s2 << endl;  // Display s2
  cout << "s2 == Hello, World!: " << (s2 == "Hello, World!") << endl;  // Check equality with a const char*
  cout << "s2 == s1: " << (s2 == s1) << endl;  // Check equality with another String
  cout << "s2 != Goodbye, World!: " << (s2 != "Goodbye, World!") << endl;  // Check inequality with a const char*
  cout << "s2 != s3: " << (s2 != s3) << endl;  // Check inequality with another String

  cout << "\ns2[0]: " << s2[0] << endl;  // Access first character of s2
  cout << "s2[1]: " << s2[1] << endl;  // Access second character of s2
  s2[0] = 'h';  // Modify first character of s2
  cout << "After modifying s2[0]: " << s2 << endl;  // Display modified s2

  s2 = s3;  // Assign s3 to s2 using assignment operator
  cout << "\nAfter assigning s3 to s2: " << s2 << endl;  // Display s2 after assignment
  s2 = "123";  // Assign a const char* to s2
  cout << "After assigning '123' to s2: " << s2 << endl;  // Display s2 after assignment

  s2 = s2 + "456";  // Concatenate a const char* to s2
  cout << "After concatenating '456' to s2: " << s2 << endl;  // Display s2 after concatenation
  s2 += "789";  // Use += operator to concatenate another const char*
  cout << "After using += to concatenate '789' to s2: " << s2 << endl;  // Display s2 after using +=
  s2 += s3;  // Use += operator to concatenate another String
  cout << "After using += to concatenate s3 to s2: " << s2 << endl;  // Display s2 after using +=
  
}

//
// Main function
//
int
main()
{
  stringTest();
  puts("Press any key to exit...");
  (void)getchar();

  return 0;
}
