#include "LRUCache.h"
#include "Trie.h"

using namespace std;

int main()
{
  // Test LRU
  CLRUCache<int, int> q;
  q.Put(1, 5566);
  q.Put(2, 7788);
  std::cout << q.Get(1).first << q.Get(2).first << std::endl;
  auto qq = q;
  std::cout << qq.Get(1).first << qq.Get(2).first << std::endl;
  auto qqq = std::move(q);
  std::cout << qqq.Get(1).first << qqq.Get(2).first << std::endl;
  CLRUCache<int, int> q2;
  CLRUCache<int, int> q3;
  q2 = qq;
  q3 = std::move(qq);
  std::cout << q2.Get(1).first << q2.Get(2).first << std::endl;
  std::cout << q3.Get(1).first << q3.Get(2).first << std::endl;

  // Test Trie
  string input[] =
  {
    "A.B.C",
    "A.C.D.E",
    "AA.BB.CC",
    "B.B.Q",
    "BB",
  };
  CTrie trie;
  cout << "== Dictionary ==" << endl;
  for (auto s : input)
  {
    trie.Insert(s);
    cout << s << endl;
  }
  cout << "=============" << endl;
  cout << "A suggests?" << endl;
  const auto& result = trie.Suggest("A");
  for (auto& s : result)
  {
    cout << s << endl;
  }
  cout << "=============" << endl;
  cout << "BB suggests?" << endl;
  const auto& result2 = trie.Suggest("BB");
  for (auto& s : result2)
  {
    cout << s << endl;
  }
  cout << "=============" << endl;
  cout << "B suggests?" << endl;
  const auto& result3 = trie.Suggest("B");
  for (auto& s : result3)
  {
    cout << s << endl;
  }
  cout << "=============" << endl;
  cout << " suggests?" << endl;
  const auto& result4 = trie.Suggest("");
  for (auto& s : result4)
  {
    cout << s << endl;
  }
  cout << "=============" << endl;
  cout << "B suggests?" << endl;
  const auto& result5 = trie.Suggest("B");
  for (auto& s : result5)
  {
    cout << s << endl;
  }

  getchar();
  return 0;
}