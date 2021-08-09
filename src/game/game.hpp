/*
  University of Bologna
  First cicle degree in Computer Science
  00819 - Programmazione

  Andreea Scrob #989372
  07/08/2021

  game.hpp: declaration of the Game::Game class.
*/

#ifndef GAME_GAME_HPP
#define GAME_GAME_HPP
#include "../data/database.hpp"
#include "../engine/screen.hpp"
#include "../world/world.hpp"

namespace Game {
class Game {
private:
  Engine::Screen screen;
  bool running = true, in_game = false;
  Data::Database db;
  World::World world;
  bool loop();
  void change_content();
  void handle_keypress();
  int play_soundtrack(const char[]);

public:
  int run();
  Game();
};

} // namespace Game

#endif