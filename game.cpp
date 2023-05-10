#include "game.h"
#include "surface.h"
#include <cstdio> //printf
#include "template.h"
#include <xpolymorphic_allocator.h>
#include <algorithm>
#include <cmath>
#include <SDL.h> // For key codes
#include "Level.h"
#include "Slime.h"

namespace Tmpl8
{
	// -----------------------------------------------------------
	// Initialize the application
	// -----------------------------------------------------------

	//Game::Game(Level& level, Slime slime)
	//	:
	//	m_level(level),
	//	m_slime(slime)
	//{
	//	// Initialize other member variables if necessary
	//}

	void Game::Init()
	{
		//m_slime = Slime(screen, m_level);
		
		m_slime = new Slime(screen, m_level, std::bind(&Game::OnFinishReached, this));
		m_level.GenerateCollisionLines();
	}
	

	// -----------------------------------------------------------
	// Close down application
	// -----------------------------------------------------------
	void Game::Shutdown()
	{
		delete m_slime;
	}

	static int frame = 0;



	// -----------------------------------------------------------
	// Main application tick function
	// -----------------------------------------------------------
	void Game::Tick(float deltaTime)
	{
		screen->Clear(0); //Clear the screen

		//Level generation
		m_level.DrawLines(screen); //Draw the lines

		//Slime - Player 
	    m_slime->Draw(deltaTime); //Draw Player
		//m_slime->CheckCollision(); //Check for collision //This is being called in SlimeDraw now
	}

	void Game::KeyDown(int key)
	{
		m_slime->KeyDown(key);

	}

	void Game::KeyUp(int key)
	{
		m_slime->KeyUp(key);

	}

	void Game::Resize(int screenWidth, int screenHeight)
	{

	}

	void Game::OnFinishReached()
	{
		
	}
};