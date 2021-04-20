// yac8emul.h : Include file for standard system include files,
// or project specific include files.

#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <memory>
#include <stack>
#include <vector>

// For windows we need to define this because reasons
#if defined(_WIN32)
#define SDL_MAIN_HANDLED
#endif
#include <SDL.h>

// TODO: Reference additional headers your program requires here.
