/*
  University of Bologna
  First cicle degree in Computer Science
  00819 - Programmazione

  Mattia Girolimetto #977478
  01/08/2021

  position.cpp: Implementation of World::WPosition.
*/

#include "position.hpp"
#include "../data/map_chunk.hpp"
#include "world.hpp"

using namespace Nostd;
using namespace Engine;

using World::Position;

Position::Position(const List<Matrix<Tile *>> *environment,
                   List<Matrix<Tile *>>::iterator fragment, int x, int y) {
  this->x = x;
  this->y = y;
  this->environment = environment;
  this->fragment = fragment;
}

List<Matrix<Tile *>>::iterator Position::get_fragment() const noexcept {
  return this->fragment;
}

bool Position::move_left() {
  if (x != 0)
    x--;
  else if (*fragment != *environment->begin()) {
    fragment = std::prev(fragment);
    x = (*fragment).extent(1) - 1;
  } else
    return false; // first column of the first chunk (and so we do nothing)
  return true;
}

bool Position::move_right() {
  if ((size_t)x != (*fragment).extent(1) - 1)
    x++;
  else {
    if (*fragment == *environment->end())
      return false; // last column of the last chunk (and so we do nothing)
    fragment = std::next(fragment);
    x = 0;
  }
  return true;
}

bool Position::move_up() {
  if (!y)
    return false; // first row (and so we do nothing)
  --y;
  return true;
}

bool Position::move_down() {
  if (y == (int)Data::MapChunk::height - 1) // last row (and so we do nothing)
    return false;
  ++y;
  return true;
}

int Position::get_x() const noexcept { return this->x; }

int Position::get_y() const noexcept { return this->y; }