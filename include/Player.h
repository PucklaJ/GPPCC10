#ifndef PLAYER_H
#define PLAYER_H
#include <PhysicsSprite.h>
#include <JoystickListener.h>
#include "Animations.h"

#define BUT_UP 0
#define BUT_LEFT 1
#define BUT_DOWN 2
#define BUT_RIGHT 3
#define BUT_ATTACK 4
#define BUT_SPECIAL 5
#define SPEED (isState(PLAYER_ATTACK_STATE) ? 10.0 : 40.0)
#define SLOW_DOWN_POINT 1.0
#define SLOW_DOWN_VEL SPEED/3.0
#define STICK_DEADZONE 10000
#define NOTHING_PRESSED_STAND_TIME 0.5
#define DAMPING_WITH_MOVE 0.0f
#define DAMPING_WITHOUT_MOVE 10.0f
#define ZERO_VEL_DIVIDE 1.5f
#define PLAYER_HEALTH 50.0
#define PLAYER_NORM_DAMAGE 1.0
#define PLAYER_ATTACK_TIME 0.3
#define PLAYER_RIGHT 0
#define PLAYER_LEFT 1
#define PLAYER_DOWN 2
#define PLAYER_UP 4
#define PLAYER_KNOCKBACK 150.0
#define PLAYER_NO_DAMAGE_TIME 0.3
#define PLAYER_STAND_VEL 1.0

#define PLAYER_DEAD_STATE 1 << 0
#define PLAYER_ATTACK_STATE 1 << 1
#define PLAYER_KNOCKBACK_STATE 1 << 2



class GameScreen;
class Game;

namespace SDL
{
	class Sprite;
}

class Player : public SDL::PhysicsSprite, public SDL::AxisButtonListener
{
public:
	Player(const SDL::Vector2&);

	bool init() override;
	bool update() override;

	bool isState(long);
	void setState(long,long del = ~0);
	double getHealth() {return m_health;}
	void damage(double,const b2Vec2&);

	void quit() override;

	void onAxisMotion(const SDL_ControllerAxisEvent&) override;
	void onButtonDown(const SDL_ControllerButtonEvent&) override;
	void onButtonUp(const SDL_ControllerButtonEvent&) override;
private:
	void handleInput();
	void controlMovement();
	void controlAttacks();
	void updateAnimations();
	bool isPressed(int);
	bool justPressed(int);
	void setCamera();
	void attack(int);
	void setAnimation(int);

	int m_curAnim = -1;
	int m_curAnimTween = -1;

	Game* m_game = nullptr;
	GameScreen* m_gameScreen = nullptr;
	SDL::Sprite* m_hitBox = nullptr;
	SDL::Vector2 m_hitPos;

	bool m_controller[6] = {false,false,false,false,false,false};
	bool m_pressed[6] = {false,false,false,false,false,false};
	bool m_pressedPrevious[6];
	int m_xAxis = 0;
	int m_yAxis = 0;
	long m_state = 0;
	double m_health = PLAYER_HEALTH;
	int m_lookDir = PLAYER_DOWN;
	double m_attackTime = 0.0;
	double m_damageTime = 0.0;


};


#endif
