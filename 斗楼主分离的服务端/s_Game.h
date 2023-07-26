#pragma once
#include<SFML/Audio.hpp>
#include<windows.h>
#include<iostream>
#include<sstream>
#include "s_player.h"

#define PLAYER_ROLE_AMOUNT 3
#define CARDS_AMOUNT 100//(CARDID_END - CARDID_BEGIN + 1)
#define HOVER_SHFIT 8.0

#define MAX_TRY 100

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

};

typedef enum clicking_state {
	no_need,                      //不能点击
	unclick_need_card,            //卡牌可以被点击
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
	bool gameOver, gameQuit;
	int Window_width, Window_height, stageWidth, stageHeight, mMineNum, mFlagCalc;

	sf::Font font;
	char before_receive[1024];
	char before_send[1024];
	std::size_t received;

	sf::Vector2i mCornPoint;
	player player1, player2, player3;
	//player player2;
	std::queue<pai> drawPiles;
	std::queue<pai> trashPiles;


	int imgBGNo, imgSkinNo;

	sf::Texture tBackground;//创建背景纹理对象
	sf::Sprite  sBackground, sPlayer1, sPlayer2, sPlayer3;//创建纹理对象
	sf::Texture t_Players[PLAYER_ROLE_AMOUNT]; //创建玩家纹理
	sf::Texture t_tCards[CARDS_AMOUNT]; // 创建卡牌纹理
	GAME_STATE gameState;

	sf::SoundBuffer sbAttack, sbEquip;
	sf::Sound soundAttack, soundEquip;
	sf::Music bkMusic;
	sf::Clock mouseClickTimer;
	sf::Clock gameClock;

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

	clicking_state clickingState;
	response_state responseState;

	sf::TcpListener listener;  //服务端的监听器
	sf::TcpSocket socket1, socket2, socket3;  //服务端的socket
	sf::Packet Game_packet;  //Game类中所有的变量的打包

	bool is_first_send = true;
	//bool is_first_receive = true;

	bool is_send1 = false;
	bool is_send2 = false;
	bool is_send3 = false;

	bool sk1R = false;
	bool sk2R = false;
	bool sk3R = false;

	bool sk1S = false;
	bool sk2S = false;
	bool sk3S = false;

	bool sk1H = false; // 玩家英雄初始化响应
	bool sk2H = false;
	bool sk3H = false;

	bool sk1Re = false;
	bool sk2Re = false;
	bool sk3Re = false;

	bool is_game_begin = false;
	bool is_gameRestart = false;
	bool is_first_round = true;

	bool is_socket1_local_restart = false;
	bool is_socket2_local_restart = false;
	bool is_socket3_local_restart = false;

	int hp1 = 0;
	int hp2 = 0;
	int hp3 = 0;

	int playerTrueId[3];
	int player1_hero_id, player2_hero_id, player3_hero_id;

	void packData();

	void unpackData();

	void Run();

	void Initial();
	void IniData();

	void Receive_packet();
	void Send_packet();

};


