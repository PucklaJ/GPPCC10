#include "../include/BowEnemy.h"
#include "../include/GameScreen.h"
#include "../include/Player.h"
#include <MainClass.h>
#include <Physics.h>
#include <iostream>
#include <Sprite.h>
#include <Colors.h>
#include "../include/Arrow.h"

BowEnemy::BowEnemy(const SDL::Vector2& pos) : SDL::PhysicsSprite("Assets/bow_enemy.png")
{
	setPosition(pos);
}

bool BowEnemy::init()
{
	SDL::PhysicsSprite::init();
	m_gameScreen = static_cast<GameScreen*>(getParent());

	b2BodyDef bdef;
	bdef.fixedRotation = true;
	bdef.type = b2_dynamicBody;
	bdef.linearDamping = BOW_ENEMY_DAMPING;

	b2FixtureDef fdef;
	fdef.density = DEF_DENSITY;
	fdef.friction = DEF_FRICTION;
	fdef.restitution = DEF_RESTITUTION;

	b2Body* body = m_mainClass->getPhysics()->createCIRCLE(getPosition(),13.0,bdef,fdef);
	setBody(body);
	setOffset(SDL::Vector2(0.0,-17.0));

	return true;
}

bool BowEnemy::update()
{
	controlMovement();

	if(isState(BOW_ENEMY_DIE_STATE))
	{
		setState(BOW_ENEMY_DEAD_STATE);
	}

	if(isState(BOW_ENEMY_DEAD_STATE))
	{
		for(size_t i = 0;i<m_gameScreen->getBowEnemies().size();i++)
		{
			if(m_gameScreen->getBowEnemies()[i] == this)
			{
				m_gameScreen->getBowEnemies()[i] = m_gameScreen->getBowEnemies().back();
				m_gameScreen->getBowEnemies().pop_back();
				break;
			}
		}

		getParent()->removeChild(this);
	}

	if(m_damageTime > 0.0)
	{
		m_damageTime -= m_mainClass->getDeltaTimeInSeconds();
	}
	else
	{
		if(isState(BOW_ENEMY_KNOCKBACK_STATE))
		{
			setState(BOW_ENEMY_STAND_STATE);
		}

	}

	if(m_attackTime > 0.0)
	{
		m_attackTime -= m_mainClass->getDeltaTimeInSeconds();
	}


	setPriority(getPosition().getY());

	return true;
}

bool BowEnemy::isState(long state)
{
	return m_state & state;
}
void BowEnemy::setState(long state,long del)
{
	m_state &= ~del;
	m_state |= state;
}

void BowEnemy::controlMovement()
{
	if(isState(BOW_ENEMY_DIE_STATE | BOW_ENEMY_DEAD_STATE))
	{
		return;
	}

	if(!isState(BOW_ENEMY_KNOCKBACK_STATE) && m_attackTime <= 0.0)
	{
		double dist = SDL::Vector2::distance(m_gameScreen->getPlayer()->getPosition(),getPosition());
		if(dist <= BOW_ENEMY_MAX_ATTACK_DISTANCE && dist >= BOW_ENEMY_MIN_ATTACK_DISTANCE)
		{
			setState(BOW_ENEMY_ATTACK_STATE);
		}
		else if(dist < ((isState(BOW_ENEMY_WALK_ATTACK_STATE) || isState(BOW_ENEMY_ATTACK_STATE)) ? BOW_ENEMY_FOLLOW_DISTANCE : BOW_ENEMY_RECOGNIZE_DISTANCE))
		{
			setState(BOW_ENEMY_WALK_ATTACK_STATE);
		}
		else if(!isState(BOW_ENEMY_STAND_STATE))
		{
			setState(BOW_ENEMY_STAND_STATE);
		}

		if(isState(BOW_ENEMY_STAND_STATE))
		{

		}
		else if(isState(BOW_ENEMY_WALK_ATTACK_STATE))
		{
			b2Vec2 vel = m_gameScreen->getPlayer()->getBody()->GetPosition();
			vel -= getBody()->GetPosition();
			vel.Normalize();
			vel.x *= BOW_ENEMY_SPEED * (dist > BOW_ENEMY_MAX_ATTACK_DISTANCE ? 1.0 : (dist < BOW_ENEMY_MIN_ATTACK_DISTANCE ? -1.0 : 0.0));
			vel.y *= BOW_ENEMY_SPEED * (dist > BOW_ENEMY_MAX_ATTACK_DISTANCE ? 1.0 : (dist < BOW_ENEMY_MIN_ATTACK_DISTANCE ? -1.0 : 0.0));

			getBody()->SetLinearVelocity(vel);
		}
		else if(isState(BOW_ENEMY_ATTACK_STATE))
		{
			getBody()->SetLinearVelocity(b2Vec2(0.0,0.0));
			b2Vec2 vel = m_gameScreen->getPlayer()->getBody()->GetPosition();
			vel -= getBody()->GetPosition();
			vel.Normalize();

			attack(m_mainClass->getPhysics()->vectorWorldToPixel(vel));
		}
	}

}

void BowEnemy::attack(const SDL::Vector2& dir)
{
	if(m_attackTime > 0.0)
		return;

	m_attackTime = BOW_ENEMY_ATTACK_TIME;

	Arrow* a = new Arrow(SDL::Vector2(16.0,32.0) + getPosition(),dir);
	m_gameScreen->addChild(a);

}

void BowEnemy::damage(double health,const b2Vec2& pos)
{
	if(m_damageTime > 0.0)
		return;

	m_damageTime = BOW_ENEMY_NO_DAMAGE_TIME;
	b2Vec2 impulse = getBody()->GetPosition();
	impulse -= pos;
	impulse.Normalize();
	impulse.x *= BOW_ENEMY_KNOCKBACK;
	impulse.y *= BOW_ENEMY_KNOCKBACK;

	setState(BOW_ENEMY_KNOCKBACK_STATE);

	getBody()->ApplyLinearImpulse(impulse,pos,true);
	m_health -= health;
	if(m_health <= 0.0)
		setState(BOW_ENEMY_DIE_STATE);
}

void BowEnemy::quit()
{

}
