#include "solarsystem_textures.h"
#include "SDL_image.h"

/* Load a Texture and Return the Surface */
SDL_Surface *loadTexture(char *filename)
{
    SDL_Surface *imgsurface;
    char texfile[FILENAME_MAX];

    snprintf(texfile, FILENAME_MAX, "%s/%s", "textures", filename); /* Try local texture dir 1st */
    if ((imgsurface = IMG_Load(texfile)) == NULL) {
        snprintf(texfile, FILENAME_MAX, "%s/%s", TEXTUREDIR, filename); /* Try configured texture dir */
        if ((imgsurface = IMG_Load(texfile)) == NULL) {
            fprintf(stderr, "Unable to load %s: %s\n", texfile, SDL_GetError());
            _exit_(6);
        }
    }

    return(imgsurface);
}

/* Setup the main textures */
void initTextures(void)
{
    int i;
    char *tex_fnames[] = { "sun.png", "mercury.png", "venus.png", "earth.png", "mars.png", "jupiter.png",
                           "saturn.png", "uranus.png", "neptune.png", "pluto.png",
                           "corona.png", "rings.png", "star.png"};
    SDL_Surface *img[N_TEXTURES];

    glGenTextures(N_TEXTURES, textures);

    for (i = 0; i < N_TEXTURES; i++) {
        img[i] = loadTexture(tex_fnames[i]);
        glBindTexture(GL_TEXTURE_2D, textures[i]);   // 2d texture (x and y size)
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // scale linearly when image bigger than texture
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // scale linearly when image smalled than texture
        glTexImage2D(GL_TEXTURE_2D, 0, 4, img[i]->w, img[i]->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, img[i]->pixels);
    }
}
