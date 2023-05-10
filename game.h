#pragma once
#include "Level.h"
#include "Slime.h"

namespace Tmpl8 {

class Surface;
class Game
{
public:
	void SetTarget( Surface* surface ) { screen = surface; }
	void Init();
	void Shutdown();
	void Tick( float deltaTime );
	void MouseUp( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseDown( int button ) { /* implement if you want to detect mouse button presses */ }
	void MouseMove( int x, int y ) { /* implement if you want to detect mouse movement */ }
	void KeyUp(int key);
	void Resize(int screenWidth, int screenHeight);
	void KeyDown(int key);
	void OnFinishReached();
private:
	Surface* screen;
	Level& m_level = Level();
	Slime* m_slime;
};

}; // namespace Tmpl8