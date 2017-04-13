#ifndef SWORD_ENEMY_H
#define SWORD_ENEMY_H
#include <PhysicsSprite.h>

#define SWORD_ENEMY_HEALTH 4.0
#define SWORD_ENEMY_DAMAGE 1.0
#define SWORD_ENEMY_DAMPING 2.0f
#define SWORD_ENEMY_SPEED 20.0
#define SWORD_ENEMY_MAX_STAND_TIME 2.0
#define SWORD_ENEMY_MIN_STAND_TIME 0.5
#define SWORD_ENEMY_MAX_X_WALK 40.0
#define SWORD_ENEMY_MIN_X_WALK -40.0
#define SWORD_ENEMY_MAX_Y_WALK 40.0
#define SWORD_ENEMY_MIN_Y_WALK -40.0
#define SWORD_ENEMY_WALK_MOVE_MULT 0.3
#define SWORD_ENEMY_WALK_X ((double)SDL::getRand(SWORD_ENEMY_MIN_X_WALK*100.0,SWORD_ENEMY_MAX_X_WALK*100.0)/100.0)
#define SWORD_ENEMY_WALK_Y ((double)SDL::getRand(SWORD_ENEMY_MIN_Y_WALK*100.0,SWORD_ENEMY_MAX_Y_WALK*100.0)/100.0)
#define SWORD_ENEMY_STAND_TIME ((double)SDL::getRand(SWORD_ENEMY_MIN_STAND_TIME*100.0,SWORD_ENEMY_MAX_STAND_TIME*100.0)/100.0)
#define SWORD_ENEMY_WALK_DIST 0.1
#define SWORD_ENEMY_NO_DAMAGE_TIME 1.0
#define SWORD_ENEMY_KNOCKBACK 500.0
#define SWORD_ENEMY_ATTACK_DISTANCE 50.0
#define SWORD_ENEMY_RECOGNIZE_DISTANCE 200.0
#define SWORD_ENEMY_FOLLOW_DISTANCE 600.0
#define SWORD_ENEMY_ATTACK_TIME 1.0

#define SWORD_ENEMY_WALK_STATE 1 << 0
#define SWORD_ENEMY_ATTACK_STATE 1 << 1
#define SWORD_ENEMY_DIE_STATE 1 << 2
#define SWORD_ENEMY_DEAD_STATE 1 << 3
#define SWORD_ENEMY_STAND_STATE 1 << 4
#define SWORD_ENEMY_KNOCKBACK_STATE 1 << 5
#define SWORD_ENEMY_WALK_ATTACK_STATE 1 << 6

#define SWORD_ENEMY_RIGHT 0
#define SWORD_ENEMY_LEFT 1
#define SWORD_ENEMY_UP 2
#define SWORD_ENEMY_DOWN 3

class GameScreen;

namespace SDL
{
	class Sprite;
}

class SwordEnemy : public SDL::PhysicsSprite
{
public:
	SwordEnemy(const SDL::Vector2& pos);

	bool init() override;
	bool update() override;

	bool isState(long);
	void setState(long,long del = ~0);

	double getHealth() {return m_health;}
	void damage(double,const b2Vec2&);

	void PreSolve(b2Contact*,b2Fixture*,b2Fixture*,const b2Manifold*) override;

	void quit() override;
private:
	void controlMovement();
	void attack(int);

	GameScreen* m_gameScreen = nullptr;
	SDL::Sprite* m_hitBox = nullptr;
	SDL::Vector2 m_hitPos;

	double m_health = SWORD_ENEMY_HEALTH;
	long m_state = SWORD_ENEMY_STAND_STATE;
	double m_standTime = SWORD_ENEMY_STAND_TIME;
	double m_damageTime = 0.0;
	double m_attackTime = 0.0;
	SDL::Vector2 m_dstWalkPoint;
	b2Vec2 m_walkVel;
};

#endif
