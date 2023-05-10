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
#include <sstream>

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

		m_timerActive = true;
		m_startTime = std::chrono::steady_clock::now();
		m_elapsedTime = 0;
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



		//TODO No build in way to change font color. Not enough time currently to do this. 
		//myFont->Print(screen, "timer", 800, 800);


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

	//Called from Level when the player collides with a finish
	void Game::OnFinishReached()
	{
		m_timerActive = false;
	}

	void Game::UpdateLevelTimer()
	{
		if (m_timerActive) {
			auto currentTime = std::chrono::steady_clock::now();
			m_elapsedTime = static_cast<int>(std::chrono::duration_cast<std::chrono::seconds>(currentTime - m_startTime).count());
		}

		std::stringstream ss;
		ss << "TIMER: " << m_elapsedTime;
		std::string timerText = ss.str();
		screen->Print(const_cast<char*>(timerText.c_str()), ScreenWidth / 2, 100, 0xFFFFFF);
	}
};