#pragma once
#include "s_pai.h"
#include<queue>

#define LOUZHU_HP 100
#define BAYOU_HP 50
#define LOUZHU_INITIAL_CARD_AMOUNT 8
#define BAYOU_INITIAL_CARD_AMOUNT 4
#define CARD_ROTATE_SPEED 20  //数值越大，速度越小
#define CARD_HOVER_SHIFT_SPEED 10

#define L_CARD_INFO_POS sf::Vector2i(800,500)
#define R_CARD_INFO_POS sf::Vector2i(1700,500)
#define LOUZHU_CARD_INFO_POS sf::Vector2i(200,500)


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
	int playerID;  //记录玩家选择的是哪一个角色，后面作为纹理传输到网络
	int playerHp;
	int playerCardRotateCount;
	int hover_card_index;
	int Card_Info_Texture;  //记录sCard_Info的纹理是纹理数组内的哪一个
	PLAYER_KIND playerKind;


	bool isPlayerAlive;
	bool isCardInHandChange;  //玩家手牌发生变化，则需要重新计算卡牌旋转角度，并对卡牌进行旋转
	bool isCardsAngleSet;  //判断每张卡牌的旋转值是否已经被确定，避免重复计算
	bool isCardInPosition; //判断卡牌是否已经旋转就位
	bool isHoverInCard;
	bool isSetRotateAngle;

	sf::Vector2i CardHomePosition; //该玩家的牌的初始位置
	sf::IntRect CardArea; //该玩家牌的初始区域
	sf::IntRect playerRect; //3个player的点击区域
	sf::Vector2i RotateCenter; //该玩家牌的旋转中心
	sf::Vector2i CardInfoPos;
	std::vector<pai>pai_list;  //玩家的手牌

	sf::Sprite sCard_Info;  //这个精灵可能不用传到网络中，它有牌的精灵赋值得到

	player();
	player(PLAYER_KIND);
	void drawCard(sf::RenderWindow*);
	void drawCardInRotation(sf::RenderWindow*, int);
	void drawCardInPosition(sf::RenderWindow*);
	void getCard(std::queue<pai>*, int num);//抽牌
	void setCardPosition(pai&, float); //做一个封装，设置牌的位置和旋转角度

	void packData(sf::Packet& packet);
	void unpackData(sf::Packet& packet);

};




