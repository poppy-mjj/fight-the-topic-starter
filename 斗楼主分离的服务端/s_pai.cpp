#pragma once
#include "s_pai.h"

pai::pai()
{

}
pai::pai(int ID)
{
	cardID = ID;
	startAngle = 0;
	endAngle = 0;
	rotationAngle = 0;
	isHover = false;
}
void pai::setParameter(sf::Vector2i homePosition, sf::IntRect cardArea, sf::Vector2i rotateCenter)
{
	ini_pos = homePosition;   //��ͬ�����תǰ���Ƶĳ�ʼλ�õ㲻ͬ
	init_rec = cardArea;      //��ͬ��ҵ��Ƶĳ�ʼ����ͬ
	ori = rotateCenter;       //��ͬ��ҵ��Ƶ���ת���ĵ㲻ͬ
	sCard.setOrigin(ori.x, ori.y);  //�鿨ʱ�Ϳ���ͨ������������������Ƶ���ת���ģ���λ�㣩
}
void pai::LoadCardIMG(sf::Texture& t)
{
	tCard = t;
	sCard.setTexture(t);
	//std::cout << "�Ƶ�ID��: " << cardId << std::endl;
}
sf::Vector2i pai::beforeTran(sf::Vector2i mPoint)
{
	sf::Transform reverse_rotation;
	sf::Vector2f center = sCard.getPosition();
	reverse_rotation.rotate(-endAngle, center);
	sf::Vector2f point = reverse_rotation.transformPoint(mPoint.x, mPoint.y);
	sf::Vector2i tran_mPoint(point.x, point.y);


	return tran_mPoint;
}
void pai::IniHaveObj()
{
	if (cardID >= 17 ||
		cardID == 15 ||
		cardID == 14 ||
		cardID == 13 ||
		cardID == 10 ||
		cardID == 9 ||
		cardID == 5 ||
		cardID == 4 ||
		cardID == 3)
		have_obj = true;

}

void pai::packData(sf::Packet& packet)
{
	packet << rotationAngle << startAngle << endAngle << cardID << have_obj << card_gray << isHover
		<< have_partner << ori.x << ori.y << ini_pos.x << ini_pos.y << init_rec.left << init_rec.top
		<< init_rec.width << init_rec.height;

	////ȡ������������������Ϣ�����������
	//sf::Vector2f sCard_pos = sCard.getPosition();
	//float sCard_rotateAngle = sCard.getRotation();
	//sf::Color sCard_color = sCard.getColor();
	//packet << sCard_pos.x << sCard_pos.y << sCard_rotateAngle << sCard_color.a << sCard_color.g << sCard_color.b;
}

void pai::unpackData(sf::Packet& packet)
{
	packet >> rotationAngle >> startAngle >> endAngle >> cardID >> have_obj >> card_gray >> isHover
		>> have_partner >> ori.x >> ori.y >> ini_pos.x >> ini_pos.y >> init_rec.left >> init_rec.top
		>> init_rec.width >> init_rec.height;

	////��������������ֵ��ȫ���Ѿ���ֵ
	//sf::Vector2f sCard_pos(0.0, 0.0);
	//float sCard_rotateAngle = 0.0;
	//sf::Color sCard_color(0.0, 0.0, 0.0);
	//packet >> sCard_pos.x >> sCard_pos.y >> sCard_rotateAngle >> sCard_color.a >> sCard_color.g >> sCard_color.b;

	//sCard.setPosition(sCard_pos);
	//sCard.setRotation(sCard_rotateAngle);
	//sCard.setColor(sCard_color);
	//sCard.setOrigin(ori.x, ori.y);

}
