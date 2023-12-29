#include "ConsoleManager.h"

using namespace std;

// Singleton Instance
dbzk_fps::ConsoleManager dbzk_fps::ConsoleManager::csm_Instance; // Seemingly need this declared in HookManager.cpp so a bunch of linker errors don't happen.

namespace dbzk_fps {
	void ConsoleManager::Init() {
		cout.flush();
		freopen_s((FILE**)stdout, "CONOUT$", "w", stdout); // Allows us to add outputs to the ASI Loader Console Window.
		cout.clear();
		cin.clear();
		AllocConsole();
	}
}