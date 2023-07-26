#pragma once
#include "s_pai.h"
#include<queue>

#define LOUZHU_HP 100
#define BAYOU_HP 50
#define LOUZHU_INITIAL_CARD_AMOUNT 8
#define BAYOU_INITIAL_CARD_AMOUNT 4
#define CARD_ROTATE_SPEED 20  //��ֵԽ���ٶ�ԽС
#define CARD_HOVER_SHIFT_SPEED 10

#define L_CARD_INFO_POS sf::Vector2i(800,500)
#define R_CARD_INFO_POS sf::Vector2i(1700,500)
#define LOUZHU_CARD_INFO_POS sf::Vector2i(200,500)


typedef enum PLAYER_KIND
{
	LOUZHU,         //¥��
	LEFT_BAYOU,     //��ߵİ���
	RIGHT_BAYOU,    //�ұߵİ���
	UNDETERMINED    //��Ϸ�տ�ʼ����δȷ�����
};


class player
{
public:
	int playerID;  //��¼���ѡ�������һ����ɫ��������Ϊ�����䵽����
	int playerHp;
	int playerCardRotateCount;
	int hover_card_index;
	int Card_Info_Texture;  //��¼sCard_Info�����������������ڵ���һ��
	PLAYER_KIND playerKind;


	bool isPlayerAlive;
	bool isCardInHandChange;  //������Ʒ����仯������Ҫ���¼��㿨����ת�Ƕȣ����Կ��ƽ�����ת
	bool isCardsAngleSet;  //�ж�ÿ�ſ��Ƶ���תֵ�Ƿ��Ѿ���ȷ���������ظ�����
	bool isCardInPosition; //�жϿ����Ƿ��Ѿ���ת��λ
	bool isHoverInCard;
	bool isSetRotateAngle;

	sf::Vector2i CardHomePosition; //����ҵ��Ƶĳ�ʼλ��
	sf::IntRect CardArea; //������Ƶĳ�ʼ����
	sf::IntRect playerRect; //3��player�ĵ������
	sf::Vector2i RotateCenter; //������Ƶ���ת����
	sf::Vector2i CardInfoPos;
	std::vector<pai>pai_list;  //��ҵ�����

	sf::Sprite sCard_Info;  //���������ܲ��ô��������У������Ƶľ��鸳ֵ�õ�

	player();
	player(PLAYER_KIND);
	void drawCard(sf::RenderWindow*);
	void drawCardInRotation(sf::RenderWindow*, int);
	void drawCardInPosition(sf::RenderWindow*);
	void getCard(std::queue<pai>*, int num);//����
	void setCardPosition(pai&, float); //��һ����װ�������Ƶ�λ�ú���ת�Ƕ�

	void packData(sf::Packet& packet);
	void unpackData(sf::Packet& packet);

};




