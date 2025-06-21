#include "GlobalScope.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

GlobalScope* 
Scope::global()
{
  static GlobalScope _global;
  return &_global;
}

Class*
Scope::lookupClass(const String& name) const
{
  return _parent ? _parent->lookupClass(name) : nullptr;
}

Variable*
Scope::lookupVariable(const String& name) const
{
  return _parent ? _parent->lookupVariable(name) : nullptr;
}

Method*
Scope::lookupMethod(const String& name) const
{
  return _parent ? _parent->lookupMethod(name) : nullptr;
}

Class*
Scope::addClass(const String&, Class*)
{
  return nullptr;
}

Variable*
Scope::addVariable(const String&, const Type*)
{
  return nullptr;
}

Method*
Scope::addMethod(const String&, const Type*, Class*)
{
  return nullptr;
}

} // end namespace mjc::symbol
