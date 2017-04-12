#ifndef BOW_ENEMY_H
#define BOW_ENEMY_H
#include <PhysicsSprite.h>

class BowEnemy : public SDL::PhysicsSprite
{
public:
	BowEnemy(const SDL::Vector2&);

	bool init() override;
	bool update() override;

	void quit();
};

#endif
