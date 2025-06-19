#ifndef __Type_h
#define __Type_h

#include "Symbol.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

class Type: public Symbol
{
public:
  static const auto error()
  {
    static Type _error{"<error>"};
    return &_error;
  }

  static const auto Int()
  {
    static Type _int{"int"};
    return &_int;
  }

  static const auto IntArray()
  {
    static Type _intArray{"int[]"};
    return &_intArray;
  }

  static const auto Boolean()
  {
    static Type _boolean{"boolean"};
    return &_boolean;
  }

  virtual bool canCastTo(const Type*) const;

protected:
  Type(const String& name):
    Symbol{name}
  {
    makeUse(this);
  }

}; // Type

using TypePtr = ObjectPtr<Type>;

} // end namespace mjc::symbol

#endif // __Type_h
