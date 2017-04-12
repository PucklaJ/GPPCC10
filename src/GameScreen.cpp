#include "../include/GameScreen.h"
#include <TiledMap.h>
#include "../include/Player.h"
#include <JoystickListener.h>
#include "../include/Game.h"
#include <JoystickManager.h>
#include <Camera.h>
#include <iostream>

GameScreen::GameScreen()
{

}

bool GameScreen::init()
{
	m_game = static_cast<Game*>(m_mainClass);

	m_game->activatePhysics(SDL::Vector2(),SDL::Vector2(3200.0,3200.0));
	m_game->activateJoystick();

	loadMap();

	m_joystickList = new SDL::JoystickListener();
	m_joystickList->addListener(m_player,0);
	m_game->getJoystickManager()->setListener(m_joystickList);

	return true;
}

void GameScreen::loadMap()
{
	m_map = new SDL::TiledMap("Assets/level01.tmx");
	addChild(m_map);
	setSize(m_map->GetWidth()*m_map->GetTileWidth(),m_map->GetHeight()*m_map->GetTileHeight());

	for(size_t i = 0;i<m_map->GetObjectGroups().size();i++)
	{
		const Tmx::ObjectGroup* objG = m_map->GetObjectGroup(i);

		if(objG->GetName() == "spawns")
		{
			for(int i = 0;i<objG->GetNumObjects();i++)
			{
				const Tmx::Object* obj = objG->GetObject(i);
				if(obj->GetName() == "player")
				{
					m_player = new Player(SDL::Vector2(obj->GetX(),obj->GetY()));
					addChild(m_player);
				}
			}
		}
	}

}

bool GameScreen::update()
{
	return true;
}

void GameScreen::quit()
{

}
