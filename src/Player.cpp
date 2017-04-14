#include "../include/Player.h"
#include <Physics.h>
#include "../include/Game.h"
#include "../include/GameScreen.h"
#include <iostream>
#include <InputManager.h>
#include <Physics.h>
#include <Camera.h>
#include <TiledMap.h>
#include <Colors.h>
#include "../include/SwordEnemy.h"
#include "../include/BowEnemy.h"

Player::Player(const SDL::Vector2& pos) : SDL::PhysicsSprite("Assets/player.png")
{
	setPosition(pos);
	setAutomaticDestroy(false);
}

bool Player::init()
{
	SDL::PhysicsSprite::init();

	m_game = static_cast<Game*>(m_mainClass);
	m_gameScreen = static_cast<GameScreen*>(getParent());

	b2BodyDef bdef;
	bdef.fixedRotation = true;
	bdef.type = b2_dynamicBody;
	bdef.linearDamping = DAMPING_WITH_MOVE;

	b2FixtureDef fdef;
	fdef.density = DEF_DENSITY;
	fdef.friction = DEF_FRICTION;
	fdef.restitution = DEF_RESTITUTION;
	char* c = "PLAYER";
	fdef.userData = c;

	b2Body* body = m_mainClass->getPhysics()->createCIRCLE(getPosition(),13.0,bdef,fdef);
	setBody(body);
	setOffset(SDL::Vector2(0.0,-17.0));

	m_hitBox = new SDL::Sprite(SDL::Textures::BOX(m_game,SDL::Vector2(32.0,32.0),SDL::Colors::RED));
	m_hitBox->setVisible(false);
	addChild(m_hitBox);

	std::cout << "asin(0): " << asin(0)/MATH_PI*180.0 << std::endl;
	std::cout << "asin(1): " << asin(1)/MATH_PI*180.0 << std::endl;
	std::cout << "asin(-1): " << asin(-1)/MATH_PI*180.0 << std::endl;

	return true;
}

bool Player::isPressed(int i)
{
	return m_pressed[i];
}
bool Player::justPressed(int i)
{
	return m_pressed[i] && !m_pressedPrevious[i];
}

void Player::handleInput()
{
	SDL::InputManager* in = m_mainClass->getInputManager();

	m_pressed[BUT_UP] = m_controller[BUT_UP] || in->isPressed(SDLK_w) || (abs(m_yAxis) > STICK_DEADZONE && m_yAxis < 0);
	m_pressed[BUT_LEFT] = m_controller[BUT_LEFT] || in->isPressed(SDLK_a) || (abs(m_xAxis) > STICK_DEADZONE && m_xAxis < 0);
	m_pressed[BUT_DOWN] = m_controller[BUT_DOWN] || in->isPressed(SDLK_s) || (abs(m_yAxis) > STICK_DEADZONE && m_yAxis > 0);
	m_pressed[BUT_RIGHT] = m_controller[BUT_RIGHT] || in->isPressed(SDLK_d) || (abs(m_xAxis) > STICK_DEADZONE && m_xAxis > 0);
	m_pressed[BUT_ATTACK] = m_controller[BUT_ATTACK] || in->isPressed(SDLK_j);
	m_pressed[BUT_SPECIAL] = m_controller[BUT_SPECIAL] || in->isPressed(SDLK_k);
}

void Player::setCamera()
{
	SDL::Vector2 pos = m_game->getPhysics()->coordsWorldToPixel(getBody()->GetPosition());
	m_game->getCamera()->setPosition(pos);

	if(m_game->getCamera()->getPosition().getX() - m_game->getNativeRes().getX()/2.0/m_game->getCamera()->getScale() < 0.0)
	{
		m_game->getCamera()->setPosition(m_game->getNativeRes().getX()/2.0/m_game->getCamera()->getScale(),m_game->getCamera()->getPosition().getY());
	}
	else if(m_game->getCamera()->getPosition().getX() + m_game->getNativeRes().getX()/2.0/m_game->getCamera()->getScale() > m_gameScreen->getMap()->GetWidth()*m_gameScreen->getMap()->GetTileWidth())
	{
		m_game->getCamera()->setPosition(m_gameScreen->getMap()->GetWidth()*m_gameScreen->getMap()->GetTileWidth()-m_game->getNativeRes().getX()/2.0/m_game->getCamera()->getScale(),m_game->getCamera()->getPosition().getY());
	}
	if(m_game->getCamera()->getPosition().getY() - m_game->getNativeRes().getY()/2.0/m_game->getCamera()->getScale() < 0.0)
	{
		m_game->getCamera()->setPosition(m_game->getCamera()->getPosition().getX(),m_game->getNativeRes().getY()/2.0/m_game->getCamera()->getScale());
	}
	else if(m_game->getCamera()->getPosition().getY() + m_game->getNativeRes().getY()/2.0/m_game->getCamera()->getScale() > m_gameScreen->getMap()->GetHeight()*m_gameScreen->getMap()->GetTileHeight())
	{
		m_game->getCamera()->setPosition(m_game->getCamera()->getPosition().getX(),m_gameScreen->getMap()->GetHeight()*m_gameScreen->getMap()->GetTileHeight()-m_game->getNativeRes().getY()/2.0/m_game->getCamera()->getScale());
	}
}

void Player::controlMovement()
{
	b2Vec2 force(0,0);
	bool pressed = false;

	if(!isState(PLAYER_KNOCKBACK_STATE))
	{
		if(isPressed(BUT_UP))
		{
			pressed = true;
			force.x = 0.0;
			force.y = SPEED*10.0;
			m_lookDir = PLAYER_UP;
		}
		if(isPressed(BUT_LEFT))
		{
			pressed = true;
			force.x = -SPEED*10.0;
			force.y = 0.0;
			m_lookDir = PLAYER_LEFT;
		}
		if(isPressed(BUT_RIGHT))
		{
			pressed = true;
			force.x = SPEED*10.0;
			force.y = 0.0;
			m_lookDir = PLAYER_RIGHT;
		}
		if(isPressed(BUT_DOWN))
		{
			pressed = true;
			force.x = 0.0;
			force.y = -SPEED*10.0;
			m_lookDir = PLAYER_DOWN;
		}
	}


	if(pressed)
	{
		getBody()->SetLinearDamping(0.0f);
		getBody()->ApplyForceToCenter(force,true);
		double x,y;
		x = getBody()->GetLinearVelocity().x;
		y = getBody()->GetLinearVelocity().y;

		if(force.x==0.0)
			getBody()->SetLinearVelocity(b2Vec2(x/ZERO_VEL_DIVIDE,y));
		else if(force.y==0.0)
			getBody()->SetLinearVelocity(b2Vec2(x,y/ZERO_VEL_DIVIDE));
		if(abs(x) > SPEED)
			getBody()->SetLinearVelocity(b2Vec2(x>0?SPEED:-SPEED,y));
		if(abs(y) > SPEED)
			getBody()->SetLinearVelocity(b2Vec2(x,y>0?SPEED:-SPEED));
	}
	else
	{
		if(!isState(PLAYER_KNOCKBACK_STATE))
		getBody()->SetLinearDamping(10.0f);
	}

}

bool Player::update()
{
	handleInput();
	controlMovement();
	controlAttacks();

	if(isState(PLAYER_ATTACK_STATE))
	{
		m_hitBox->setVisible(true);
		m_hitBox->setPosition(m_hitPos + getPosition());
		m_attackTime += m_game->getDeltaTimeInSeconds();
		if(m_attackTime >= PLAYER_ATTACK_TIME)
		{
			m_attackTime = 0.0;
			setState(0);
		}
		for(size_t i = 0;i<m_gameScreen->getSwordEnemies().size();i++)
		{
			SwordEnemy* e = m_gameScreen->getSwordEnemies()[i];
			if(e->intersects(m_hitPos + getPosition(),SDL::Vector2(32.0,32.0)))
			{
				e->damage(PLAYER_NORM_DAMAGE,m_mainClass->getPhysics()->coordsPixelToWorld(m_hitPos+getPosition()+SDL::Vector2(16.0,16.0)));
			}
		}
		for(size_t i = 0;i<m_gameScreen->getBowEnemies().size();i++)
		{
			BowEnemy* e = m_gameScreen->getBowEnemies()[i];
			if(e->intersects(m_hitPos + getPosition(),SDL::Vector2(32.0,32.0)))
			{
				e->damage(PLAYER_NORM_DAMAGE,m_mainClass->getPhysics()->coordsPixelToWorld(m_hitPos+getPosition()+SDL::Vector2(16.0,16.0)));
			}
		}
	}
	else
	{
		m_hitBox->setVisible(false);
	}

	if(m_damageTime > 0.0)
	{
		m_damageTime -= m_mainClass->getDeltaTimeInSeconds();
	}
	else
	{
		if(isState(PLAYER_KNOCKBACK_STATE))
		{
			setState(0);
		}
	}

	for(int i = 0;i<6;i++)
	m_pressedPrevious[i] = m_pressed[i];

	setCamera();

	setPriority(getPosition().getY());

	return true;
}

void Player::damage(double health,const b2Vec2& pos)
{
	if(m_damageTime > 0.0)
		return;

	m_damageTime = PLAYER_NO_DAMAGE_TIME;
	b2Vec2 impulse = getBody()->GetPosition();
	impulse -= pos;
	impulse.Normalize();
	impulse.x *= PLAYER_KNOCKBACK;
	impulse.y *= PLAYER_KNOCKBACK;

	setState(PLAYER_KNOCKBACK_STATE);

	getBody()->ApplyLinearImpulse(impulse,pos,true);
	m_health -= health;
	if(m_health <= 0.0)
		setState(PLAYER_DEAD_STATE);
	std::cout << "Healh: " << m_health << std::endl;
}

void Player::onAxisMotion(const SDL_ControllerAxisEvent& e)
{
	if(e.axis == SDL::Axis::R2 || e.axis == SDL::Axis::L2)
		return;

	switch(SDL::Uint8ToInt(e.axis))
	{
	case SDL::Axis::LEFT_STICK_X: m_xAxis = e.value;break;
	case SDL::Axis::LEFT_STICK_Y: m_yAxis = e.value;break;
	}
}
void Player::onButtonDown(const SDL_ControllerButtonEvent& e)
{
	switch(SDL::Uint8ToInt(e.button))
	{
	case SDL::Buttons::UP:
		m_controller[BUT_UP] = true;
		break;
	case SDL::Buttons::LEFT:
		m_controller[BUT_LEFT] = true;
		break;
	case SDL::Buttons::RIGHT:
		m_controller[BUT_RIGHT] = true;
		break;
	case SDL::Buttons::DOWN:
		m_controller[BUT_DOWN] = true;
		break;
	case SDL::Buttons::SQUARE:
		m_controller[BUT_ATTACK] = true;
		break;
	case SDL::Buttons::CIRCLE:
		m_controller[BUT_SPECIAL] = true;
		break;
	}
}
void Player::onButtonUp(const SDL_ControllerButtonEvent& e)
{
	switch(SDL::Uint8ToInt(e.button))
	{
	case SDL::Buttons::UP:
		m_controller[BUT_UP] = false;
		break;
	case SDL::Buttons::LEFT:
		m_controller[BUT_LEFT] = false;
		break;
	case SDL::Buttons::RIGHT:
		m_controller[BUT_RIGHT] = false;
		break;
	case SDL::Buttons::DOWN:
		m_controller[BUT_DOWN] = false;
		break;
	case SDL::Buttons::SQUARE:
		m_controller[BUT_ATTACK] = false;
		break;
	case SDL::Buttons::CIRCLE:
		m_controller[BUT_SPECIAL] = false;
		break;
	}
}

void Player::controlAttacks()
{
	if(isState(PLAYER_KNOCKBACK_STATE))
		return;

	if(!isState(PLAYER_ATTACK_STATE) && justPressed(BUT_ATTACK))
	{
		attack(m_lookDir);
	}
}

void Player::attack(int dir)
{
	setState(PLAYER_ATTACK_STATE);
	switch(dir)
	{
	case PLAYER_RIGHT:
		m_hitPos = SDL::Vector2(32.0,16.0);
		break;
	case PLAYER_LEFT:
		m_hitPos = SDL::Vector2(-32.0,16.0);
		break;
	case PLAYER_UP:
		m_hitPos = SDL::Vector2(0.0,-32.0);
		break;
	case PLAYER_DOWN:
		m_hitPos = SDL::Vector2(0.0,64.0);
		break;
	}
}

bool Player::isState(long state)
{
	return m_state & state;
}
void Player::setState(long state,long del)
{
	m_state &= ~del;
	m_state |= state;
}

void Player::quit()
{
	m_listener->removeListener(this);
}
