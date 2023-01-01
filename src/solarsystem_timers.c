#include "solarsystem_timers.h"

/* Timer callback. Create a redraw event */
Uint32 _frametimer(Uint32 interval, void *param)
{
    SDL_Event event;

    /* Don't overload queue with draw events and delaying other events */
    if (SDL_PeepEvents(NULL, 1, SDL_PEEKEVENT, SDL_ALLEVENTS) == 0) {
        event.type = SDL_USEREVENT;
        event.user.code = TIM_FRAME_INTERVAL;
        SDL_PushEvent(&event);
    }

    return interval;
}

/* Timer callback. Create a model switch event */
Uint32 _modelswitchtimer(Uint32 interval, void *param)
{
    SDL_Event event;
    event.type = SDL_USEREVENT;
    event.user.code = TIM_MODEL_SWITCH;
    SDL_PushEvent(&event);
    return interval;
}
