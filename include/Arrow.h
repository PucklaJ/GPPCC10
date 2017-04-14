#ifndef ARROW_H
#define ARROW_H
#include <PhysicsSprite.h>

#define ARROW_SPEED 200.0
#define ARROW_DAMAGE 2.0

class GameScreen;

class Arrow : public SDL::PhysicsSprite
{
public:
	Arrow(const SDL::Vector2&,const SDL::Vector2&);

	bool init() override;
	bool update() override;
	void quit() override;

	void BeginContact(b2Contact*,b2Fixture*,b2Fixture*);

private:
	SDL::Vector2 m_direction;
	GameScreen* m_gameScreen = nullptr;
};


#endif
