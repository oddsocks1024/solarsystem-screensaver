/*
Copyright (C) 2009 Ian Chapman

This program is free software; you can redistribute it and/or
modify it under the terms of the GNU General Public License
as published by the Free Software Foundation; either version 2
of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
*/
#include "solarsystem.h"

#define N_TEXTURES 12
GLuint textures[N_TEXTURES];

enum TEXTURES
{
    T_SUN,
    T_MERCURY,
    T_VENUS,
    T_EARTH,
    T_MARS,
    T_JUPITER,
    T_SATURN,
    T_URANUS,
    T_NEPTUNE,
    T_PLUTO,
    T_CORONA,
    T_RINGS
};

/* Protos */
SDL_Surface *loadTexture(char *filename);
void initTextures(void);
