#ifndef PLAYER_H
#define PLAYER_H
#include <PhysicsSprite.h>
#include <JoystickListener.h>

#define BUT_UP 0
#define BUT_LEFT 1
#define BUT_DOWN 2
#define BUT_RIGHT 3
#define BUT_ATTACK 4
#define BUT_SPECIAL 5
#define SPEED 60.0
#define SLOW_DOWN_POINT 1.0
#define SLOW_DOWN_VEL SPEED/3.0
#define STICK_DEADZONE 10000
#define NOTHING_PRESSED_STAND_TIME 0.5
#define DAMPING_WITH_MOVE 0.0f
#define DAMPING_WITHOUT_MOVE 10.0f
#define ZERO_VEL_DIVIDE 1.5f

class GameScreen;
class Game;

class Player : public SDL::PhysicsSprite, public SDL::AxisButtonListener
{
public:
	Player(const SDL::Vector2&);

	bool init() override;
	bool update() override;

	void handleInput();
	void controlMovement();
	bool isPressed(int);
	bool justPressed(int);

	void quit() override;
	void setCamera();

	void onAxisMotion(const SDL_ControllerAxisEvent&) override;
	void onButtonDown(const SDL_ControllerButtonEvent&) override;
	void onButtonUp(const SDL_ControllerButtonEvent&) override;
private:
	Game* m_game = nullptr;
	GameScreen* m_gameScreen = nullptr;

	bool m_controller[6] = {false,false,false,false,false,false};
	bool m_pressed[6] = {false,false,false,false,false,false};
	bool m_pressedPrevious[6];
	int m_xAxis = 0;
	int m_yAxis = 0;


};


#endif
