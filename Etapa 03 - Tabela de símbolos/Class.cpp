#include "Class.h"
#include "Method.h"
#include "GlobalScope.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

inline Scope*
parentScope(Class* superClass)
{
  if (superClass)
    return superClass;
  return Scope::global();
}

Class::Class(const String& name, Class* superClass):
  Type{name},
  Scope{parentScope(superClass)},
  _superClass{superClass}
{
  //do nothing
}

Class* 
Class::lookupClass(const String& name) const
{
  auto c = _classes.find(name);
  return c ? c : _parent->lookupClass(name);
}

Variable*
Class::lookupVariable(const String& name) const
{
  auto v = _variables.find(name);
  return v ? v: _parent->lookupVariable(name);
}

Method*
Class::lookupMethod(const String& name) const
{
  auto m = _methods.find(name);
  return m ? m : _parent->lookupMethod(name);
}

Class* 
Class::addClass(const String& name, Class* superClass)
{
  return _classes.add(name, superClass);
}

Variable* 
Class::addVariable(const String& name, const Type* type)
{
  return _variables.add(name, type);
}

Method*
Class::addMethod(const String& name, const Type* returnType, Class* clazz)
{
  return _methods.add(name, returnType, clazz);
}

bool
Class::canCastTo(const Type* type) const
{
  if (auto to = asClass(type))
    for (auto from = this; from; from = from->_superClass)
      if (from == to)
        return true;
  return false;
}

} // end namespace mjc::symbol