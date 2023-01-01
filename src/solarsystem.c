/*
Copyright (C) 2009-2012 Ian Chapman

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

GLfloat pan = PANSTART;
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

/* Cache as much as possible */
void preCacheScene(int model)
{
    /* MODEL CACHE SECTION */
    /* See if we already have a display list for this model, create if not */
    if (glIsList(modelDL[model]) != GL_TRUE) {
        modelDL[model] = glGenLists(1);
        glNewList(modelDL[model], GL_COMPILE);
        glEnable(GL_LIGHTING);

        switch (model) {
            case M_MERCURY:
                planets[model].tilt = 0.0f;
                planets[model].parentsize = 35.0f;
                planets[model].corona = 5.0f;
                glBindTexture(GL_TEXTURE_2D, textures[T_MERCURY]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                break;
            case M_VENUS:
                planets[model].tilt = 177.36f;
                planets[model].parentsize = 8.0f;
                planets[model].corona = 4.0f;
                glBindTexture(GL_TEXTURE_2D, textures[T_VENUS]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                break;
            case M_EARTH:
                planets[model].tilt = 23.45f;
                planets[model].parentsize = 3.0f;
                planets[model].corona = 3.5f;
                glBindTexture(GL_TEXTURE_2D, textures[T_EARTH]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                break;
            case M_MARS:
                planets[model].tilt = 25.19f;
                planets[model].parentsize = 2.0f;
                planets[model].corona = 3.0f;
                glBindTexture(GL_TEXTURE_2D, textures[T_MARS]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                break;
            case M_JUPITER:
                planets[model].tilt = 3.13f;
                planets[model].parentsize = 1.0f;
                planets[model].corona = 3.0f;
                glBindTexture(GL_TEXTURE_2D, textures[T_JUPITER]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                break;
            case M_SATURN:
                planets[model].tilt = 15.0f;
                planets[model].parentsize = 0.5f;
                planets[model].corona = 3.0f;
                glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);
                glBindTexture(GL_TEXTURE_2D, textures[T_SATURN]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
                glBindTexture(GL_TEXTURE_2D, textures[T_RINGS]);
                gluDisk(diskQuadric, 2.2f, 3.2f, 128, 128);
                break;
            case M_URANUS:
                planets[model].tilt = 97.86f;
                planets[model].parentsize = 0.5f;
                planets[model].corona = 3.0f;
                glBindTexture(GL_TEXTURE_2D, textures[T_URANUS]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                break;
            case M_NEPTUNE:
                planets[model].tilt = 28.31f;
                planets[model].parentsize = 0.25f;
                planets[model].corona = 2.8f;
                glBindTexture(GL_TEXTURE_2D, textures[T_NEPTUNE]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                break;
            case M_PLUTO:
                planets[model].tilt = 122.52f;
                planets[model].parentsize = 0.25f;
                planets[model].corona = 3.0f;
                glBindTexture(GL_TEXTURE_2D, textures[T_PLUTO]);
                gluSphere(sphereQuadric, 2.0f, PSLICES, PSTACKS);
                break;
        }
        glDisable(GL_LIGHTING);
        glEndList();
    }
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
    gluPerspective(45.0f, (GLfloat) w/ (GLfloat) h, 0.1f, 100.0f); /* Calc Aspect Ratio */
    glMatrixMode(GL_MODELVIEW);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LIGHT0);
    glEnable(GL_TEXTURE_2D);
    glDisable(GL_LIGHTING);
    glEnable(GL_POINT_SMOOTH);
    GLfloat Ambient[4]={0.0f, 0.0f, 0.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_AMBIENT, Ambient);
}

/* Draw a Frame */
void drawScene(int model)
{
    static GLfloat prot,srot,sunrot,crot,sunorb = 0.0f; /* Planet, star, sun, corona rotation, sun orbit */
    static GLfloat LightPos[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    GLfloat sunsize = 0.0f;
    int i;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glBlendFunc(GL_SRC_COLOR, GL_ONE_MINUS_SRC_ALPHA);

    /* STARFIELD SECTION */
    glLoadIdentity();
    glDisable(GL_TEXTURE_2D);
    glRotatef(srot,0.0f,0.0f,-1.0f); srot += 0.05f; /* Set Rotation */

    for (i = 0; i < STARNUM ; i++) {
        if (stars[i].x > STARPSIZE) {
            stars[i].x = genRandFloat(-STARPSIZE, -STARPSIZE+2);
        } else {
            stars[i].x += 0.05f;
        }

        glPointSize(stars[i].size);
        glBegin(GL_POINTS);
        glColor3ub(stars[i].r, stars[i].g, stars[i].b);
        glVertex3f(stars[i].x, stars[i].y, stars[i].z);
        glEnd();
    }
    glEnable(GL_TEXTURE_2D);

    /* SUN SECTION */
    /* Move Sun around planet - simulate planet orbiting sun */
    glLoadIdentity();
    glTranslatef (0.0f, 0.0f, -2.0f); /* Move to centre of planet */
    glRotatef(sunorb, 0.0f, 1.0f, 0.0f); sunorb += 0.25f; /* Set orbit rotation */
    if (sunorb >= 360.0f) { sunorb = 0.0f; }
    glTranslatef (50.0f, 0.0f, 0.0f); /* Move out to orbit */
    glLightfv(GL_LIGHT0, GL_POSITION, LightPos); /* Set light position */
    glColor3ub(255,255,255);
    glBindTexture(GL_TEXTURE_2D, textures[T_SUN]);
    glRotatef(90.0f,1.0f,0.0f,0.0f);
    glRotatef(sunrot, 0.0f,0.0f,1.0f); sunrot += 1.0f; /* Set sun rotation */
    gluSphere(sphereQuadric, planets[model].parentsize, PSLICES, PSTACKS);

    /* PLANET SECTION */
    glLoadIdentity();
    glTranslatef(0.0f,pan,-7.0f);
    glRotatef(90.0f,1.0f,0.0f,0.0f); /* Set Planet Upright */
    glRotatef(planets[model].tilt,1.0f,0.0f,0.0f); /* Tilt */
    glRotatef(prot,0.0f,0.0f,-1.0f); prot += 1.0f; /* spin the planet */
    glCallList(modelDL[model]);

    /* CORONA SECTION - Draw last due to transparency */
    if (sunorb < 175.0f) {
        glLoadIdentity();
        glEnable(GL_LIGHTING);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glTranslatef(0.0f,pan,-7.0f);
        glRotatef(crot, 0.0f, 0.0f, 1.0f); crot+= 0.1f; /* Corona Rotation */
        glBindTexture(GL_TEXTURE_2D, textures[T_CORONA]);
        glNormal3f(0.0f, 0.0f, -1.0f);
        glBegin(GL_QUADS);
        glTexCoord2f(0.0f,0.0f); glVertex3f(-planets[model].corona, -planets[model].corona, -2.0f);
        glTexCoord2f(1.0f,0.0f); glVertex3f(planets[model].corona, -planets[model].corona, -2.0f);
        glTexCoord2f(1.0f,1.0f); glVertex3f(planets[model].corona, planets[model].corona, -2.0f);
        glTexCoord2f(0.0f,1.0f); glVertex3f(-planets[model].corona, planets[model].corona, -2.0f);
        glEnd();
        glDisable(GL_LIGHTING);
    }

    if ( pan < PANSTOP ) {
        pan += 0.1f;
    }

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
    preCacheScene(model);
    timerMain = SDL_AddTimer(FRAME_INTERVAL, _frametimer, NULL); /* Init framerate timer */
    timerModelSwitch = SDL_AddTimer(MODEL_SWITCH, _modelswitchtimer, NULL); /* Init model switch timer */
    SDL_ShowCursor(SDL_DISABLE);

    while ( SDL_WaitEvent(&event) && _quit == 0) {
        switch (event.type) {
            case SDL_QUIT: _quit = 1; break;
            case SDL_USEREVENT:
                if (event.user.code == TIM_FRAME_INTERVAL) {
                    drawScene(model);
                } else if (event.user.code == TIM_MODEL_SWITCH) {
                    pan = PANSTART;
                    model = genRandInt(0, (N_MODELS -1)); /* Pick a random model */
                    preCacheScene(model);
                }
                break;
            case SDL_KEYDOWN: _quit = 1; break;
        }
    }

    SDL_ShowCursor(SDL_ENABLE);
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

    mainloop(winattr.width, winattr.height);
    _exit_(0);
    return 0; /* Silence Warnings */
}
