#ifndef __Method_h
#define __Method_h

#include "BlockScope.h"
#include "Class.h"
#include "Variable.h"
#include <list>

namespace mjc::symbol
{ // begin namespace mjc::symbol

class Method: public Symbol
{
public:
  using Parameters = std::list<VariablePtr>;

  Method(const String& name, const Type* returnType, Class* clazz):
    Symbol{name},
    _returnType{returnType},
    _scope{new BlockScope{clazz, clazz}}
  {
    // do nothing
  }

  auto returnType() const
  {
    return _returnType;
  }

  Scope* scope() const
  {
    return _scope;
  }

  auto addParameter(const String& name, const Type* type)
  {
    auto p = _scope->addVariable(name, type);

    if (p != nullptr)
      _parameters.push_back(p);
    return p;
  }

  auto& parameters() const
  {
    return _parameters;
  }

private:
  const Type* _returnType;
  Parameters _parameters;
  BlockScopePtr _scope;

}; // Method

} // end namespace mjc::symbol

#endif // __Method_h
