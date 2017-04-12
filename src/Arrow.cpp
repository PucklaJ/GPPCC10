#include "../include/Arrow.h"

Arrow::Arrow(const SDL::Vector2& pos,const SDL::Vector2& dir)
{
	setPosition(pos);
}

bool Arrow::init()
{
	return true;
}

bool Arrow::update()
{
	return true;
}

void Arrow::quit()
{

}
