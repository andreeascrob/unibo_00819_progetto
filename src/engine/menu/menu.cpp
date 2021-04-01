/*
  University of bologna
  First cicle degree in Computer Science
  00819 - Programmazione

  Luca Tagliavini #971133
  03/31/2021

  menu.hpp: Implements the Engine::Menu::Menu shared methods across all menus
  subclasses. This implementation should allow menus not to have any focusable
  item, that's why we take care and check each Box* for nullness.
*/
#include "menu.hpp"
#include "../screen.hpp"
#include <exception>

void Engine::Menu::Menu::redraw() {
  // assert the root is not null by
  // - attempting to generate it for the first time
  // - throwing if the generator does not return a valid value
  if (first_draw) {
    root = generate();
    if (root == nullptr)
      throw new std::bad_alloc();

    Engine::UI::Box *ele = curr_box();
    if (ele != nullptr)
      focus(ele);

    first_draw = false;
  }

  root->show(window, 1, 1);
  wrefresh(window);
}

Engine::Menu::Menu::Menu(WINDOW *window)
    : Drawable(window, Screen::SCREEN_COLS, Screen::SCREEN_LINES) {
  this->drawable_kind = Kind::menu;
}

Engine::Menu::Menu::~Menu() { delete root; }

void Engine::Menu::Menu::handle_event(Event e) {
  switch (e) {
  case Event::redraw:
    redraw();
    break;

  case Event::move_up:
  case Event::move_down: {
    Engine::UI::Box *tmp = curr_box();
    if (tmp != nullptr)
      unfocus(tmp);

    tmp = e == Event::move_up ? prev_box() : next_box();
    if (tmp != nullptr)
      focus(tmp);

    redraw();
    break;
  }

  // TODO: for more complex menus. to be handeld manually (?)
  // will close this out when we do the settings menu
  case Event::move_left:
  case Event::move_right:
    break;
  };
}
