/*
  University of Bologna
  First cicle degree in Computer Science
  00819 - Programmazione

  Luca Tagliavini #971133
  02/21/2021

  screen.cpp: implementation for the Screen class, setting up the ncurses' box
  to contain the content and implementing other lifecycle initial/final setps in
  the game engine (screen initialization/clearing/freeing/...)
*/

#include "screen.hpp"
#include "menu/main.hpp"
#include <curses.h>

Engine::Screen::Screen() {
  this->content = nullptr;
  this->stdscreen = nullptr;
  this->container = nullptr;
}
Engine::Screen::~Screen() {
  close();
  clear_content();
}

Engine::Drawable::Kind Engine::Screen::get_state() {
  if (content == nullptr)
    return Engine::Drawable::Kind::none;

  return content->kind();
}
bool Engine::Screen::is_over() {
  if (content == nullptr)
    return false; // undefined behaviour TODO: exception
  return content->is_over();
}

void Engine::Screen::send_event(Drawable::Event e) {
  if (content != nullptr)
    content->handle_event(e);
}

WINDOW *Engine::Screen::get_container() { return container; }

bool Engine::Screen::can_fit() {
  getmaxyx(stdscreen, terminal_lines, terminal_cols);

  // check that the terminal is big enough to fit the game
  if (terminal_cols < columns || terminal_lines < lines)
    return false;

  x = (terminal_cols - columns) / 2;
  y = (terminal_lines - lines) / 2;
  return true;
}

bool Engine::Screen::open() {
  stdscreen = initscr();
  if (stdscreen == nullptr || start_color())
    return false;

  // NOTE: look at man curses(3) for documentation on these functions
  use_default_colors(); // allows to use -1 as a background neutral color
  noecho(); // prevents user-inputted charters to be displayed on the stdscreen
  nodelay(stdscreen, true); // makes getch non-blocking
  raw();       // ncurses will report any keystrokes with getch immediately
  curs_set(0); // hide the cursor by default
  keypad(
      stdscreen,
      true); // `true` is used to caputre arrow keys and other special sequences

  if (container != nullptr)
    delwin(container);

  outer_box = newwin(lines + 2, columns + 2, y, x);
  container = newwin(lines, columns, y + 1, x + 1);
  return reposition();
}

bool Engine::Screen::reposition() {
  if (!can_fit())
    return false;

  erase();   // clear the stdscreen and
  refresh(); // send the changes to the user so we can start drawing

  mvwin(outer_box, y, x);
  box(outer_box, ACS_VLINE, ACS_HLINE);
  wnoutrefresh(outer_box);

  mvwin(container, y + 1, x + 1);
  send_event(Engine::Drawable::Event::redraw);
  // force redrawing to make the container window stand above the outer box
  // window
  redrawwin(container);

  // do two refreshes in one
  wnoutrefresh(container);
  doupdate();

  return true;
}

void Engine::Screen::close() {
  delwin(outer_box);
  delwin(container);
  endwin();
  stdscreen = nullptr;
  container = nullptr;
}

void Engine::Screen::clear_content() {
  // free the previous content data
  if (content != nullptr)
    delete content;
  content = nullptr;
  werase(container);
  wrefresh(container);
}
