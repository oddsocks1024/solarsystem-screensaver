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
#define N_MODELS 10
GLuint modelDL[N_MODELS];
GLUquadricObj *sphereQuadric, *diskQuadric;

/* Object Model Enums */
enum MODELS
{
    M_SUN,
    M_MERCURY,
    M_VENUS,
    M_EARTH,
    M_MARS,
    M_JUPITER,
    M_SATURN,
    M_URANUS,
    M_NEPTUNE,
    M_PLUTO
};

struct planetoid
{
    float tilt; // maybe remove
    float corona; // maybe remove
    float parentsize; //maybe remove
    float size;
    float orbitsize;
    float spin;
    float spinspeed;
    float orbit;
    float orbitspeed;
};

struct planetoid planets[N_MODELS];

struct starfield
{
    float x;
    float y;
    float z;
    float size;
    int r;
    int g;
    int b;
};
