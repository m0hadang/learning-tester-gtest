#include <iostream>

class NotImplementedException : public std::exception {
public:
  virtual const char* what() { 
    return "you have to implement code"; 
  }
};