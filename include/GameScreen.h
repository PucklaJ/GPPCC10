#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H
#include <Actor.h>


class GameScreen : public SDL::Actor
{
public:
	GameScreen();

	bool init() override;
	bool update() override;
	void quit();
};

#endif
