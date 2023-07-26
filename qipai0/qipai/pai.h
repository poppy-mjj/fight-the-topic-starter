#pragma once
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<SFML/Network.hpp>
#include<windows.h>
#include<iostream>
#include<sstream>
#include <queue>


#define CARDID_BEGIN 1  //��ƬID�Ŀ�ʼ����
#define CARDID_END 25    //��ƬID�Ľ�������
#define CARDID_MIDDLE 17   //��������ʼ������

#define CARD_WIDTH 121 //���ƴ�С��121 * 200
#define CARD_HEIGHT 200 

//����ͼ��С ��1920 * 1080
#define BG_WIDTH 1920
#define BG_HEIGHT 1080

//���������λ�ã� x : 710  y : 790
//�Ұ�������λ�ã� x : 1210  y : 790


#define LOUZHU_CARD_POSTION sf::Vector2i(830, 680) //¥��������תǰ��λ��
#define LOUZHU_ROTATE_ORIGIN sf::Vector2i(CARD_WIDTH / 2, CARD_HEIGHT * 4) //¥��������ת����
#define LOUZHU_CARD_AREA sf::FloatRect(830, 680, CARD_WIDTH, CARD_HEIGHT) //¥���Ƶ���תǰ������

#define LEFT_CARD_POSTION sf::Vector2i(830, 680) //��ߵİ��ѿ�����תǰ��λ��
#define LEFT_ROTATE_ORIGIN sf::Vector2i(CARD_WIDTH / 2, CARD_HEIGHT * 4) //��ߵİ��ѿ�����ת����
#define LEFT_BAYOU_CARD_AREA sf::FloatRect(830, 680, CARD_WIDTH, CARD_HEIGHT) //��ߵİ����Ƶ���תǰ������

#define RIGHT_CARD_POSTION sf::Vector2i(830, 680)                          //�ұߵİ��ѿ�����תǰ��λ��
#define RIGHT_ROTATE_ORIGIN sf::Vector2i(CARD_WIDTH / 2, CARD_HEIGHT * 4)   //�ұߵİ��ѿ�����ת����
#define RIGHT_BAYOU_CARD_AREA sf::FloatRect(830, 680, CARD_WIDTH, CARD_HEIGHT) //�ұߵİ����Ƶ���תǰ������

#define LOUZHU_AREA sf::FloatRect(1663.0, 0.0, 257.0, 360.0)          //¥�����ͷ��������
#define LEFT_BAYOU_AREA sf::FloatRect(0.0, 720.0, 257.0, 360.0)    //��������ͷ��������
#define RIGHT_BAYOU_AREA sf::FloatRect(1663.0, 720.0, 257.0, 360.0)      //�Ұ������ͷ��������


#define LOCAL_LOUZHU_AREA sf::FloatRect(0.0, 720.0, 257.0, 360.0)   
#define LOCAL_LEFT_BAYOU_AREA sf::FloatRect(0.0, 0.0, 257.0, 360.0)   
#define LOCAL_RIGHT_BAYOU_AREA sf::FloatRect(1663.0, 0.0, 257.0, 360.0) 


#define L_BAYOU_ROTATE_START_ANGLE -30.0
#define L_BAYOU_ROTATE_END_ANGLE  -2.0
#define R_BAYOU_ROTATE_START_ANGLE  2.0
#define R_BAYOU_ROTATE_END_ANGLE  30.0
#define LOUZHU_ROTATE_START_ANGLE -30.0
#define LOUZHU_ROTATE_END_ANGLE 30.0

#define RADIUS 366 //��ת�뾶��366







class pai
{
public:
	sf::Vector2i ori;     //��ת���ĵ�(Ҳ������ȫ������ϵ�µĶ�λ��),ע��˱���Ӧ��¼�Ƶľֲ�����ϵ����ʼ��ʱ�����Ƶ����Ͻ���Ϊ����ԭ��
	sf::Vector2i ini_pos; //��תǰ���Ƶĳ�ʼλ�õ�
	sf::FloatRect init_rec; //��תǰ���Ƶĳ�ʼ����

	float rotationAngle; //������Ҫ��ת�ĽǶ�
	float startAngle;    //��ʼ��ת�ĽǶȣ��������л���
	float endAngle;      //ֹͣ��ת�ĽǶȣ� ����Ҫ������תʱ���� ��rotationAngle = endAngle - startAngle��

	sf::Texture tCard;    //�����������
	sf::Sprite  sCard;    //�Ƶľ������
	sf::FloatRect rectCard; //����ʵ�ֵ��
	int cardId;
	bool have_obj = false;

	bool card_gray = true;
	bool isHover;
	bool have_partner = false;

	enum card_State
	{
		trashBin, //���ƶ�
		drawPile, //���ƶ�
		inHand,   //������
	};

	pai();
	pai(int ID);
	void LoadCardIMG(sf::Texture&);
	void setParameter(sf::Vector2i, sf::FloatRect, sf::Vector2i); //ÿ����ҵ��ƵĲ���������ͬ��Ҫ�ֱ�����
	sf::Vector2i beforeTran(sf::Vector2i mPoint); //�����������λ�ã�����������������ת����任
	void IniHaveObj();
	void IniHavePartner();
	bool haveObjOrNot(int num);
	void cardFunc();

	void unpackData(sf::Packet&);
	void packData(sf::Packet&);
};


