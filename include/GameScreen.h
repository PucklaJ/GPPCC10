#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H
#include <Actor.h>

namespace SDL
{
	class TiledMap;
	class JoystickListener;
}

class Player;
class Game;


class GameScreen : public SDL::Actor
{
public:
	GameScreen();

	bool init() override;
	bool update() override;
	void quit();

	SDL::TiledMap* getMap() {return m_map;}

private:
	void loadMap();

	SDL::TiledMap* m_map = nullptr;
	Player* m_player = nullptr;
	SDL::JoystickListener* m_joystickList = nullptr;
	Game* m_game = nullptr;
};

#endif
