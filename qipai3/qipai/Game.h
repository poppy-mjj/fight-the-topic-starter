#pragma once
#define _USE_MATH_DEFINES 
#define CARDS_AMOUNT   100// (CARDID_END - CARDID_BEGIN + 1)
#define HOVER_SHFIT 4.0

#define CARD_INFO_MOVESET sf::Vector2f(-40.0, -340.0)

#define LINE_ARROW_GROWING_SPEED 50 // ֡�����ƣ�ԽСԽ��
#define LINE_ARROW_LASTTIME 10 // ֡�����ƣ�ԽСԽ��
#define COVER_RECT_FADDED_SPEED 80 // ֡�����ƣ�ԽСԽ�� 
#define COVER_RECT_TRANSPARENCY 180 // �ܻ�����Ѫ�������ɰ���ɫ�ĳ�ʼ͸����

#define HEROS_POS sf::Vector2f(630.0, 100.0)
#define HERO_STEP_X 230
#define HERO_STEP_Y 290
#define HEROS_AMOUNT 7  // �Ƕ�̬Ӣ������
#define HEROS_ROW_AMOUNT 3 // Ӣ��һ�е�����������

#define GOBLIN_COVER_RECT sf::Vector2f(210.0, 150.0)



#include "player.h"
#include<windows.h>
#include<iostream>
#include<sstream>
#include <stack>
// ����������ѧ��������
#include <cmath>
#include "Animation.hpp"
#include "AnimatedSprite.hpp"


typedef enum GAME_STATE
{
	stGET1,   //���һ����
	stGET2,   //��Ҷ�����
	stGET3,   //���������
	stPLAYER1,//���һ����
	stPLAYER2,//��Ҷ�����
	stPLAYER3,//���������
	stSTART,
	stGUIDE,
	stRESPONSE1,
	stRESPONSE2,
	stRESPONSE3,
	stCHOOCE,

};

typedef enum clicking_state {
	no_need,                      //���ܵ��
	click_sending,
	click_need_obj,               //���Ƶ������Ҫ�������
	clicked_need_card,            //���Ƶ������Ҫ�ٵ�������γ���ϼ�
	click_func,
};

typedef enum response_state {
	no_card,                      //û����Ҫ��Ӧ�Ŀ���
	need_response_card,            //��Ҫ��Ӧ����
	response_success,               //�ɹ���Ӧ����
	response_fail,                  //��Ӧ����ʧ��

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

	sf::Texture tReStart, tPoFang, tTrashCard, tStartGround, tGuide, tStart, tStartButton, tBackground, tChooseBackground, tPlayer1, tPlayer2, tPlayer3, tRoundOver, tRoundBackGround;//�����������
	sf::Sprite  sReStart, sPoFang, sTrashCard, sStartGround, sGuide, sStart, sStartButton, sBackground, sChooseBackground, sPlayer1, sPlayer2, sPlayer3, sRoundOver;//�����������
	sf::Texture t_tCard[CARDS_AMOUNT]; // ������������
	sf::Texture t_tCardBigger[CARDS_AMOUNT]; // ������������
	sf::Texture t_tHero[HEROS_AMOUNT]; // ����Ӣ������
	sf::Sprite s_sHero[HEROS_AMOUNT]; // ����Ӣ�۾������
	sf::FloatRect r_rHero[HEROS_AMOUNT]; // ����Ӣ�۾���ĵ������
	bool is_selected_pwh = false;
	//int pwh_player_id = -1;
	sf::Texture tGuidePage[11];
	sf::Sprite  sGudiePage[11];

	sf::Texture tUnSelectedHero; // ����δѡ���Ӣ������
	sf::Texture tPWH_BG;

	sf::Clock clock;
	sf::RectangleShape pwh_Hero_selected_cover; // ���ĺ�Ӣ�۵�ѡ���ɰ�
	sf::Texture pwh_texture;
	Animation pwh_animation; // ���ĺ�����
	AnimatedSprite pwh_Hero; // �����Ӣ��-->���ĺ�
	AnimatedSprite pwh_Hero_copy1, pwh_Hero_copy2;
	sf::FloatRect r_pwh_Hero; // �����Ӣ��-->���ĺ�,��Ӧ����
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


	sf::TcpSocket socket;  //�ͻ��˵�socket
	sf::Packet Game_packet;  //Game�������еı����Ĵ��

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
	void bind_player_textures(); // ���������


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
	void card_func(int cardID);//roundΪ��ϼ�״̬���
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


