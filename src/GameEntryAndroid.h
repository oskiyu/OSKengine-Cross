#pragma once

#include "Platforms.h"
#include "UniquePtr.hpp"
#include "Game.h"

#ifdef OSK_ANDROID
extern OSK::UniquePtr<OSK::IGame> game;
#endif
