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
#include <stdlib.h>
#include <time.h>

/* Generate a random integer between low and high inclusive */
int genRandInt(int low, int high) {
    static int seed = 1;
    int num;

    /* Gets called only once as seed is static */
    if (seed == 1) {
        srand(time(NULL));
        seed = 0;
    }

    /* Generate number between limits, inclusive */
    num = (rand() % (high - low + 1) + low);
    return (num);
}

/* Generate a random float between low and high inclusive */
float genRandFloat(float low, float high)
{
    static int seed = 1;
    float num;

    if (seed == 1) {
        srand(time(NULL));
        seed = 0;
    }

    num = (rand() / ((float)(RAND_MAX) + 1.0)) * (high - low) + low;
    return num;
}
