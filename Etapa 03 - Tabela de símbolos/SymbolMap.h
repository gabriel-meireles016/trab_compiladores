#ifndef __SymbolMap_h
#define __SymbolMap_h

#include "Symbol.h"
#include <map>

namespace mjc::symbol
{ // begin namespace mjc::symbol

template <typename T>
class SymbolMap
{
public:
  template <typename... Args>
  T* add(const String& name, Args&&... args)
  {
    if (find(name))
      return nullptr;

    auto symbol = new T{name, std::forward<Args>(args)...};

    _map.insert({name, symbol});
    return symbol;
  }

  T* find(const String& name) const
  {
    auto it = _map.find(name);
    return it != _map.end() ? (T*)it->second.get() : nullptr;
  }

  auto remove(const String& name)
  {
    return _map.erase(name) != 0;
  }

  void clear()
  {
    _map.clear();
  }

  auto size() const
  {
    return _map.size();
  }

  auto empty() const
  {
    return _map.empty();
  }

  auto begin() const
  {
    return _map.begin();
  }

  auto end() const
  {
    return _map.end();
  }

private:
  std::map<String, ObjectPtr<T>> _map;

}; // SymbolMap

} // end namespace mjc::symbol

#endif // __SymbolMap_h