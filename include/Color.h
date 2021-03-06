#pragma once

#include <string>
#include <vector>
#include <curses.h>

/*******************************************************************************

Manages colors for ncurses, making color pairs easy to reference by foreground
and background color.

*******************************************************************************/
class Color {

  private:

    struct color_entry {
      std::string label, name, basename, type;
      bool bright;
    };

    struct color_pair {
      color_entry fg, bg;
      int idx;
      bool use_bold, use_blink;
    };

    std::vector<color_pair> _colors;
    int _active_pair;

    /* Translate string color (red, green, etc.) into ncurses variable */

    void baseColorName(const std::string & colorname, std::string & basename,
                       bool & bright) const;
    int ncursesColor(const std::string & basename) const;

  public:

    /* Constructors */

    Color();

    /* Create or access color pair by foreground & background colors */

    void addPair(const std::string & fg_label, const std::string & fg_name,
                 const std::string & bg_label, const std::string & bg_name);
    int getPair(const std::string & fg_label,
                const std::string & bg_label) const;

    /* Bold / blink properties */

    bool pairIsBold(int vec_idx) const;
    bool pairBlinks(int vec_idx) const;

    /* Turn color setting on or off in window */

    int turnOn(WINDOW *win, const std::string & fg_label,
               const std::string & bg_label);
    int turnOn(WINDOW *win, int vec_idx);
    int turnOff(WINDOW *win);
    int setBackground(WINDOW *win, const std::string & fg_label,
                      const std::string & bg_label) const;
    int setBackground(WINDOW *win, int vec_idx) const;

    /* Clears all colors */

    void clear();
};
