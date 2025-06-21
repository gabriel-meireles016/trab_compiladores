#ifndef __Symbol_h
#define __Symbol_h

#include "SharedObject.h"
#include <string>

namespace mjc
{ // begin namespace mjc

namespace symbol
{ // begin namespace symbol

using String = std::string;

class Symbol: virtual public SharedObject
{
public:
  virtual ~Symbol() = default;

  auto name() const
  {
    return _name.c_str();
  }

protected:
  Symbol(const String& name):
    _name{name}
  {
    // do nothing
  }

private:
  String _name;

}; // Symbol

using SymbolPtr = ObjectPtr<Symbol>;

} // end namespace symbol

} // end namespace mjc

#endif // __Symbol_h