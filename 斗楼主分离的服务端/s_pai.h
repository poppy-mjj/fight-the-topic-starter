#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Network.hpp>


#define CARDID_BEGIN 1  //��ƬID�Ŀ�ʼ����
#define CARDID_END 24    //��ƬID�Ľ�������
#define CARDID_MIDDLE 17   //��������ʼ������

#define CARD_WIDTH 121 //���ƴ�С��116 * 154
#define CARD_HEIGHT 200 

//����ͼ��С ��1920 * 1080
#define BG_WIDTH 1920
#define BG_HEIGHT 1080

//���������λ�ã� x : 710  y : 790
//�Ұ�������λ�ã� x : 1210  y : 790


#define LOUZHU_CARD_POSTION sf::Vector2i(100, 100) //¥��������תǰ��λ��
#define LOUZHU_CARD_AREA sf::IntRect(10,10,CARD_WIDTH,CARD_HEIGHT) //¥���Ƶ���תǰ������
#define LOUZHU_ROTATE_ORIGIN sf::Vector2i(960, 290) //¥��������ת����

#define LEFT_CARD_POSTION sf::Vector2i(600, 400) //��ߵİ��ѿ�����תǰ��λ��
#define LEFT_ROTATE_ORIGIN sf::Vector2i(CARD_WIDTH / 2, CARD_HEIGHT * 3) //��ߵİ��ѿ�����ת����
#define LEFT_BAYOU_CARD_AREA sf::IntRect(600,400,CARD_WIDTH,CARD_HEIGHT) //��ߵİ����Ƶ���תǰ������

#define RIGHT_CARD_POSTION sf::Vector2i(1199, 400) //�ұߵİ��ѿ�����תǰ��λ��
#define RIGHT_ROTATE_ORIGIN sf::Vector2i(CARD_WIDTH / 2, CARD_HEIGHT * 3)//�ұߵİ��ѿ�����ת����
#define RIGHT_BAYOU_CARD_AREA sf::IntRect(1199, 400,CARD_WIDTH,CARD_HEIGHT) //�ұߵİ����Ƶ���תǰ������

#define LOUZHU_AREA sf::IntRect(910, 0,157,200) //�ұߵİ����Ƶ���תǰ������
#define LEFT_BAYOU_AREA sf::IntRect(800, 700,186,200) //�ұߵİ����Ƶ���תǰ������
#define RIGHT_BAYOU_AREA sf::IntRect(1020, 700,167,200) //�ұߵİ����Ƶ���תǰ������



#define L_BAYOU_ROTATE_START_ANGLE -60.0
#define L_BAYOU_ROTATE_END_ANGLE  -15.0
#define R_BAYOU_ROTATE_START_ANGLE  15.0
#define R_BAYOU_ROTATE_END_ANGLE  60.0
#define LOUZHU_ROTATE_START_ANGLE -60.0
#define LOUZHU_ROTATE_END_ANGLE 60.0

#define RADIUS 366 //��ת�뾶��366

enum card_State
{
	trashBin, //���ƶ�
	drawPile, //���ƶ�
	inHand,   //������
};


class pai
{
public:
	float rotationAngle; //������Ҫ��ת�ĽǶ�
	float startAngle;    //��ʼ��ת�ĽǶȣ��������л���
	float endAngle;      //ֹͣ��ת�ĽǶȣ� ����Ҫ������תʱ���� ��rotationAngle = endAngle - startAngle��

	int cardID;  //����ͨ����������ҵ����Ƶ��������Կ����������������tCard�Ĵ���
	bool have_obj = false;

	bool card_gray = false;
	bool isHover;
	bool have_partner = false;

	sf::Vector2i ori;     //��ת���ĵ�(Ҳ������ȫ������ϵ�µĶ�λ��),ע��˱���Ӧ��¼�Ƶľֲ�����ϵ����ʼ��ʱ�����Ƶ����Ͻ���Ϊ����ԭ��
	sf::Vector2i ini_pos; //��תǰ���Ƶĳ�ʼλ�õ�
	sf::IntRect init_rec; //��תǰ���Ƶĳ�ʼ����

	sf::Texture tCard;    //�����������
	sf::Sprite  sCard;    //�Ƶľ������


	pai();
	pai(int ID);
	void LoadCardIMG(sf::Texture&);
	void setParameter(sf::Vector2i, sf::IntRect, sf::Vector2i); //ÿ����ҵ��ƵĲ���������ͬ��Ҫ�ֱ�����
	sf::Vector2i beforeTran(sf::Vector2i mPoint); //�����������λ�ã�����������������ת����任
	void IniHaveObj();
	void cardFunc();

	void packData(sf::Packet& packet);
	void unpackData(sf::Packet& packet);
};

