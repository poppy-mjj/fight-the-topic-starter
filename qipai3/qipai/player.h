#pragma once
#include "pai.h"

#define HP_WIDTH 25

#define LOUZHU_HP 140
#define BAYOU_HP 70
#define LOUZHU_INITIAL_CARD_AMOUNT 6
#define LOUZHU_GET_CARD_AMOUNT 4
#define BAYOU_INITIAL_CARD_AMOUNT 4
#define BAYOU_GET_CARD_AMOUNT 2
#define CARD_ROTATE_SPEED 20  //数值越大，速度越小
#define CARD_HOVER_SHIFT_SPEED 10

#define L_CARD_INFO_POS sf::Vector2i(800,500)
#define R_CARD_INFO_POS sf::Vector2i(1700,500)
#define LOUZHU_CARD_INFO_POS sf::Vector2i(200,500)

#define LOCAL_LOUZHU_POS sf::Vector2f(0.0, 720.0)       // 当本地玩家是楼主时，楼主应在游戏画面左下方
#define LOCAL_LEFT_BAYOU_POS sf::Vector2f(0.0, 0.0)
#define LOCAL_RIGHT_BAYOU_POS sf::Vector2f(1663.0, 0.0)

#define LOCAL_LOUZHU_POINT_POS sf::Vector2f(128.5, 900.0)
#define LOCAL_LEFT_BAYOU_POINT_POS sf::Vector2f(128.5, 180.0)
#define LOCAL_RIGHT_BAYOU_POINT_POS sf::Vector2f(1791.5, 180.0)

#define LOUZHU_POS sf::Vector2f(1663.0, 0.0) 
#define LEFT_BAYOU_POS sf::Vector2f(0.0, 720.0)
#define RIGHT_BAYOU_POS sf::Vector2f(1663.0, 720.0)

#define LOUZHU_POINT_POS sf::Vector2f(1791.5, 180.0)
#define LEFT_BAYOU_POINT_POS sf::Vector2f(128.5, 900.0)
#define RIGHT_BAYOU_POINT_POS sf::Vector2f(1791.5, 900.0)

#define LOUZHU_HP_POS sf::Vector2f(1633.0, 0.0)
#define LEFT_BAYOU_HP_POS sf::Vector2f(267.0, 720.0)
#define RIGHT_BAYOU_HP_POS sf::Vector2f(1633.0, 720.0)

#define LOCAL_LOUZHU_HP_POS sf::Vector2f(267.0, 720.0)
#define LOCAL_LEFT_HP_POS sf::Vector2f(267.0, 0.0)
#define LOCAL_RIGHT_HP_POS sf::Vector2f(1633.0, 0.0)

#define PLAYER_WIDTH 257  // 玩家头像的长宽，用于制作血量变化蒙版
#define PLAYER_HEIGHT 360


typedef enum PLAYER_KIND
{
	LOUZHU,         //楼主
	LEFT_BAYOU,     //左边的吧友
	RIGHT_BAYOU,    //右边的吧友
	UNDETERMINED    //游戏刚开始，还未确定身份
};

class player
{
public:

	int playerHp;
	int LastplayerHp;
	int playerID;
	int playerCardRotateCount;
	int hover_card_index;
	PLAYER_KIND playerKind;


	bool isPlayerAlive;
	bool isCardInHandChange;  //玩家手牌发生变化，则需要重新计算卡牌旋转角度，并对卡牌进行旋转
	bool isCardsAngleSet;  //判断每张卡牌的旋转值是否已经被确定，避免重复计算
	bool isCardInPosition; //判断卡牌是否已经旋转就位
	bool isHoverInCard;
	bool isSetRotateAngle;

	sf::Vector2i CardHomePosition; //该玩家的牌的初始位置
	sf::FloatRect CardArea; //该玩家牌的初始区域
	sf::FloatRect playerRect; //3个player的点击区域
	sf::Vector2i RotateCenter; //该玩家牌的旋转中心
	sf::Sprite sCard_Info;
	std::vector<pai>pai_list;  //玩家的手牌
	sf::Vector2i CardInfoPos;

	player();
	player(PLAYER_KIND);
	void drawCard(sf::RenderWindow*);
	void drawCardInRotation(sf::RenderWindow*, int);
	void drawCardInPosition(sf::RenderWindow*);
	void getCard(std::queue<pai>*, int num);//抽牌
	void setCardPosition(pai&, float); //做一个封装，设置牌的位置和旋转角度

	void unpackData(sf::Packet&);
	void packData(sf::Packet&);

	bool Hp_decreased = false;
	bool Hp_increased = false;
};



