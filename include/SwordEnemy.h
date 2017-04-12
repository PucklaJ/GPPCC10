#ifndef SWORD_ENEMY_H
#define SWORD_ENEMY_H
#include <PhysicsSprite.h>

class SwordEnemy : public SDL::PhysicsSprite
{
public:
	SwordEnemy(const SDL::Vector2& pos);

	bool init() override;
	bool update() override;

	void quit() override;
};

#endif
