#ifndef __Variable_h
#define __Variable_h

#include "Type.h"

namespace mjc::symbol
{ // begin namespace calc::symbol

class Variable: public Symbol
{
public:
  Variable(const String& name, const Type* type):
    Symbol{name},
    _type{type}
  {
    // do nothing
  }

  auto type() const
  {
    return _type;
  }

protected:
  const Type* _type;

}; // Variable

using VariablePtr = ObjectPtr<Variable>;

} // end namespace calc::symbol

#endif // __Variable_h
