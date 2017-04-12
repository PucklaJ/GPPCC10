#include "../include/Player.h"
#include <Physics.h>
#include "../include/Game.h"
#include "../include/GameScreen.h"
#include <iostream>
#include <InputManager.h>
#include <Physics.h>
#include <Camera.h>
#include <TiledMap.h>

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
	bdef.linearDamping = DAMPING;

	b2FixtureDef fdef;
	fdef.density = DEF_DENSITY;
	fdef.friction = DEF_FRICTION;
	fdef.restitution = DEF_RESTITUTION;

	b2Body* body = m_mainClass->getPhysics()->createCIRCLE(getPosition(),10.0,bdef,fdef);
	setBody(body);
	setOffset(SDL::Vector2(0.0,17.0));

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

	if(isPressed(BUT_UP))
	{
		pressed = true;
		force.x = 0.0;
		force.y = SPEED;
	}
	if(isPressed(BUT_LEFT))
	{
		pressed = true;
		force.x = -SPEED;
		force.y = 0.0;
	}
	if(isPressed(BUT_RIGHT))
	{
		pressed = true;
		force.x = SPEED;
		force.y = 0.0;
	}
	if(isPressed(BUT_DOWN))
	{
		pressed = true;
		force.x = 0.0;
		force.y = -SPEED;
	}

	if(pressed)
	{
		getBody()->ApplyForceToCenter(force,true);
		m_nothingPressedTime = 0.0;
	}
	else
	{
		m_nothingPressedTime += m_game->getDeltaTimeInSeconds();
		if(m_nothingPressedTime > NOTHING_PRESSED_STAND_TIME)
			getBody()->SetLinearVelocity(b2Vec2(0.0,0.0));
	}
	// Slow down

	if(getBody()->GetLinearVelocity().LengthSquared() > SLOW_DOWN_POINT*SLOW_DOWN_POINT)
	{
		b2Vec2 vel = getBody()->GetLinearVelocity();
		vel = -vel;
		vel.x /= vel.Length();
		vel.y /= vel.Length();
		vel *= SLOW_DOWN_VEL;

		getBody()->ApplyForceToCenter(vel,true);
	}

}

bool Player::update()
{
	handleInput();
	controlMovement();

	for(int i = 0;i<6;i++)
	m_pressedPrevious[i] = m_pressed[i];

	setCamera();

	return true;
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

void Player::quit()
{
	m_listener->removeListener(this);
}
