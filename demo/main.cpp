#include "../src/Platforms.h"

#include "Game1.hpp"


#ifndef OSK_ANDROID
#include "../src/GameEntry.h"
int main() {
	OSK::GameEntry<GameMin>(); // MODIFICAR
	return 0;
}
#else
#include "../src/GameEntryAndroid.h"
OSK::UniquePtr<OSK::IGame> game = OSK::MakeUnique<GameMin>(); // ¿extern?
#define OSK_ANDROID_MAIN
#include "../src/GameEntry.h"
#endif
