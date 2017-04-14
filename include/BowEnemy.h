#ifndef BOW_ENEMY_H
#define BOW_ENEMY_H
#include <PhysicsSprite.h>

#define BOW_ENEMY_HEALTH 4.0
#define BOW_ENEMY_DAMAGE 1.0
#define BOW_ENEMY_DAMPING 2.0f
#define BOW_ENEMY_SPEED 20.0
#define BOW_ENEMY_NO_DAMAGE_TIME 1.0
#define BOW_ENEMY_KNOCKBACK 500.0
#define BOW_ENEMY_MAX_ATTACK_DISTANCE 300.0
#define BOW_ENEMY_MIN_ATTACK_DISTANCE 200.0
#define BOW_ENEMY_RECOGNIZE_DISTANCE 200.0
#define BOW_ENEMY_FOLLOW_DISTANCE 600.0
#define BOW_ENEMY_ATTACK_TIME 4.0

#define BOW_ENEMY_ATTACK_STATE 1 << 0
#define BOW_ENEMY_DIE_STATE 1 << 1
#define BOW_ENEMY_DEAD_STATE 1 << 2
#define BOW_ENEMY_STAND_STATE 1 << 3
#define BOW_ENEMY_KNOCKBACK_STATE 1 << 4
#define BOW_ENEMY_WALK_ATTACK_STATE 1 << 5

class GameScreen;

namespace SDL
{
	class Sprite;
}

class BowEnemy : public SDL::PhysicsSprite
{
public:
	BowEnemy(const SDL::Vector2& pos);

	bool init() override;
	bool update() override;

	bool isState(long);
	void setState(long,long del = ~0);

	double getHealth() {return m_health;}
	void damage(double,const b2Vec2&);

	void quit() override;
private:
	void controlMovement();
	void attack(const SDL::Vector2&);

	GameScreen* m_gameScreen = nullptr;

	double m_health = BOW_ENEMY_HEALTH;
	long m_state = BOW_ENEMY_STAND_STATE;
	double m_damageTime = 0.0;
	double m_attackTime = 0.0;
};

#endif
