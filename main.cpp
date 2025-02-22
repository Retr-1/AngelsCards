
#define OLC_PGE_APPLICATION
#include "olcPixelGameEngine.h"
#include <string>
#include <random>
#include <time.h>
using namespace std;

class Card {
public:
	int suit;
	int rank;

	static void initialize_assets() {
		for (int i = 0; i < 13; i++) {
			string filename = "./assets/poker_cards_2_" + to_string(i) + ".png";
			Card::heart_sprites[i] = new olc::Sprite(filename);
			Card::heart_decals[i] = new olc::Decal(Card::heart_sprites[i]);

			filename = "./assets/poker_cards_0_" + to_string(i) + ".png";
			Card::club_sprites[i] = new olc::Sprite(filename);
			Card::club_decals[i] = new olc::Decal(Card::club_sprites[i]);

			filename = "./assets/poker_cards_1_" + to_string(i) + ".png";
			Card::diamond_sprites[i] = new olc::Sprite(filename);
			Card::diamond_decals[i] = new olc::Decal(Card::diamond_sprites[i]);
		}

		red_back_sprite = new olc::Sprite("./assets/card_back_red.png");
		red_back_decal = new olc::Decal(red_back_sprite);
		black_back_sprite = new olc::Sprite("./assets/card_back_black.png");
		black_back_decal = new olc::Decal(black_back_sprite);
	}

	static olc::Sprite* heart_sprites[13];
	static olc::Decal* heart_decals[13];
	static olc::Sprite* club_sprites[13];
	static olc::Decal* club_decals[13];
	static olc::Sprite* diamond_sprites[13];
	static olc::Decal* diamond_decals[13];

	static olc::Sprite* red_back_sprite;
	static olc::Decal* red_back_decal;
	static olc::Sprite* black_back_sprite;
	static olc::Decal* black_back_decal;
};

olc::Sprite* Card::heart_sprites[13] = {};
olc::Decal* Card::heart_decals[13] = {};
olc::Sprite* Card::club_sprites[13] = {};
olc::Decal* Card::club_decals[13] = {};
olc::Sprite* Card::diamond_sprites[13] = {};
olc::Decal* Card::diamond_decals[13] = {};

olc::Sprite* Card::red_back_sprite = nullptr;
olc::Decal* Card::red_back_decal = nullptr;
olc::Sprite* Card::black_back_sprite = 0;
olc::Decal* Card::black_back_decal = 0;


// Override base class with your custom functionality
class Window : public olc::PixelGameEngine
{
	enum Conclusion {
		TIE,
		PLAYER_A,
		PLAYER_B
	};

	enum RoundWinner {
		NONE,
		PLAYER,
		BOT,
	};

	float select_yoffsets[13] = { 0 };
	int player_picks[13];
	int bot_picks[13];
	int round = 0;
	int used_bot = 0;
	int used_player = 0;
	RoundWinner winners[13];
	int table[13];

	olc::Sprite* tie_s;
	olc::Decal* tie_d;
	olc::Sprite* diamond_win_s;
	olc::Decal* diamond_win_d;
	olc::Sprite* heart_win_s;
	olc::Decal* heart_win_d;

	void load_asset(olc::Sprite*& s, olc::Decal*& d, const string& filepath) {
		s = new olc::Sprite(filepath);
		d = new olc::Decal(s);
	}


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
		Card::initialize_assets();
		load_asset(tie_s, tie_d, "./assets/tie.png");
		load_asset(diamond_win_s, diamond_win_d, "./assets/diamond_win.png");
		load_asset(heart_win_s, heart_win_d, "./assets/heart_win.png");

		for (int i = 0; i < 13; i++) {
			table[i] = i;
		}

		shuffle(table, table + 13, default_random_engine(time(0)));

		return true;
	}

	int get_bot_pick() {
		int arr[13] = { 9,2,3,4,5,6,7,8,1,10,11,12,0 };
		return arr[table[round]];
	}

	Conclusion evaluate(int a, int b) {
		if (b == 0) {
			if (a == 0) {
				return TIE;
			}
			else if (a < 10) {
				return PLAYER_A;
			}
			else {
				return PLAYER_B;
			}
		}
		else {
			if (a == 0) {
				if (b < 10) {
					return PLAYER_B;
				}
				else {
					return PLAYER_A;
				}
			}
			else {
				if (a == b) {
					return TIE;
				}
				else {
					return a < b ? PLAYER_B : PLAYER_A;
				}
			}
		}
	}

	RoundWinner evaluate_round() {
		Conclusion first_duel = evaluate(player_picks[round], bot_picks[round]);
		switch (first_duel) {
			case TIE: {
				return NONE;
				break;
			}
			case PLAYER_A: {
				Conclusion second_duel = evaluate(player_picks[round], table[round]);
				if (second_duel == PLAYER_A)
					return PLAYER;
				else
					return NONE;
				break;
			}
			case PLAYER_B: {
				Conclusion second_duel = evaluate(bot_picks[round], table[round]);
				if (second_duel == PLAYER_A)
					return BOT;
				else
					return NONE;
				break;
			}
		}
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

		if (GetMouse(0).bPressed && hover && !(used_player & 1<<i)) {
			player_picks[round] = i;
			used_player |= 1 << i;

			int bot_pick = get_bot_pick();
			bot_picks[round] = bot_pick;
			used_bot |= 1 << bot_pick;

			winners[round] = evaluate_round();

			round++;
		}

		for (int i = 0; i < 13; i++) {
			
			// Player cards
			if (!(used_player & (1<<i)))
				DrawPartialDecal(olc::vf2d((card_w + gap) * i, ScreenHeight() - card_h - select_yoffsets[i]), olc::vf2d(card_w, card_h), Card::diamond_decals[i], { 0,0 }, (olc::vf2d)Card::diamond_sprites[i]->Size());
			

			// Table cards
			DrawPartialDecal(olc::vf2d((card_w + gap) * i, ScreenHeight()/2 - card_h/2), olc::vf2d(card_w, card_h), Card::club_decals[table[i]], { 0,0 }, (olc::vf2d)Card::club_sprites[table[i]]->Size());

			// Bot cards
			if (!(used_bot & (1 << i)))
				DrawPartialDecal(olc::vf2d((card_w + gap) * i, 0), olc::vf2d(card_w, card_h), Card::red_back_decal, { 0,0 }, (olc::vf2d)Card::red_back_sprite->Size());
		}

		
		float heart_w = card_w - 10;
		float heart_h = heart_w * heart_win_s->height / heart_win_s->width;
		float diamond_w = heart_w;
		float diamond_h = diamond_w * diamond_win_s->height / diamond_win_s->width;
		float tie_w = card_w / 4;
		float tie_h =  tie_w * tie_s->height / tie_s->width;

		for (int i = 0; i < round; i++) {
			DrawPartialDecal(olc::vf2d((card_w + gap) * i, ScreenHeight() / 2 + card_h / 2), olc::vf2d(card_w, card_h), Card::diamond_decals[player_picks[i]], { 0,0 }, (olc::vf2d)Card::diamond_sprites[player_picks[i]]->Size());
			DrawPartialDecal(olc::vf2d((card_w + gap) * i, ScreenHeight() / 2 - card_h*1.5f), olc::vf2d(card_w, card_h), Card::heart_decals[bot_picks[i]], { 0,0 }, (olc::vf2d)Card::heart_sprites[bot_picks[i]]->Size());

			//cout << "WINNER " << winners[i] << '\n';
			switch (winners[i]) {
			case NONE:
				DrawPartialDecal(olc::vf2d((card_w + gap) * i + card_w / 2 - tie_w / 2, ScreenHeight() / 2 - tie_h / 2), olc::vf2d(tie_w, tie_h), tie_d, { 0,0 }, tie_s->Size());
				break;
			case PLAYER:
				DrawPartialDecal(olc::vf2d((card_w + gap) * i + card_w / 2 - diamond_w / 2, ScreenHeight() / 2 - diamond_h / 2), olc::vf2d(diamond_w, diamond_h), diamond_win_d, { 0,0 }, diamond_win_s->Size());
				break;
			case BOT:
				DrawPartialDecal(olc::vf2d((card_w + gap) * i + card_w / 2 - heart_w / 2, ScreenHeight() / 2 - heart_h / 2), olc::vf2d(heart_w, heart_h), heart_win_d, { 0,0 }, heart_win_s->Size());
				break;
			}
		}

		if (round >= 13) {
			const string endings[3] = { "Tie!", "YOU WIN!", "YOU LOSE!" };

			int sum = 0;
			for (int i = 0; i < 13; i++) {
				if (winners[i] == PLAYER) {
					sum += table[i] == 0 ? 13 : table[i];
				}
				else if (winners[i] == BOT) {
					sum -= table[i] == 0 ? 13 : table[i];
				}
			}

			RoundWinner winner = NONE;
			if (sum > 0) {
				winner = PLAYER;
			}
			else if (sum < 0) {
				winner = BOT;
			}

			DrawStringDecal(olc::vf2d(40, ScreenHeight() - 80), endings[winner], olc::WHITE, olc::vf2d(5,5));
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