#ifndef __GlobalScope_h
#define __GlobalScope_h

#include "Class.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

class GlobalScope: public Scope
{
public:
  GlobalScope()
  {
    makeUse(this);
  }

  Class* lookupClass(const String&) const override;
  Class* addClass(const String&, Class*) override;

private:
  SymbolMap<Class> _classes;

}; // GlobalScope

} // end namespace mjc::symbol

#endif // __GlobalScope_h
