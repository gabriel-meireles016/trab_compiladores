#ifndef __BlockScope_h
#define __BlockScope_h

#include "Scope.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

class BlockScope: public Scope
{
public:
  BlockScope(Scope* parent, const Class* clazz):
    Scope{parent},
    _enclosingClass{clazz}
  {
    // do nothing
  }

  Variable* lookupVariable(const String&) const override;
  Variable* addVariable(const String&, const Type*) override;

  auto enclosingClass() const
  {
    return _enclosingClass;
  }

protected:
  SymbolMap<Variable> _variables;
  const Class* _enclosingClass;

}; // BlockScope

using BlockScopePtr = ObjectPtr<BlockScope>;

} // end namespace mjc::symbol

#endif // __BlockScope_h
