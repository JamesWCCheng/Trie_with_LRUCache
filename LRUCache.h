#ifndef LRUCACHE_H
#define LRUCACHE_H

#include <unordered_map>
#include <queue>
#include <list>

template<class CacheKeyType, class CacheValueType, size_t SIZE = 10u>
class CLRUCache final
{
public:
  CLRUCache() = default;
  // Only allow same size copy or move
  CLRUCache(const CLRUCache& aRhs)
    : iKeyLRU(aRhs.iKeyLRU),
      iCacheMap(aRhs.iCacheMap)
  {
    RenewIterator();
  }

  CLRUCache(CLRUCache&& aRhs)
    : iKeyLRU(std::move(aRhs.iKeyLRU)),
      iCacheMap(std::move(aRhs.iCacheMap))
  {
    RenewIterator();
  }

  CLRUCache& operator = (CLRUCache aRhs)
  {
    iKeyLRU = std::move(aRhs.iKeyLRU);
    iCacheMap = std::move(aRhs.iCacheMap);
    RenewIterator();
    return *this;
  }

  void RenewIterator()
  {
    // renew list iterator in the map.
    typename std::list<CacheKeyType>::iterator itr = iKeyLRU.begin();
    while (itr != iKeyLRU.end())
    {
      iCacheMap[*itr].second = itr;
      itr++;
    }
  }

  std::pair<CacheValueType, bool> Get(CacheKeyType aKey) {
    auto itr = iCacheMap.find(aKey);
    if (itr == iCacheMap.end()) {
      return{ CacheValueType(), false }; // cache miss.
    }
    // update
    Update(itr);
    return{ itr->second.first, true };
  }

  void Put(CacheKeyType aKey, CacheValueType aValue) {
    auto itr = iCacheMap.find(aKey);
    if (itr == iCacheMap.end()) {
      // insert
      if (iCacheMap.size() == SIZE) {
        // delete the least recently used element entry from the map.
        iCacheMap.erase(iKeyLRU.back());
        // delete the value in the list also.
        iKeyLRU.pop_back();
      }
      iKeyLRU.push_front(aKey);
    }
    else {
      // already exist
      Update(itr);
    }
    iCacheMap[aKey] = { aValue, iKeyLRU.begin() };
  }
private:
  using CacheMapType =
    std::unordered_map<CacheKeyType, std::pair<CacheValueType, typename std::list<CacheKeyType>::iterator>>;
  using CacheMapEntry = typename CacheMapType::iterator;
  void Update(CacheMapEntry aItr) {
    auto key = aItr->first;
    // remove the original one.
    iKeyLRU.erase(aItr->second.second);
    // push to the front
    iKeyLRU.push_front(key);
    aItr->second.second = iKeyLRU.begin();
  }
  std::list<CacheKeyType> iKeyLRU;
  CacheMapType iCacheMap;
};

#endif