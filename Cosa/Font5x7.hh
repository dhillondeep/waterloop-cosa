/**
 * @file Cosa/Font5x7.hh
 * @version 1.0
 *
 * @section License
 * Copyright (C) 2012, Mikael Patel
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 * 
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 * 
 * You should have received a copy of the GNU Lesser General
 * Public License along with this library; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place, Suite 330,
 * Boston, MA  02111-1307  USA
 *
 * @section Description
 * Bitmap font 5x7.
 *
 * This file is part of the Arduino Che Cosa project.
 */

#ifndef __COSA_FONT5X7_HH__
#define __COSA_FONT5X7_HH__

#include "Cosa/Font.hh"

class Font5x7 : public Font {

private:
  static uint8_t bitmap[] PROGMEM;

public:
  Font5x7() : Font(5, 8, bitmap) {}
};

extern Font5x7 font5x7;

#endif
