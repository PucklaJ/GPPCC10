#ifndef ARROW_H
#define ARROW_H
#include <PhysicsSprite.h>

class Arrow : public SDL::PhysicsSprite
{
public:
	Arrow(const SDL::Vector2&,const SDL::Vector2&);

	bool init() override;
	bool update() override;
	void quit() override;
};


#endif
