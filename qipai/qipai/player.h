#pragma once
#include "pai.h"

#define HP_WIDTH 25

#define LOUZHU_HP 140
#define BAYOU_HP 100
#define LOUZHU_INITIAL_CARD_AMOUNT 4
#define LOUZHU_GET_CARD_AMOUNT 4
#define BAYOU_INITIAL_CARD_AMOUNT 2
#define BAYOU_GET_CARD_AMOUNT 2
#define CARD_ROTATE_SPEED 20  //��ֵԽ���ٶ�ԽС
#define CARD_HOVER_SHIFT_SPEED 10

#define L_CARD_INFO_POS sf::Vector2i(800,500)
#define R_CARD_INFO_POS sf::Vector2i(1700,500)
#define LOUZHU_CARD_INFO_POS sf::Vector2i(200,500)

#define LOCAL_LOUZHU_POS sf::Vector2f(0.0, 720.0)       // �����������¥��ʱ��¥��Ӧ����Ϸ�������·�
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

#define PLAYER_WIDTH 257  // ���ͷ��ĳ�����������Ѫ���仯�ɰ�
#define PLAYER_HEIGHT 360


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

	int playerHp;
	int LastplayerHp;
	int playerID;
	int playerCardRotateCount;
	int hover_card_index;
	PLAYER_KIND playerKind;


	bool isPlayerAlive;
	bool isCardInHandChange;  //������Ʒ����仯������Ҫ���¼��㿨����ת�Ƕȣ����Կ��ƽ�����ת
	bool isCardsAngleSet;  //�ж�ÿ�ſ��Ƶ���תֵ�Ƿ��Ѿ���ȷ���������ظ�����
	bool isCardInPosition; //�жϿ����Ƿ��Ѿ���ת��λ
	bool isHoverInCard;
	bool isSetRotateAngle;

	sf::Vector2i CardHomePosition; //����ҵ��Ƶĳ�ʼλ��
	sf::FloatRect CardArea; //������Ƶĳ�ʼ����
	sf::FloatRect playerRect; //3��player�ĵ������
	sf::Vector2i RotateCenter; //������Ƶ���ת����
	sf::Sprite sCard_Info;
	std::vector<pai>pai_list;  //��ҵ�����
	sf::Vector2i CardInfoPos;

	player();
	player(PLAYER_KIND);
	void drawCard(sf::RenderWindow*);
	void drawCardInRotation(sf::RenderWindow*, int);
	void drawCardInPosition(sf::RenderWindow*);
	void getCard(std::queue<pai>*, int num);//����
	void setCardPosition(pai&, float); //��һ����װ�������Ƶ�λ�ú���ת�Ƕ�

	void unpackData(sf::Packet&);
	void packData(sf::Packet&);

	bool Hp_decreased = false;
	bool Hp_increased = false;
};



