#include "../include/SwordEnemy.h"
#include "../include/GameScreen.h"
#include "../include/Player.h"
#include <MainClass.h>
#include <Physics.h>
#include <iostream>
#include <Sprite.h>
#include <Colors.h>

SwordEnemy::SwordEnemy(const SDL::Vector2& pos) : SDL::PhysicsSprite("Assets/sword_enemy.png")
{
	setPosition(pos);
}

bool SwordEnemy::init()
{
	SDL::PhysicsSprite::init();
	m_gameScreen = static_cast<GameScreen*>(getParent());

	b2BodyDef bdef;
	bdef.fixedRotation = true;
	bdef.type = b2_dynamicBody;
	bdef.linearDamping = SWORD_ENEMY_DAMPING;

	b2FixtureDef fdef;
	fdef.density = DEF_DENSITY;
	fdef.friction = DEF_FRICTION;
	fdef.restitution = DEF_RESTITUTION;

	b2Body* body = m_mainClass->getPhysics()->createCIRCLE(getPosition(),13.0,bdef,fdef);
	setBody(body);
	setOffset(SDL::Vector2(0.0,-17.0));

	m_hitBox = new SDL::Sprite(SDL::Textures::BOX(m_mainClass,SDL::Vector2(32.0,32.0),SDL::Colors::BLUE));
	m_hitBox->setVisible(false);
	addChild(m_hitBox);

	return true;
}

bool SwordEnemy::update()
{
	controlMovement();

	if(isState(SWORD_ENEMY_DIE_STATE))
	{
		setState(SWORD_ENEMY_DEAD_STATE);
	}

	if(isState(SWORD_ENEMY_DEAD_STATE))
	{
		for(size_t i = 0;i<m_gameScreen->getSwordEnemies().size();i++)
		{
			if(m_gameScreen->getSwordEnemies()[i] == this)
			{
				m_gameScreen->getSwordEnemies()[i] = m_gameScreen->getSwordEnemies().back();
				m_gameScreen->getSwordEnemies().pop_back();
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
		if(isState(SWORD_ENEMY_KNOCKBACK_STATE))
		{
			setState(SWORD_ENEMY_STAND_STATE);
			m_standTime = SWORD_ENEMY_STAND_TIME;
		}

	}

	if(m_attackTime > 0.0)
	{
		m_attackTime -= m_mainClass->getDeltaTimeInSeconds();
	}

	if(isState(SWORD_ENEMY_ATTACK_STATE) && m_attackTime > 0.0)
		m_hitBox->setVisible(true);
	else
		m_hitBox->setVisible(false);


	setPriority(getPosition().getY());

	return true;
}

bool SwordEnemy::isState(long state)
{
	return m_state & state;
}
void SwordEnemy::setState(long state,long del)
{
	m_state &= ~del;
	m_state |= state;
}

void SwordEnemy::controlMovement()
{
	if(isState(SWORD_ENEMY_DIE_STATE | SWORD_ENEMY_DEAD_STATE))
	{
		return;
	}

	if(!isState(SWORD_ENEMY_KNOCKBACK_STATE) && m_attackTime <= 0.0)
	{
		double dist = SDL::Vector2::distance(m_gameScreen->getPlayer()->getPosition(),getPosition());
		if(dist < SWORD_ENEMY_ATTACK_DISTANCE)
		{
			setState(SWORD_ENEMY_ATTACK_STATE);
		}
		else if(dist < ((isState(SWORD_ENEMY_WALK_ATTACK_STATE) || isState(SWORD_ENEMY_ATTACK_STATE)) ? SWORD_ENEMY_FOLLOW_DISTANCE : SWORD_ENEMY_RECOGNIZE_DISTANCE))
		{
			setState(SWORD_ENEMY_WALK_ATTACK_STATE);
		}
		else if(!isState(SWORD_ENEMY_STAND_STATE) && !isState(SWORD_ENEMY_WALK_STATE))
		{
			setState(SWORD_ENEMY_STAND_STATE);
			m_standTime = SWORD_ENEMY_STAND_TIME;
		}

		if(isState(SWORD_ENEMY_STAND_STATE))
		{
			m_standTime -= m_mainClass->getDeltaTimeInSeconds();
			if(m_standTime<=0.0)
			{
				setState(SWORD_ENEMY_WALK_STATE);
				double x = SWORD_ENEMY_WALK_X;
				double y = SWORD_ENEMY_WALK_Y;
				m_dstWalkPoint = SDL::Vector2(x,y) + m_mainClass->getPhysics()->coordsWorldToPixel(getBody()->GetPosition());
				m_walkVel = m_mainClass->getPhysics()->coordsPixelToWorld(m_dstWalkPoint);
				m_walkVel -= getBody()->GetPosition();
				m_walkVel.Normalize();
				m_walkVel.x *= SWORD_ENEMY_SPEED * SWORD_ENEMY_WALK_MOVE_MULT;
				m_walkVel.y *= SWORD_ENEMY_SPEED * SWORD_ENEMY_WALK_MOVE_MULT;
			}

		}
		else if(isState(SWORD_ENEMY_WALK_STATE))
		{
			getBody()->SetLinearVelocity(m_walkVel);

			b2Vec2 dst = m_mainClass->getPhysics()->coordsPixelToWorld(m_dstWalkPoint);
			dst -= getBody()->GetPosition();
			if(dst.LengthSquared() <= SWORD_ENEMY_WALK_DIST*SWORD_ENEMY_WALK_DIST)
			{
				getBody()->SetLinearVelocity(b2Vec2(0.0,0.0));
				setState(SWORD_ENEMY_STAND_STATE);
				m_standTime = SWORD_ENEMY_STAND_TIME;
			}
		}
		else if(isState(SWORD_ENEMY_WALK_ATTACK_STATE))
		{
			b2Vec2 vel = m_gameScreen->getPlayer()->getBody()->GetPosition();
			vel -= getBody()->GetPosition();
			vel.Normalize();
			vel.x *= SWORD_ENEMY_SPEED;
			vel.y *= SWORD_ENEMY_SPEED;

			getBody()->SetLinearVelocity(vel);
		}
		else if(isState(SWORD_ENEMY_ATTACK_STATE))
		{
			getBody()->SetLinearVelocity(b2Vec2(0.0,0.0));
			b2Vec2 vel = m_gameScreen->getPlayer()->getBody()->GetPosition();
			vel -= getBody()->GetPosition();
			vel.Normalize();

			if(abs(vel.x) > abs(vel.y))
			{
				if(vel.x > 0.0)
				{
					attack(SWORD_ENEMY_RIGHT);
				}
				else
				{
					attack(SWORD_ENEMY_LEFT);
				}
			}
			else
			{
				if(vel.y > 0.0)
				{
					attack(SWORD_ENEMY_UP);
				}
				else
				{
					attack(SWORD_ENEMY_DOWN);
				}
			}
		}
	}

}

void SwordEnemy::attack(int dir)
{
	if(m_attackTime > 0.0)
		return;

	switch(dir)
	{
	case SWORD_ENEMY_RIGHT:
		m_hitPos = SDL::Vector2(32.0,16.0);
		break;
	case SWORD_ENEMY_LEFT:
		m_hitPos = SDL::Vector2(-32.0,16.0);
		break;
	case SWORD_ENEMY_UP:
		m_hitPos = SDL::Vector2(0.0,-32.0);
		break;
	case SWORD_ENEMY_DOWN:
		m_hitPos = SDL::Vector2(0.0,64.0);
		break;
	}

	m_hitBox->setPosition(m_hitPos + getPosition());
	m_attackTime = SWORD_ENEMY_ATTACK_TIME;

	if(m_gameScreen->getPlayer()->intersects(m_hitBox))
	{
		m_gameScreen->getPlayer()->damage(SWORD_ENEMY_DAMAGE,m_mainClass->getPhysics()->coordsPixelToWorld(SDL::Vector2(16.0,16.0)+m_hitBox->getPosition()));
	}

}

void SwordEnemy::damage(double health,const b2Vec2& pos)
{
	if(m_damageTime > 0.0)
		return;

	m_damageTime = SWORD_ENEMY_NO_DAMAGE_TIME;
	b2Vec2 impulse = getBody()->GetPosition();
	impulse -= pos;
	impulse.Normalize();
	impulse.x *= SWORD_ENEMY_KNOCKBACK;
	impulse.y *= SWORD_ENEMY_KNOCKBACK;

	setState(SWORD_ENEMY_KNOCKBACK_STATE);

	getBody()->ApplyLinearImpulse(impulse,pos,true);
	m_health -= health;
	if(m_health <= 0.0)
		setState(SWORD_ENEMY_DIE_STATE);
}

void SwordEnemy::PreSolve(b2Contact* c,b2Fixture* f1,b2Fixture* f2,const b2Manifold* oldManifold)
{
	if(isState(SWORD_ENEMY_WALK_STATE))
	{
		setState(SWORD_ENEMY_STAND_STATE);
		m_standTime = 0.0;
	}
}

void SwordEnemy::quit()
{

}
