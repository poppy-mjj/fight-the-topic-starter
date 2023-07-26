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
	ini_pos = homePosition;   //不同玩家旋转前的牌的初始位置点不同
	init_rec = cardArea;      //不同玩家的牌的初始区域不同
	ori = rotateCenter;       //不同玩家的牌的旋转中心点不同
	sCard.setOrigin(ori.x, ori.y);  //抽卡时就可以通过玩家种类先行设置牌的旋转中心（定位点）
}
void pai::LoadCardIMG(sf::Texture& t)
{
	tCard = t;
	sCard.setTexture(t);
	//std::cout << "牌的ID是: " << cardId << std::endl;
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

	////取出纹理对象除了纹理信息外的所有属性
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

	////精灵对象除了纹理值，全都已经赋值
	//sf::Vector2f sCard_pos(0.0, 0.0);
	//float sCard_rotateAngle = 0.0;
	//sf::Color sCard_color(0.0, 0.0, 0.0);
	//packet >> sCard_pos.x >> sCard_pos.y >> sCard_rotateAngle >> sCard_color.a >> sCard_color.g >> sCard_color.b;

	//sCard.setPosition(sCard_pos);
	//sCard.setRotation(sCard_rotateAngle);
	//sCard.setColor(sCard_color);
	//sCard.setOrigin(ori.x, ori.y);

}
