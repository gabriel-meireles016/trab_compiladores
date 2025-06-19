#ifndef __Scope_h
#define __Scope_h

#include "SymbolMap.h"

namespace mjc
{ // begin namespace mjc

namespace symbol
{ // begin namespace symbol

class Class;
class Variable;
class Method;
class Type;
class GlobalScope;

class Scope: virtual public SharedObject
{
public:
  static GlobalScope* global();

  Scope* parent() const
  {
    return _parent;
  }

  virtual Class* lookupClass(const String&) const;
  virtual Variable* lookupVariable(const String&) const;
  virtual Method* lookupMethod(const String&) const;

  virtual Class* addClass(const String&, Class*);
  virtual Variable* addVariable(const String&, const Type*);
  virtual Method* addMethod(const String&, const Type*, Class*);

protected:
  Scope* _parent;

  Scope(Scope* parent = nullptr):
    _parent{parent}
  {
    // do nothing
  }

}; // Scope

using ScopePtr = ObjectPtr<Scope>;

} // end namespace symbol

} // end namespace mjc

#endif // __Scope_h
