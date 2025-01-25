
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
using namespace std;

class Card {
public:
	int suit;
	int number;

	static olc::Sprite* heart_sprites[13];
	static olc::Decal* heart_decals[13];
	static olc::Sprite* spade_sprites[13];
	static olc::Decal* spade_decals[13];
};

olc::Sprite* Card::heart_sprites[13] = {};
olc::Sprite* Card::spade_sprites[13] = {};
olc::Decal* Card::heart_decals[13] = {};
olc::Decal* Card::spade_decals[13] = {};

// Override base class with your custom functionality
class Window : public olc::PixelGameEngine
{
	float select_yoffsets[13] = { 0 };

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
		for (int i = 0; i < 13; i++) {
			string filename = "./assets/poker_cards_2_" + to_string(i) + ".png";
			Card::heart_sprites[i] = new olc::Sprite(filename);
			Card::heart_decals[i] = new olc::Decal(Card::heart_sprites[i]);

			filename = "./assets/poker_cards_1_" + to_string(i) + ".png";
			Card::spade_sprites[i] = new olc::Sprite(filename);
			Card::spade_decals[i] = new olc::Decal(Card::spade_sprites[i]);
		}

		return true;
	}

	bool OnUserUpdate(float fElapsedTime) override
	{
		// Called once per frame, draws random coloured pixels
		const int gap = 5;
		int w = ScreenWidth()-gap*12;
		float card_ratio = Card::heart_sprites[0]->width / (float)Card::heart_sprites[0]->height;
		float card_w = w / 13.0f;
		float card_h = card_w / card_ratio;
		//float scale = card_w / Card::heart_sprites[0]->width;
		//float scale_h = card_h / Card::heart_sprites[0]->height;

		int i = GetMouseX() / (card_w + gap);
		bool hover = GetMouseY() > ScreenHeight() - card_h - select_yoffsets[i];
		const float mv_speed = 30;
		const float max_offset = 18;
		if (hover) {
			select_yoffsets[i] += mv_speed * fElapsedTime;
			select_yoffsets[i] = min(max_offset, select_yoffsets[i]);
		}
		for (int j=0; j<13; j++) {
			if (i == j && hover) continue;
			select_yoffsets[j] -= mv_speed * fElapsedTime;
			select_yoffsets[j] = max(0.0f, select_yoffsets[j]);
		}

		for (int i = 0; i < 13; i++) {
			/*DrawPartialSprite({ card_w * i, ScreenHeight() - card_h }, Card::heart_sprites[i], { 0,0 }, Card::heart_sprites[i]->Size(), scale);*/
			DrawPartialDecal(olc::vf2d((card_w+gap) * i, ScreenHeight() - card_h - select_yoffsets[i]), olc::vf2d(card_w, card_h), Card::heart_decals[i], {0,0}, (olc::vf2d)Card::heart_sprites[i]->Size());
		}

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