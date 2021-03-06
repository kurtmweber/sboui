#include <string>
#include <curses.h>
#include <cmath>    // floor
#include "Color.h"
#include "settings.h"
#include "signals.h"
#include "ListItem.h"
#include "SelectionBox.h"
#include "MouseEvent.h"

/*******************************************************************************

Constructors

*******************************************************************************/
SelectionBox::SelectionBox()
{
  _reserved_rows = 6;
  _header_rows = 3;
  addButton("    Ok    ", signals::keyEnter);
  addButton("  Cancel  ", signals::quit);
  setColor("fg_popup", "bg_popup");
  _modal = true;
  _external_hotkeys.resize(0);
}

SelectionBox::SelectionBox(WINDOW *win, const std::string & name)
{
  _win = win;
  _name = name;
  _reserved_rows = 6;
  _header_rows = 3;
  addButton("    Ok    ", signals::keyEnter);
  addButton("  Cancel  ", signals::quit);
  setColor("fg_popup", "bg_popup");
  _modal = true;
  _external_hotkeys.resize(0);
}

/*******************************************************************************

Get / set attributes

*******************************************************************************/
void SelectionBox::setModal(bool modal) { _modal = modal; }
bool SelectionBox::modal() const { return _modal; }
void SelectionBox::addExternalHotKey(char ch)
{
  _external_hotkeys.push_back(ch);
}
void SelectionBox::setExternalHotKeys(std::vector<char> hotkeys)
{
  _external_hotkeys = hotkeys;
}

/*******************************************************************************

User interaction

*******************************************************************************/
std::string SelectionBox::exec(MouseEvent * mevent)
{
  int ch, check_redraw, hotkey;
  char ch_char, hotcharN, hotcharL;
  std::string retval;
  bool getting_input, check_hotkeys;
  unsigned int i;
  MEVENT event;

  const int MY_ESC = 27;

  // Highlight first entry on first display

  if ( (_highlight == 0) && (_prevhighlight == 0) ) { highlightFirst(); }

  getting_input = true;
  while (getting_input)
  {

    check_hotkeys = false;

    // Draw list elements

    draw();

    // Get user input

    switch (ch = getch()) {

      // Enter key: accept selection

      case '\n':
      case '\r':
      case KEY_ENTER:
        if (int(_button_signals.size()) >= _highlighted_button+1)
          retval = _button_signals[_highlighted_button];
        else
          retval = signals::keyEnter;
        _redraw_type = "all";
        getting_input = false;
        break;

      // Arrows/Home/End/PgUp/Dn: change highlighted value

      case KEY_UP:
        check_redraw = highlightPrevious();
        if (check_redraw == 1) { _redraw_type = "all"; }
        else { _redraw_type = "changed"; }
        break;
      case KEY_DOWN:
        check_redraw = highlightNext();
        if (check_redraw == 1) { _redraw_type = "all"; }
        else { _redraw_type = "changed"; }
        break;
      case KEY_PPAGE:
        check_redraw = highlightPreviousPage();
        if (check_redraw == 1) { _redraw_type = "all"; }
        else { _redraw_type = "changed"; }
        break;
      case KEY_NPAGE:
        check_redraw = highlightNextPage();
        if (check_redraw == 1) { _redraw_type = "all"; }
        else { _redraw_type = "changed"; }
        break;
      case KEY_HOME:
        check_redraw = highlightFirst();
        if (check_redraw == 1) { _redraw_type = "all"; }
        else { _redraw_type = "changed"; }
        break;
      case KEY_END:
        check_redraw = highlightLast();
        if (check_redraw == 1) { _redraw_type = "all"; }
        else { _redraw_type = "changed"; }
        break;

      // Right/Left: change highlighted button

      case KEY_RIGHT:
        if (_buttons.size() > 0)
        {
          check_redraw = highlightNextButton();
          if (check_redraw == 1) { _redraw_type = "buttons"; }
          else { _redraw_type = "none"; }
        }
        else
        {
          retval = signals::keyRight;
          _redraw_type = "all";
          getting_input = false;
        }
        break;

      case KEY_LEFT:
        if (_buttons.size() > 0)
        {
          check_redraw = highlightPreviousButton();
          if (check_redraw == 1) { _redraw_type = "buttons"; }
          else { _redraw_type = "none"; }
        }
        else
        {
          retval = signals::keyLeft;
          _redraw_type = "all";
          getting_input = false;
        }
        break;

      // Resize signal

      case KEY_RESIZE:
        retval = signals::resize;
        _redraw_type = "all";
        getting_input = false;
        break;

      // Quit key

      case MY_ESC:
        retval = signals::quit;
        _redraw_type = "all";
        getting_input = false;
        break;

      // F9

      case KEY_F(9):
        retval = signals::keyF9;
        _redraw_type = "all";
        getting_input = false;
        break;

      // Mouse

      case KEY_MOUSE:
        if ( (getmouse(&event) == OK) && mevent )
        {
          mevent->recordClick(event);
          retval = handleMouseEvent(mevent);
          if ( (retval == signals::keyEnter) || (retval == signals::quit) )
          {
            getting_input = false;
            _redraw_type = "all";
          }
          else if ( (retval == signals::nullEvent) && (! _modal) )
          {
            retval = signals::mouseEvent;
            getting_input = false;
            _redraw_type = "all";
          }
        }
        break;

      default:
        _redraw_type = "none";
        check_hotkeys = true;
        getting_input = true;
        break;
    }

    // Handle hotkeys (allow upper and lower case)

    if (check_hotkeys)
    {
      ch_char = char(ch);
      for ( i = 0; i < numItems() + _external_hotkeys.size(); i++ )
      {
        if (i < numItems())
        {
          hotkey = _items[i]->hotKey();
          if (hotkey != -1)
          {
            hotcharN = _items[i]->name()[hotkey];
            hotcharL = std::tolower(_items[i]->name()[hotkey]);
            if ( (ch_char == hotcharN) || (ch_char == hotcharL) )
            {
              retval = hotcharN;
              _redraw_type = "all";
              getting_input = true;
              setHighlight(i);
              break;
            }
          }
        }
        else
        {
          hotcharN = _external_hotkeys[i-numItems()];
          hotcharL = std::tolower(_external_hotkeys[i-numItems()]);
          if ( (ch_char == hotcharN) || (ch_char == hotcharL) )
          {
            retval = hotcharN;
            _redraw_type = "all";
            getting_input = false;
            break;
          }
        }
      }
    }
  }
  return retval;
}
