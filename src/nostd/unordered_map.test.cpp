
/*
  University of Bologna
  First cicle degree in Computer Science
  00819 - Programmazione

  Mattia Girolimetto #977478
  04/01/2021

  unordered_map.test.cpp: tests for Nostd::UnorderedMap.
*/

#include "test.hpp"
#include "unordered_map.hpp"
#include <cassert>

using namespace Nostd;

int main() {
  it("puts elements into  an umap", [] {
    UnorderedMap<int, int> um;
    um.put(2, 5);
    um.put(4, 10);
    um.put(1, 23);
    um.put(5, 111);
  });
  it("operator []", [] {
    UnorderedMap<int, int> um;
    um.put(2, 5);
    um[2] = 10;
    assert(um[2] == 10);
  });

  it("gets values from a umap", [] {
    UnorderedMap<int, int> um;
    um.put(2, 5);
    um.put(4, 25);
    Vector<int> v = um.get_values();
    assert(v.size() == 2);
  });

  it("checks wheter an umap contains an element or not", [] {
    UnorderedMap<int, int> um;
    um.put(4, 5);
    um.put(14, 5);
    um.put(24, 5);
    um.put(34, 5);
    assert(um.contains(4));
    assert(!um.contains(5));
  });

  it("removes all element from an umap", [] {
    UnorderedMap<int, int> um;
    um.put(4, 5);
    um.put(10, 15);
    um.remove(4);
    assert(um.get_values().size() == 1);
  });

  it("clears the umap", [] {
    UnorderedMap<int, int> um;
    um.put(2, 5);
    um.put(4, 10);
    um.put(1, 23);
    um.put(5, 111);
    um.clear();
    assert(um.get_values().size() == 0);
  });
}