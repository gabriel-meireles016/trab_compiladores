#ifndef __Class_h
#define __Class_h

#include "Scope.h"
#include "Type.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

class Class: public Type, public Scope
{
public:
  Class(const String&, Class*);

  Class* superClass() const
  {
    return _superClass;
  }

  Class* lookupClass(const String&) const override;
  Variable* lookupVariable(const String&) const override;
  Method* lookupMethod(const String&) const override;

  Class* addClass(const String&, Class*) override;
  Variable* addVariable(const String&, const Type*) override;
  Method* addMethod(const String&, const Type*, Class*) override;

  bool canCastTo(const Type*) const override;

private:
  ObjectPtr<Class> _superClass;
  SymbolMap<Class> _classes;
  SymbolMap<Variable> _variables;
  SymbolMap<Method> _methods;

}; // Class

inline auto
asClass(const Type* type)
{
  return dynamic_cast<const Class*>(type);
}

} // end namespace mjc::symbol

#endif // __Class_h