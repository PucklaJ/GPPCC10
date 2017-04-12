#include "../include/Game.h"
#include "../include/GameScreen.h"

Game::Game() : SDL::MainClass("GPPCC10")
{

}

Game::~Game()
{

}

bool Game::init()
{
	m_gameScreen = new GameScreen();
	addChild(m_gameScreen);

	return true;
}

bool Game::update()
{
	return true;
}

void Game::quit()
{

}
