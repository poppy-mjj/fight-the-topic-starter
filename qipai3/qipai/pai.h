#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<windows.h>
#include<iostream>
#include<sstream>
#include <queue>


#define CARDID_BEGIN 1  //卡片ID的开始数字
#define CARDID_END 25    //卡片ID的结束数字
#define CARDID_MIDDLE 17   //基础卡开始的数字

#define CARD_WIDTH 121 //卡牌大小：121 * 200
#define CARD_HEIGHT 200 

//背景图大小 ：1920 * 1080
#define BG_WIDTH 1920
#define BG_HEIGHT 1080

//左吧友中心位置： x : 710  y : 790
//右吧友中心位置： x : 1210  y : 790


#define LOUZHU_CARD_POSTION sf::Vector2i(830, 680) //楼主卡牌旋转前的位置
#define LOUZHU_ROTATE_ORIGIN sf::Vector2i(CARD_WIDTH / 2, CARD_HEIGHT * 4) //楼主卡牌旋转中心
#define LOUZHU_CARD_AREA sf::FloatRect(830, 680, CARD_WIDTH, CARD_HEIGHT) //楼主牌的旋转前的区域

#define LEFT_CARD_POSTION sf::Vector2i(830, 680) //左边的吧友卡牌旋转前的位置
#define LEFT_ROTATE_ORIGIN sf::Vector2i(CARD_WIDTH / 2, CARD_HEIGHT * 4) //左边的吧友卡牌旋转中心
#define LEFT_BAYOU_CARD_AREA sf::FloatRect(830, 680, CARD_WIDTH, CARD_HEIGHT) //左边的吧友牌的旋转前的区域

#define RIGHT_CARD_POSTION sf::Vector2i(830, 680)                          //右边的吧友卡牌旋转前的位置
#define RIGHT_ROTATE_ORIGIN sf::Vector2i(CARD_WIDTH / 2, CARD_HEIGHT * 4)   //右边的吧友卡牌旋转中心
#define RIGHT_BAYOU_CARD_AREA sf::FloatRect(830, 680, CARD_WIDTH, CARD_HEIGHT) //右边的吧友牌的旋转前的区域

#define LOUZHU_AREA sf::FloatRect(1663.0, 0.0, 257.0, 360.0)          //楼主玩家头像点击区域
#define LEFT_BAYOU_AREA sf::FloatRect(0.0, 720.0, 257.0, 360.0)    //左吧友玩家头像点击区域
#define RIGHT_BAYOU_AREA sf::FloatRect(1663.0, 720.0, 257.0, 360.0)      //右吧友玩家头像点击区域


#define LOCAL_LOUZHU_AREA sf::FloatRect(0.0, 720.0, 257.0, 360.0)   
#define LOCAL_LEFT_BAYOU_AREA sf::FloatRect(0.0, 0.0, 257.0, 360.0)   
#define LOCAL_RIGHT_BAYOU_AREA sf::FloatRect(1663.0, 0.0, 257.0, 360.0) 


#define L_BAYOU_ROTATE_START_ANGLE -30.0
#define L_BAYOU_ROTATE_END_ANGLE  -2.0
#define R_BAYOU_ROTATE_START_ANGLE  2.0
#define R_BAYOU_ROTATE_END_ANGLE  30.0
#define LOUZHU_ROTATE_START_ANGLE -30.0
#define LOUZHU_ROTATE_END_ANGLE 30.0

#define RADIUS 366 //旋转半径：366







class pai
{
public:
	sf::Vector2i ori;     //旋转中心点(也是牌在全局坐标系下的定位点),注意此变量应记录牌的局部坐标系，初始的时候以牌的左上角作为坐标原点
	sf::Vector2i ini_pos; //旋转前的牌的初始位置点
	sf::FloatRect init_rec; //旋转前的牌的初始区域

	float rotationAngle; //卡牌需要旋转的角度
	float startAngle;    //开始旋转的角度，用来进行绘制
	float endAngle;      //停止旋转的角度， 刚需要进行旋转时满足 “rotationAngle = endAngle - startAngle”

	sf::Texture tCard;    //创建纹理对象
	sf::Sprite  sCard;    //牌的精灵对象
	sf::FloatRect rectCard; //方便实现点击
	int cardId;
	bool have_obj = false;

	bool card_gray = true;
	bool isHover;
	bool have_partner = false;

	enum card_State
	{
		trashBin, //弃牌堆
		drawPile, //抽牌堆
		inHand,   //手牌上
	};

	pai();
	pai(int ID);
	void LoadCardIMG(sf::Texture&);
	void setParameter(sf::Vector2i, sf::FloatRect, sf::Vector2i); //每名玩家的牌的参数都不相同，要分别设置
	sf::Vector2i beforeTran(sf::Vector2i mPoint); //传入鼠标点击的位置，对鼠标点击点进行排旋转的逆变换
	void IniHaveObj();
	void IniHavePartner();
	bool haveObjOrNot(int num);
	void cardFunc();

	void unpackData(sf::Packet&);
	void packData(sf::Packet&);
};


