#include <string>
#include <curses.h>
#include "Color.h"
#include "settings.h"
#include "InputItem.h"
#include "Label.h"

/*******************************************************************************

Constructor

*******************************************************************************/
Label::Label()
{
  _redraw_type = "label";
  _item_type = "Label";
  _selectable = false;
  _color_idx = -1;
}

/*******************************************************************************

Set attributes

*******************************************************************************/
void Label::setColor(int color_idx) { _color_idx = color_idx; }

/*******************************************************************************

Draws label

*******************************************************************************/
void Label::draw(int y_offset, bool force, bool highlight)
{
  wmove(_win, _posy-y_offset, _posx);

  colors.turnOn(_win, _color_idx);
  wprintw(_win, _name.c_str());
  colors.turnOff(_win);

  wrefresh(_win);
}
