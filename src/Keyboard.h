#ifndef KEYBOARD_H
#define KEYBOARD_H

extern "C" {
#include <termios.h>
}

class Keyboard {
 public:
  Keyboard();
  ~Keyboard();
  bool kbhit();
  int getch();
  
 private:
  struct termios initial_settings, new_settings;
  int peek_character;
};

#endif
