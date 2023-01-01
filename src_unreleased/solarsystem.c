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
#include "solarsystem_random.h"
#include "solarsystem_models.h"
#include "solarsystem_textures.h"
#include "solarsystem_timers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <math.h>


struct starfield stars[STARNUM];
static GLubyte starcols[12][3] =
{
    {160,0,0}, {200,200,0}, {0,100,100},
    {150,0,0}, {150,150,0}, {0,100,150},
    {100,0,0}, {100,100,0}, {0,100.100},
    {255,255,255}, {200,200,200}, {150,150,150},
};

/* Generate a star plane */
void initStars(GLfloat psize)
{
    int i, rn;

    for (i = 0 ; i < STARNUM ; i++) {
        rn = genRandInt(0,11);
        stars[i].x = genRandFloat(-psize,psize);
        stars[i].y = genRandFloat(-psize,psize);
        stars[i].z = -psize;
        stars[i].size = genRandFloat(0.1f, 3.0f);
        stars[i].r = starcols[rn][0];
        stars[i].g = starcols[rn][1];
        stars[i].b = starcols[rn][2];
    }
}

float deg2rad(float deg)
{
    return (deg * PI / 180);
}

void initPlanets()
{
    planets[M_SUN].size = 15.0f;
    planets[M_SUN].orbitsize = 0.0f;
    planets[M_SUN].spin = 0.0f;
    planets[M_SUN].spinspeed = 3.0f;
    planets[M_SUN].orbit = 0.0f;
    planets[M_SUN].orbitspeed = 0.0f;

    planets[M_MERCURY].size = 0.38f;
    planets[M_MERCURY].orbitsize = 18.87f;
    planets[M_MERCURY].spin = 0.0f;
    planets[M_MERCURY].spinspeed = 0.1f;
    planets[M_MERCURY].orbit = 0.0f;
    planets[M_MERCURY].orbitspeed = 3.21f;

    planets[M_VENUS].size = 0.95f;
    planets[M_VENUS].orbitsize = 22.23f;
    planets[M_VENUS].spin = 0.0f;
    planets[M_VENUS].spinspeed = 0.1f;
    planets[M_VENUS].orbit = 0.0f;
    planets[M_VENUS].orbitspeed = 2.35f;

    planets[M_EARTH].size = 1.0f;
    planets[M_EARTH].orbitsize = 25.0f;
    planets[M_EARTH].spin = 0.0f;
    planets[M_EARTH].spinspeed = 1.0f;
    planets[M_EARTH].orbit = 0.0f;
    planets[M_EARTH].orbitspeed = 2.0f;

    planets[M_MARS].size = 0.53f;
    planets[M_MARS].orbitsize = 30.24f;
    planets[M_MARS].spin = 0.0f;
    planets[M_MARS].spinspeed = 0.98f;
    planets[M_MARS].orbit = 0.0f;
    planets[M_MARS].orbitspeed = 1.62f;

    planets[M_JUPITER].size = 6.11f;
    planets[M_JUPITER].orbitsize = 46.23f;
    planets[M_JUPITER].spin = 0.0f;
    planets[M_JUPITER].spinspeed = 2.0f;
    planets[M_JUPITER].orbit = 0.0f;
    planets[M_JUPITER].orbitspeed = 0.88f;

    planets[M_SATURN].size = 4.73f;
    planets[M_SATURN].orbitsize = 72.49f;
    planets[M_SATURN].spin = 0.0f;
    planets[M_SATURN].spinspeed = 2.0f;
    planets[M_SATURN].orbit = 0.0f;
    planets[M_SATURN].orbitspeed = 0.65f;

    planets[M_URANUS].size = 2.00f;
    planets[M_URANUS].orbitsize = 91.92f;
    planets[M_URANUS].spin = 0.0f;
    planets[M_URANUS].spinspeed = 1.39f;
    planets[M_URANUS].orbit = 0.0f;
    planets[M_URANUS].orbitspeed = 0.46f;

    planets[M_NEPTUNE].size = 1.95f;
    planets[M_NEPTUNE].orbitsize = 105.31f;
    planets[M_NEPTUNE].spin = 0.0f;
    planets[M_NEPTUNE].spinspeed = 1.49f;
    planets[M_NEPTUNE].orbit = 0.0f;
    planets[M_NEPTUNE].orbitspeed = 0.36f;

    planets[M_PLUTO].size = 0.18f;
    planets[M_PLUTO].orbitsize = 133.45f;
    planets[M_PLUTO].spin = 0.0f;
    planets[M_PLUTO].spinspeed = 0.16f;
    planets[M_PLUTO].orbit = 0.0f;
    planets[M_PLUTO].orbitspeed = 0.31f;
}


/* Cache as much as possible */
void preCacheScene(int model)
{

}

/* Try to exit cleanly */
int _exit_(int code)
{
    int i;

    if(SDL_WasInit(SDL_INIT_VIDEO) != 0) {
        for (i = 0; i < N_MODELS ; i++) {
            glDeleteLists(modelDL[i], 1);
        }
        glDeleteTextures(N_TEXTURES,textures);
        SDL_Quit();
    }
    exit(code);
}

/* Initialise OpenGL */
void initOpenGL(int w, int h)
{
    glViewport(0, 0, w, h);
    initStars(STARPSIZE); /* Set up the stars */
    initTextures();
    sphereQuadric = gluNewQuadric();
    diskQuadric = gluNewQuadric();
    gluQuadricOrientation(sphereQuadric, GLU_OUTSIDE); /* Normals point outwards */
    gluQuadricNormals(sphereQuadric, GLU_SMOOTH); /* Create Smooth Normals */
    gluQuadricTexture(sphereQuadric, GL_TRUE); /* Enable Texturing for quadrics */
    gluQuadricOrientation(diskQuadric, GLU_INSIDE); /* Normals point outwards */
    gluQuadricNormals(diskQuadric, GLU_SMOOTH); /* Create Smooth Normals */
    gluQuadricTexture(diskQuadric, GL_TRUE); /* Enable Texturing for quadrics */
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_BLEND);
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClearDepth(1.0);
    glDepthFunc(GL_LEQUAL);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, (GLfloat) w/ (GLfloat) h, 0.1f, 270.0f); /* Calc Aspect Ratio */
    glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    GLfloat Ambient[4]={0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

float planetx(int planet)
{
    float x;
    //x = (planets[planet].orbitsize + planets[planet].size) * sinf(deg2rad(planets[planet].orbit + 90.0f));
    x = (planets[planet].orbitsize) * sinf(deg2rad(planets[planet].orbit + 90.0f));
    return x;
}

float planety(int planet)
{
    float y;
    //y = (planets[planet].orbitsize + planets[planet].size) * cosf(deg2rad(planets[planet].orbit + 90.0f));
    y = (planets[planet].orbitsize) * cosf(deg2rad(planets[planet].orbit + 90.0f));
    return y;
}

/* Draw a Frame */
void drawScene(int model)
{
    static GLfloat LightPos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    static GLfloat camrot = 0.0f;
    camrot += 0.1f;
    if (camrot > 360.0f) { camrot = 0.0f;}
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();

    //gluLookAt(planetx(M_EARTH), 0.0f, planety(M_EARTH), 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f);
    //gluLookAt(planetx(M_EARTH) + (2.0f * sinf(deg2rad(90.0f))), 0.0f, planety(M_EARTH)+ (2.0f * sinf(deg2rad(90.0f))), planetx(M_EARTH), 0.0f, planety(M_EARTH), 1.0f, 0.0f, 0.0f);
    gluLookAt(planetx(M_URANUS) + (15.0f * sinf(deg2rad(90.f))), 0.0f, planety(M_URANUS)+ (15.0f * sinf(deg2rad(90.0f))), planetx(M_URANUS), 0.0f, planety(M_URANUS), 0.0f, 1.0f, 0.0f);

    //glPushMatrix();
    glRotatef(90.0f, 1.0f, 0.0f, 0.0f); // Flip Upright

    // Sun
    glPushMatrix();
    glRotatef(planets[M_SUN].spin, 0.0f, 0.0f, -1.0f); // Spin CCW
    planets[M_SUN].spin += planets[M_SUN].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_SUN]);
    gluSphere(sphereQuadric, planets[M_SUN].size, PSLICES, PSTACKS);
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos);
    glEnable(GL_LIGHTING);
    glPopMatrix();

    // Mercury
    glPushMatrix();
    glRotatef(planets[M_MERCURY].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_MERCURY].orbit += planets[M_MERCURY].orbitspeed;
    glTranslatef(planets[M_MERCURY].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_MERCURY].spin, 0.0f, 0.0f, -1.0f);
    planets[M_MERCURY].spin += planets[M_MERCURY].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_MERCURY]);
    gluSphere(sphereQuadric, planets[M_MERCURY].size, PSLICES, PSTACKS);
    glPopMatrix();

    // Venus
    glPushMatrix();
    glRotatef(planets[M_VENUS].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_VENUS].orbit += planets[M_VENUS].orbitspeed;
    glTranslatef(planets[M_VENUS].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_VENUS].spin, 0.0f, 0.0f, 1.0f);
    planets[M_VENUS].spin += planets[M_VENUS].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_VENUS]);
    gluSphere(sphereQuadric, planets[M_VENUS].size, PSLICES, PSTACKS);
    glPopMatrix();

    // Earth
    glPushMatrix();
    glRotatef(planets[M_EARTH].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_EARTH].orbit += planets[M_EARTH].orbitspeed;
    if (planets[M_EARTH].orbit > 360.0f) { planets[M_EARTH].orbit = 0.0f; }
    glTranslatef(planets[M_EARTH].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_EARTH].spin, 0.0f, 0.0f, -1.0f);
    planets[M_EARTH].spin += planets[M_EARTH].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_EARTH]);
    gluSphere(sphereQuadric, planets[M_EARTH].size, PSLICES, PSTACKS);
    glPopMatrix();

    // Mars
    glPushMatrix();
    glRotatef(planets[M_MARS].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_MARS].orbit += planets[M_MARS].orbitspeed;
    glTranslatef(planets[M_MARS].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_MARS].spin, 0.0f, 0.0f, -1.0f);
    planets[M_MARS].spin += planets[M_MARS].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_MARS]);
    gluSphere(sphereQuadric, planets[M_MARS].size, PSLICES, PSTACKS);
    glPopMatrix();

    // Jupiter
    glPushMatrix();
    glRotatef(planets[M_JUPITER].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_JUPITER].orbit += planets[M_JUPITER].orbitspeed;
    glTranslatef(planets[M_JUPITER].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_JUPITER].spin, 0.0f, 0.0f, -1.0f);
    planets[M_JUPITER].spin += planets[M_JUPITER].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_JUPITER]);
    gluSphere(sphereQuadric, planets[M_JUPITER].size, PSLICES, PSTACKS);
    glPopMatrix();

    // Saturn
    glPushMatrix();
    glRotatef(planets[M_SATURN].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_SATURN].orbit += planets[M_SATURN].orbitspeed;
    glTranslatef(planets[M_SATURN].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_SATURN].spin, 0.0f, 0.0f, -1.0f);
    planets[M_SATURN].spin += planets[M_SATURN].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_SATURN]);
    gluSphere(sphereQuadric, planets[M_SATURN].size, PSLICES, PSTACKS);
    glPopMatrix();

    // Uranus
    glPushMatrix();
    glRotatef(planets[M_URANUS].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_URANUS].orbit += planets[M_URANUS].orbitspeed;
    glTranslatef(planets[M_URANUS].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_URANUS].spin, 0.0f, 0.0f, -1.0f);
    planets[M_URANUS].spin += planets[M_URANUS].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_URANUS]);
    gluSphere(sphereQuadric, planets[M_URANUS].size, PSLICES, PSTACKS);
    glPopMatrix();

    // Uranus
    //glPushMatrix();
    //glRotatef(90.0f, -1.0f, 0.0f, 0.0f);
    //glRotatef(planets[M_URANUS].orbit, 1.0f, 0.0f, 0.0f);
    //planets[M_URANUS].orbit += planets[M_URANUS].orbitspeed;
    //glTranslatef(planets[M_URANUS].orbitsize, 0.0f, 0.0f);
    //glRotatef(planets[M_URANUS].spin, 1.0f, 0.0f, 0.0f);
    //planets[M_URANUS].spin += planets[M_URANUS].spinspeed;
    //glBindTexture(GL_TEXTURE_2D, textures[T_URANUS]);
    //gluSphere(sphereQuadric, planets[M_URANUS].size, PSLICES, PSTACKS);
    //glPopMatrix();

    // Neptune
    glPushMatrix();
    glRotatef(planets[M_NEPTUNE].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_NEPTUNE].orbit += planets[M_NEPTUNE].orbitspeed;
    glTranslatef(planets[M_NEPTUNE].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_NEPTUNE].spin, 0.0f, 0.0f, -1.0f);
    planets[M_NEPTUNE].spin += planets[M_NEPTUNE].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_NEPTUNE]);
    gluSphere(sphereQuadric, planets[M_NEPTUNE].size, PSLICES, PSTACKS);
    glPopMatrix();

    // Pluto
    glPushMatrix();
    glRotatef(planets[M_PLUTO].orbit, 0.0f, 0.0f, -1.0f);
    planets[M_PLUTO].orbit += planets[M_PLUTO].orbitspeed;
    glTranslatef(planets[M_PLUTO].orbitsize, 0.0f, 0.0f);
    glRotatef(planets[M_PLUTO].spin, 0.0f, 0.0f, -1.0f);
    planets[M_PLUTO].spin += planets[M_PLUTO].spinspeed;
    glBindTexture(GL_TEXTURE_2D, textures[T_PLUTO]);
    gluSphere(sphereQuadric, planets[M_PLUTO].size, PSLICES, PSTACKS);
    glPopMatrix();


    glPopMatrix();
    glDisable(GL_LIGHTING);

    SDL_GL_SwapBuffers();
}

void mainloop(int scr_w, int scr_h)
{
    SDL_TimerID timerMain;
    SDL_TimerID timerModelSwitch;
    SDL_Event event;
    SDL_Surface* mainSurface;
    int _quit = 0;
    int model = genRandInt(0, (N_MODELS -1)); /* Pick a random model */

    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        fprintf(stderr, "Unable to initialize SDL: %s\n", SDL_GetError());
        _exit_(1);
    }

#if SDL_PATCHLEVEL > 12
    SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );
#endif

    /* Some SDL installations don't default to desktop display size if 0 is passed */
    /* so if we fail, try with some defaults */
    if ((mainSurface = SDL_SetVideoMode(scr_w, scr_h, 0, SDL_OPENGL)) == NULL) {
        scr_w = 640;
        scr_h = 480;
        if ((mainSurface = SDL_SetVideoMode(scr_w, scr_h, 0, SDL_OPENGL)) == NULL) {
            fprintf(stderr, "Unable to create OpenGL screen: %s\n", SDL_GetError());
            _exit_(2);
        }
    }

    SDL_WM_SetCaption("SolarSystem Screensaver", NULL);
    initOpenGL(mainSurface->w, mainSurface->h); /* Init OpenGL view port */
    initPlanets();
    //preCacheScene(model);
    timerMain = SDL_AddTimer(FRAME_INTERVAL, _frametimer, NULL); /* Init framerate timer */
    timerModelSwitch = SDL_AddTimer(MODEL_SWITCH, _modelswitchtimer, NULL); /* Init model switch timer */

    while ( SDL_WaitEvent(&event) && _quit == 0) {
        switch (event.type) {
            case SDL_QUIT: _quit = 1; break;
            case SDL_USEREVENT:
                if (event.user.code == TIM_FRAME_INTERVAL) {
                    drawScene(model);
                } else if (event.user.code == TIM_MODEL_SWITCH) {

                    model = genRandInt(0, (N_MODELS -1)); /* Pick a random model */
                    //preCacheScene(model);
                }
                break;
            case SDL_KEYDOWN: _quit = 1; break;
        }
    }

    SDL_RemoveTimer(timerMain);
    SDL_RemoveTimer(timerModelSwitch);
}



int main(int argc, char **argv)
{
    int i;
    char *wid_env;
    static char sdlwid[100];
    Uint32 wid = 0;
    Display *dpy;
    XWindowAttributes winattr;
    winattr.width = 0;
    winattr.height = 0;

    /* See if we've been given a window argument */
    for (i = 0; i < argc ; i++) {
        if (!strcmp (argv[i], "-window-id")) {
            wid = strtol(argv[i+1], (char **) NULL, 0); /* Base 0 autodetects hex/dec */
        }
    }

    /* If no window argument, check environment */
    if (wid == 0) {
        if ((wid_env = getenv("XSCREENSAVER_WINDOW")) != NULL ) {
            wid = strtol(wid_env, (char **) NULL, 0); /* Base 0 autodetects hex/dec */
        }
    }

    /* Get win attrs if we've been given a window, otherwise we'll use our own */
    if (wid != 0 ) {
        if ((dpy = XOpenDisplay(NULL)) != NULL) { /* Use the default display */
            XGetWindowAttributes(dpy, (Window) wid, &winattr);
            XCloseDisplay(dpy);
            snprintf(sdlwid, 100, "SDL_WINDOWID=0x%X", wid);
            putenv(sdlwid); /* Tell SDL to use this window */
        }
    }
    winattr.width=1824;
    winattr.height=768;
    mainloop(winattr.width, winattr.height);
    _exit_(0);
    return 0; /* Silence Warnings */
}
