
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
using namespace std;

class Card {
public:
	int suit;
	int number;

	static olc::Sprite heart_sprites[14];
	static olc::Sprite spade_sprites[14];
};

// Override base class with your custom functionality
class Window : public olc::PixelGameEngine
{
public:
	Window()
	{
		// Name your application
		sAppName = "Window";
	}

public:
	bool OnUserCreate() override
	{
		// Called once at the start, so create things here
		for (int i = 0; i < 14; i++) {
			string filename = "poker_cards_0_" + to_string(i) + ".png";
			Card::heart_sprites[i] = olc::Sprite(filename);

			filename = "poker_cards_1_" + to_string(i) + ".png";
			Card::spade_sprites[i] = olc::Sprite(filename);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		return true;
	}
};

int main()
{
	Window win;
	if (win.Construct(800, 800, 1, 1))
		win.Start();
	return 0;
}