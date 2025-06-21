#include "BlockScope.h"
#include "Variable.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

Variable*
BlockScope::lookupVariable(const String& name) const
{
  auto v = _variables.find(name);
  return v ? v : _parent->lookupVariable(name);
}

Variable*
BlockScope::addVariable(const String& name, const Type* type)
{
  return _variables.add(name, type);
}

} // end namespace mjc::symbol