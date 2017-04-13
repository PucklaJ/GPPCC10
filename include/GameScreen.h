#ifndef GAME_SCREEN_H
#define GAME_SCREEN_H
#include <Actor.h>
#include <vector>

namespace SDL
{
	class TiledMap;
	class JoystickListener;
}

class Player;
class SwordEnemy;
class BowEnemy;
class Game;


class GameScreen : public SDL::Actor
{
public:
	GameScreen();

	bool init() override;
	bool update() override;
	void quit();

	SDL::TiledMap* getMap() {return m_map;}
	Player* getPlayer() {return m_player;}

	std::vector<SwordEnemy*>& getSwordEnemies() {return m_swordEnemies;}
	std::vector<BowEnemy*>& getBowEnemies() {return m_bowEnemies;}

private:
	void loadMap();

	SDL::TiledMap* m_map = nullptr;
	Player* m_player = nullptr;
	SDL::JoystickListener* m_joystickList = nullptr;
	Game* m_game = nullptr;

	std::vector<SwordEnemy*> m_swordEnemies;
	std::vector<BowEnemy*> m_bowEnemies;
};

#endif
