#pragma once
#define _USE_MATH_DEFINES 
#define CARDS_AMOUNT   100// (CARDID_END - CARDID_BEGIN + 1)
#define HOVER_SHFIT 4.0

#define CARD_INFO_MOVESET sf::Vector2f(-40.0, -340.0)

#define LINE_ARROW_GROWING_SPEED 50 // 帧率限制，越小越快
#define LINE_ARROW_LASTTIME 10 // 帧率限制，越小越快
#define COVER_RECT_FADDED_SPEED 80 // 帧率限制，越小越快 
#define COVER_RECT_TRANSPARENCY 180 // 受击、加血动画的蒙版颜色的初始透明度

#define HEROS_POS sf::Vector2f(630.0, 100.0)
#define HERO_STEP_X 230
#define HERO_STEP_Y 290
#define HEROS_AMOUNT 7  // 非动态英雄总数
#define HEROS_ROW_AMOUNT 3 // 英雄一行的期望的数量

#define GOBLIN_COVER_RECT sf::Vector2f(210.0, 150.0)



#include "player.h"
#include<windows.h>
#include<iostream>
#include<sstream>
#include <stack>
// 用于启用数学常量定义
#include <cmath>
#include "Animation.hpp"
#include "AnimatedSprite.hpp"


typedef enum GAME_STATE
{
	stGET1,   //玩家一抽牌
	stGET2,   //玩家二抽牌
	stGET3,   //玩家三抽牌
	stPLAYER1,//玩家一打牌
	stPLAYER2,//玩家二打牌
	stPLAYER3,//玩家三打牌
	stSTART,
	stGUIDE,
	stRESPONSE1,
	stRESPONSE2,
	stRESPONSE3,
	stCHOOCE,

};

typedef enum clicking_state {
	no_need,                      //不能点击
	click_sending,
	click_need_obj,               //卡牌点击后需要点击对象
	clicked_need_card,            //卡牌点击后需要再点击卡牌形成组合技
	click_func,
};

typedef enum response_state {
	no_card,                      //没有需要回应的卡牌
	need_response_card,            //需要回应卡牌
	response_success,               //成功回应卡牌
	response_fail,                  //回应卡牌失败

};



class Game
{
public:
	sf::RenderWindow window;
	Game();
	~Game();

	int hero_id;

	char buffer[1024];
	char is_start[1024];
	char before_receive[1024];
	char before_send[1024];

	bool is_send = false;
	bool is_game_start = false;
	bool is_selected = false;
	std::size_t received = 0;
	bool gameOver, gameQuit, gameRestart, is_first_round = true, is_local_restart = false;
	int Window_width, Window_height, stageWidth, stageHeight, mMineNum, mFlagCalc;

	sf::Font font;

	sf::Vector2i mCornPoint;
	std::vector<player>players;//player1, player2, player3;
	//player player2;
	std::queue<pai> drawPiles;
	std::stack<pai> trashPiles;
	std::stack<pai> trashPiles_backUp;


	int imgBGNo, imgSkinNo;

	int player1_hero_id, player2_hero_id, player3_hero_id;

	sf::Texture tReStart, tPoFang, tTrashCard, tStartGround, tGuide, tStart, tStartButton, tBackground, tChooseBackground, tPlayer1, tPlayer2, tPlayer3, tRoundOver, tRoundBackGround;//创建纹理对象
	sf::Sprite  sReStart, sPoFang, sTrashCard, sStartGround, sGuide, sStart, sStartButton, sBackground, sChooseBackground, sPlayer1, sPlayer2, sPlayer3, sRoundOver;//创建纹理对象
	sf::Texture t_tCard[CARDS_AMOUNT]; // 创建卡牌纹理
	sf::Texture t_tCardBigger[CARDS_AMOUNT]; // 创建大卡牌纹理
	sf::Texture t_tHero[HEROS_AMOUNT]; // 创建英雄纹理
	sf::Sprite s_sHero[HEROS_AMOUNT]; // 创建英雄精灵对象
	sf::FloatRect r_rHero[HEROS_AMOUNT]; // 创建英雄精灵的点击对象
	bool is_selected_pwh = false;
	//int pwh_player_id = -1;
	sf::Texture tGuidePage[11];
	sf::Sprite  sGudiePage[11];

	sf::Texture tUnSelectedHero; // 创建未选择的英雄纹理
	sf::Texture tPWH_BG;

	sf::Clock clock;
	sf::RectangleShape pwh_Hero_selected_cover; // 彭文海英雄的选中蒙版
	sf::Texture pwh_texture;
	Animation pwh_animation; // 彭文海动画
	AnimatedSprite pwh_Hero; // 特殊的英雄-->彭文海
	AnimatedSprite pwh_Hero_copy1, pwh_Hero_copy2;
	sf::FloatRect r_pwh_Hero; // 特殊的英雄-->彭文海,响应区域
	bool is_hover_pwh = false;
	bool pwh_is_player1 = false;
	bool pwh_is_player2 = false;
	bool pwh_is_player3 = false;

	GAME_STATE gameState;
	GAME_STATE lastState;

	bool is_played_sound = false;
	int playIndex = -1;

	sf::SoundBuffer sbAttack, sbEquip;
	sf::Sound soundAttack, soundEquip;
	sf::SoundBuffer sbMenuButtom;
	sf::Sound soundMenuButtom;
	sf::SoundBuffer sbRoundButtom;
	sf::Sound soundRoundButtom;
	sf::SoundBuffer sbHpDown;
	sf::Sound soundHpDown;
	sf::SoundBuffer sbHpUp;
	sf::Sound soundHpUp;
	sf::SoundBuffer sbMainMusic;
	sf::Sound soundMainMusic;
	sf::SoundBuffer sbCaiXukun, sbDingZhen, sbWujing, sbSunXiaochuan, sbSpiderMan, sbXiaogui, sbXiangjiaojun, sbPWH;
	sf::Sound soundCaiXukun, soundDingZhen, soundWujing, soundSunXiaochuan, soundSpiderMan, soundXiaogui, soundXiangjiaojun, soundPWH;
	sf::Music menu_bgMusic, choose_bgMusic, game_bgMusic;



	sf::Text texting;

	std::vector<int> playing_card;
	std::vector<int> playing_card_objs;
	sf::Packet playing_packet;

	std::vector<int> received_card;
	std::vector<int> received_card_objs;
	sf::Packet receive_packet;

	std::vector<int> sending_card;
	std::vector<int> sending_card_objs;
	sf::Packet sending_packet;

	int response_card;
	int page_guide = 0;

	int true_playerID = -1;
	//clicking_state clickingState;
	std::stack<clicking_state> clickingState_stack;
	response_state responseState;


	sf::FloatRect roundOverRect;
	sf::FloatRect StartRect;
	sf::FloatRect GuideRect;
	sf::FloatRect ReStartRect;


	sf::TcpSocket socket;  //客户端的socket
	sf::Packet Game_packet;  //Game类中所有的变量的打包

	bool is_first_receive = true;
	bool skR = false, skS = false, skI = false, skRe = false;


	std::map<int, GAME_STATE>Num2GameState;
	std::map< GAME_STATE, int>GameState2Num;
	std::map<PLAYER_KIND, GAME_STATE>Player2GameState_Response;
	std::map<PLAYER_KIND, int>Player2Num;
	std::map<int, int>CardTypeCal;


	bool is_DrawLineArrow_inPos = false;

	sf::Vector2f start_point;
	sf::Vector2f temp_point;
	sf::Vector2f end_point;
	sf::Vector2f growing_step;
	int growing_cnt = 0;
	int lastTime_line_arrow = 0;
	bool is_valued = false;

	bool Hp_decreased = false;
	bool Hp_increased = false;
	bool Local_Hp_decreased = false;
	bool Local_Hp_increased = false;
	bool is_fadded_done = true;
	sf::RectangleShape cover_rect;
	int cover_rect_fadded_cnt = 0;
	float fadded_speed;
	float start_transparency;
	float end_transparency;
	float temp_transparency;
	sf::Vector3i fill_color;


	void Run();

	void Initial();
	void LoadMediaData();
	void IniData();
	void bind_player_textures(); // 绑定玩家纹理


	void Input();
	void player_click(sf::Event event, player& a_player);
	void mouseHover(sf::Vector2i mPoint, player&);
	void mouseOut(sf::Vector2i mPoint, player&);
	void LButtonDown(sf::Vector2i mPoint, player&);
	void LButtonResponse(sf::Vector2i mPoint, player&);
	void RButtonDown(player&);


	void Logic();
	void card_full(player& a_player);
	int card_demage(int cardID);
	bool part_function(int cardID);
	bool is_part_function(int cardID1, int cardID2);
	void card_func(int cardID);//round为组合技状态点击
	void card_received_func(int cardID);
	void card_response_func(int cardID);
	bool judge_card(int card1, int card2);
	void receive_clear();
	void beforeReStart();


	void Draw();
	void DrawBG();
	void DrawHeros();
	void DrawPWH();
	void DrawHp();
	void DrawHpChange();
	void DrawWin();
	void DrawCard();
	void DrawButton();
	void DrawCardInfo();
	void DrawPlayingCard_fromTrash();
	void draw_piles_getFromTrash();
	void DrawGameEnd();
	void DrawLineArrow();
	void static_DrawLineArrow();
	void growing_DrawLineArrow();
	void playHeroSound();
	void stopHeroSound(int);
	void startChangeSound();

	void receiving();
	void sending();
	void unpackData();
	void packData();


};


