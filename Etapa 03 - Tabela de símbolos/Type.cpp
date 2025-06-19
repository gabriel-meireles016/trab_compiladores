#include "Type.h"

namespace mjc::symbol
{ // begin namespace mjc::symbol

bool
Type::canCastTo(const Type* type) const
{
  return this == type;
}

} // end namespace mjc::symbol
