#include "Class.h"
#include "GlobalScope.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

Class*
GlobalScope::lookupClass(const String& name) const
{
  return _classes.find(name);
}

Class* 
GlobalScope::addClass(const String& name, Class* superClass)
{
  return _classes.add(name, superClass);
}

} // end namespace mjc::symbol
