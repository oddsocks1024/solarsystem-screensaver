#include "solarsystem.h"

enum TIMERS
{
    TIM_FRAME_INTERVAL,
    TIM_MODEL_SWITCH
};

/* Protos */
Uint32 _frametimer(Uint32 interval, void *param);
Uint32 _modelswitchtimer(Uint32 interval, void *param);
