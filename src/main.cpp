#include "../include/Game.h"



#ifdef _WIN32
int WinMain(int args,char* argv[])
#else
int main(int args,char* argv[])
#endif
{
	Game().run();
	return 0;
}
