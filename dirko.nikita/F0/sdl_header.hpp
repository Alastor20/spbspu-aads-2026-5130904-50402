#ifndef SDL_HEADER_HPP
#define SDL_HEADER_HPP

#define USE_FAKE_SDL

#ifdef USE_FAKE_SDL
#include <cstdint>
using Uint16 = uint16_t;
using Uint32 = uint32_t;
using Uint8 = uint8_t;
#define SDL_INIT_AUDIO 0
int SDL_Init(int init)
{
  return init;
}
int SDL_GetError();
#else
#include <SDL2/SDL.h>

#endif

#endif
