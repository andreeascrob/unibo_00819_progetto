#include "screen.hpp"
#include <iostream>
#include <ncurses.h>
using namespace std;

// screen wraps the whole ncurses screen in a box of SCREEN_WIDTHxSCREEN_HEIGHT
// that is centered on the screen and repositioned properly on resizes
//
// Furthermore the screen class keeps track of the terminal dimentions
// to center boxes as we see fit and also calls helper functions like
// `raw`, `keypad` and `noecho` to properly get the user input.

bool Engine::Screen::measure() {
  getmaxyx(terminal, terminal_lines, terminal_cols);

  // check that the terminal is big enough to fit the game
  if (terminal_cols < SCREEN_COLS || terminal_lines < SCREEN_LINES)
    return true;

  terminal_offset_cols = (terminal_cols - SCREEN_COLS) / 2;
  terminal_offset_lines = (terminal_lines - SCREEN_LINES) / 2;

  return false;
}

bool Engine::Screen::open() {
  terminal = initscr(); // terminal should be equal to stdscr
  if (terminal == NULL)
    return true;

  clear();   // clear the screen and
  refresh(); // send the changes to the user so we can start drawing

  noecho(); // prevents user-inputted charters to be displayed on the screen
  raw();    // with raw we intercept all keystrokes and prevent ^C from quitting
            // the game
  keypad(
      terminal,
      false); // the second argument is set to `false` to caputre RESIZE events

  if (measure())
    return true;

  window = newwin(SCREEN_LINES, SCREEN_COLS, terminal_offset_lines,
                  terminal_offset_cols);
  box(window, ACS_VLINE, ACS_HLINE);
  wrefresh(window);
  return false;
}

bool Engine::Screen::recenter() {
  if (measure())
    return true;

  // clear the previous screen
  clear();
  refresh();

  mvwin(window, terminal_offset_lines, terminal_offset_cols);
  wrefresh(window);

  return false;
}

void Engine::Screen::close() { endwin(); }