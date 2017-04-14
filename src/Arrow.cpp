#include "../include/Arrow.h"
#include <MainClass.h>
#include <Physics.h>
#include <iostream>
#include "../include/GameScreen.h"
#include "../include/Player.h"

Arrow::Arrow(const SDL::Vector2& pos,const SDL::Vector2& dir) : SDL::PhysicsSprite("Assets/arrow.png"),
	m_direction(dir)
{
	setPosition(pos);
}

bool Arrow::init()
{
	SDL::PhysicsSprite::init();

	m_gameScreen = static_cast<GameScreen*>(getParent());

	b2BodyDef bdef;
	b2FixtureDef fdef;

	bdef.type = b2_dynamicBody;
	bdef.angle = m_direction.angle();

	fdef.friction = 0.0;
	fdef.restitution = 0.0;
	fdef.density = 0.5;
	fdef.isSensor = true;
	char* c = "ARROW";
	fdef.userData = c;

	b2Body* body = m_mainClass->getPhysics()->createBOX(getPosition(),40,10,bdef,fdef);
	setBody(body);

	body->SetLinearVelocity(m_mainClass->getPhysics()->vectorPixelToWorld(m_direction * ARROW_SPEED));
	setFlip(SDL_FLIP_HORIZONTAL);

	return true;
}

bool Arrow::update()
{
	setPriority(getPosition().getY());

	return true;
}

void Arrow::BeginContact(b2Contact* c,b2Fixture* f1,b2Fixture* f2)
{
	if(f2->GetUserData() && strcmp((char*)f2->GetUserData(),"PLAYER") == 0)
	{
		m_gameScreen->getPlayer()->damage(ARROW_DAMAGE,c->GetFixtureA()->GetBody()->GetWorldPoint(c->GetManifold()->localPoint));
		m_gameScreen->removeChild(this);
	}
	else if(f2->GetUserData() && strcmp((char*)f2->GetUserData(),"WORLD") == 0)
	{
		m_gameScreen->removeChild(this);
	}
}

void Arrow::quit()
{

}
