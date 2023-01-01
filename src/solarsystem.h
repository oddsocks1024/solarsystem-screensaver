/*
Copyright (C) 2009-2010 Ian Chapman

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
#include "config.h"
#ifdef HAVE_GL_GL_H
#include <GL/gl.h>
#else
#include <OpenGL/gl.h>
#endif
#ifdef HAVE_GL_GLU_H
#include <GL/glu.h>
#else
#include <OpenGL/glu.h>
#endif
#include "SDL.h"

/* Draw a frame every 30ms (~ 33fps) */
#define FRAME_INTERVAL 30

/* Time to next model switch in ms */
#define MODEL_SWITCH 60000

/* Starting point to pan up from */
#define PANSTART -5.0f

/* End point to pan up from */
#define PANSTOP 0.0f

/* Star Plane Size (actually its half width -x to +x) */
#define STARPSIZE 55.0f

/* Number of Stars */
#define STARNUM 1000

/* Number of planet slices */
#define PSLICES 64

/* Number of planet stacks */
#define PSTACKS 64

/* Protos */
int _exit_(int code);
