#ifndef GAME_H
#define GAME_H
#include <MainClass.h>

class Game : public SDL::MainClass
{
public:
	Game();
	~Game();

	bool init() override;
	bool update() override;

	void quit() override;
};


#endif
