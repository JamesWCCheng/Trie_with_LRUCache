#ifndef TRIE_H
#define TRIE_H

#include <iostream>
#include <memory>
#include <string>

#include "LRUCache.h"

struct CTrieNode
{
  using NodeValueType = char;
  CTrieNode() :
    iIsWord(false)
  {
  }

  bool iIsWord;
  std::unordered_map<NodeValueType, std::unique_ptr<CTrieNode>> iChildren;
};

struct CTrie
{
  CTrie() :
    //iRoot(std::make_unique<CTrieNode>())
    iRoot(new CTrieNode())
  {
  }

  void Insert(const std::string& aPrefix)
  {
    auto current = iRoot.get();
    for (char c : aPrefix)
    {
      // std::pair<iterator,bool>, be true when the insertion took place.
      auto check = current->iChildren.emplace(c, nullptr);
      if (check.second)
      {
        current->iChildren[c].reset(new CTrieNode());
      }
      current = current->iChildren[c].get();
    }
    current->iIsWord = true;
  }

  void Insert(const std::vector<std::string>& aPrefixVec)
  {
    for (const auto& pre : aPrefixVec)
    {
      Insert(pre);
    }
  }

  std::vector<std::string> Suggest(const std::string& aPrefix)
  {
    auto&& cache = iCache.Get(aPrefix);
    if (cache.second)
    {
      std::cout << "cache hit with prefix = " << aPrefix << std::endl;
      return cache.first;
    }
    auto current = iRoot.get();
    // traverse to the last valid word starting from aPrefix.

    std::string validPrefix;
    for (char c : aPrefix)
    {
      if (current->iChildren.find(c) == current->iChildren.end())
      {
        break;
      }
      validPrefix.push_back(c);
      current = current->iChildren[c].get();
    }
    std::cout << "valid prefix = " << validPrefix << std::endl;
    // Cannot find any words starting with aPrefix.
    if (validPrefix.empty())
    {
      return std::vector<std::string>{};
    }

    std::vector<std::string> result;
    // BFS
    std::queue<std::pair<std::string, CTrieNode*>> q;
    q.push({ validPrefix, current });

    while (!q.empty())
    {
      auto p = q.front();
      q.pop();
      if (p.second->iIsWord)
      {
        result.push_back(p.first);
      }

      for (auto& trieNode : p.second->iChildren) {
        q.push({ p.first + trieNode.first, trieNode.second.get() });
      }
    }

    iCache.Put(validPrefix, result);
    return result;
  }

private:
  CLRUCache<std::string, std::vector<std::string>> iCache;
  std::unique_ptr<CTrieNode> iRoot;
};

#endif