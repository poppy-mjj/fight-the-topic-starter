#pragma once
#include "s_player.h"

player::player()
{
	isCardInHandChange = false;
	isCardsAngleSet = false;
	playerCardRotateCount = 0;
	isHoverInCard = false;
	hover_card_index = -1;
	isSetRotateAngle = false;
}
player::player(PLAYER_KIND kind)
{
	if (kind == LOUZHU)
	{
		playerHp = LOUZHU_HP;
		CardHomePosition = LOUZHU_CARD_POSTION;
		CardArea = LOUZHU_CARD_AREA;
		RotateCenter = LOUZHU_ROTATE_ORIGIN;
	}
	else if (kind == LEFT_BAYOU)
	{
		playerHp = BAYOU_HP;
		CardHomePosition = LEFT_CARD_POSTION;
		CardArea = LEFT_BAYOU_CARD_AREA;
		RotateCenter = LEFT_ROTATE_ORIGIN;
	}
	else
	{
		playerHp = BAYOU_HP;
		CardHomePosition = RIGHT_CARD_POSTION;
		CardArea = RIGHT_BAYOU_CARD_AREA;
		RotateCenter = RIGHT_ROTATE_ORIGIN;
	}
	isPlayerAlive = true;

}

void player::drawCard(sf::RenderWindow* pWindow)
{
	if (isCardInHandChange)
	{
		if (!isCardsAngleSet) //如果旋转角度未被设置
		{
			int rotate_start_angle;
			int rotate_end_angle;
			if (playerKind == LEFT_BAYOU)
			{
				rotate_start_angle = L_BAYOU_ROTATE_START_ANGLE;
				rotate_end_angle = L_BAYOU_ROTATE_END_ANGLE;
			}
			else if (playerKind == RIGHT_BAYOU)
			{
				rotate_start_angle = R_BAYOU_ROTATE_START_ANGLE;
				rotate_end_angle = R_BAYOU_ROTATE_END_ANGLE;
			}
			else if (playerKind == LOUZHU)
			{
				rotate_start_angle = LOUZHU_ROTATE_START_ANGLE;
				rotate_end_angle = LOUZHU_ROTATE_END_ANGLE;
			}

			float perAngle = (rotate_end_angle - rotate_start_angle) / (pai_list.size() + 1);//手牌旋转角度
			for (int i = 0; i < pai_list.size(); i++)
			{
				if (pai_list[i].startAngle == 0)
				{
					pai_list[i].endAngle = rotate_start_angle + perAngle * (i + 1); //卡牌该旋转到的位置
					pai_list[i].startAngle = rotate_start_angle;
					pai_list[i].rotationAngle = pai_list[i].endAngle - pai_list[i].startAngle; //计算卡牌需要旋转的角度
				}
				else
				{
					pai_list[i].endAngle = rotate_start_angle + perAngle * (i + 1); //卡牌该旋转到的位置
					pai_list[i].rotationAngle = pai_list[i].endAngle - pai_list[i].startAngle; //计算卡牌需要旋转的角度
				}

			}
			isCardsAngleSet = true;
		}
		if (isHoverInCard)
		{
			drawCardInRotation(pWindow, CARD_HOVER_SHIFT_SPEED);
		}
		else
		{
			drawCardInRotation(pWindow, CARD_ROTATE_SPEED);
		}


	}
	else
	{
		drawCardInPosition(pWindow);
	}


}
void player::drawCardInRotation(sf::RenderWindow* pWindow, int speed)
{
	playerCardRotateCount++;
	for (int i = 0; i < pai_list.size(); i++)
	{
		float angle = pai_list[i].rotationAngle / speed; //暂时以帧率影响卡牌旋转速度
		if (isSetRotateAngle == false)
		{
			pai_list[i].startAngle += angle;  //及时修改startAngle，方便后期可以实现卡牌的任意旋转
			setCardPosition(pai_list[i], pai_list[i].startAngle);
		}
		else
		{
			pai_list[i].startAngle += angle;
			setCardPosition(pai_list[i], angle);
		}
		// 设置hover的蒙版
		if (pai_list[i].isHover == true)
		{
			pai_list[i].sCard.setColor(sf::Color::Yellow);
			if (pai_list[i].card_gray == true)
			{
				pai_list[i].sCard.setColor((sf::Color(100, 100, 100, 255) * sf::Color::Yellow));
			}
		}
		else if (pai_list[i].card_gray == true)
		{
			pai_list[i].sCard.setColor(sf::Color(100, 100, 100, 255));
		}
		else
		{
			pai_list[i].sCard.setColor(sf::Color(255, 255, 255));
		}

		pWindow->draw(pai_list[i].sCard);
		if (playerCardRotateCount == speed)
		{
			pai_list[i].rotationAngle = 0; //旋转完成后，把牌需要旋转的角度置成0
		}
	}
	isSetRotateAngle = true;
	if (playerCardRotateCount == speed)
	{
		playerCardRotateCount = 0;
		isCardInPosition = true;
		isCardInHandChange = false;
	}
}
void player::drawCardInPosition(sf::RenderWindow* pWindow)
{
	for (int i = 0; i < pai_list.size(); i++)
	{
		// 设置hover的蒙版
		if (pai_list[i].isHover == true)
		{
			pai_list[i].sCard.setColor(sf::Color::Yellow);
			if (pai_list[i].card_gray == true)
			{
				pai_list[i].sCard.setColor((sf::Color(100, 100, 100, 255) * sf::Color::Yellow));
			}
		}
		else if (pai_list[i].card_gray == true)
		{
			pai_list[i].sCard.setColor(sf::Color(100, 100, 100, 255));
		}
		else
		{
			pai_list[i].sCard.setColor(sf::Color(255, 255, 255));
		}


		pWindow->draw(pai_list[i].sCard);
	}
}
void player::getCard(std::queue<pai>* draw_piles, int num)//抽牌,num为抽牌数
{
	for (int i = 0; i < num; i++)
	{
		draw_piles->front().setParameter(CardHomePosition, CardArea, RotateCenter);
		pai_list.emplace_back(draw_piles->front()); // 在手牌队尾添加新抽取的牌
		//std::cout << "第n张牌的ID是: " << pai_list.back().cardId << std::endl;
		draw_piles->pop();
	}
	isCardInHandChange = true; //玩家抽牌后手牌总数应该发生变换
	isCardsAngleSet = false;   //玩家抽牌后手牌旋转的角度应该要重新计算
}
void player::setCardPosition(pai& a_card, float angle)
{
	a_card.sCard.setPosition(a_card.ini_pos.x + a_card.ori.x, a_card.ini_pos.y + a_card.ori.y);//抵消ori不为卡牌左上点的偏移量
	a_card.sCard.rotate(angle);
}

void player::packData(sf::Packet& packet)
{
	int playerKind_int = static_cast<int>(playerKind);
	// 将每个成员变量依次写入packet
	packet << playerID << playerHp << playerCardRotateCount << hover_card_index << Card_Info_Texture << playerKind_int
		<< isPlayerAlive << isCardInHandChange << isCardsAngleSet << isCardInPosition << isHoverInCard
		<< isSetRotateAngle << CardHomePosition.x << CardHomePosition.y << CardArea.left << CardArea.top
		<< CardArea.width << CardArea.height << playerRect.left << playerRect.top << playerRect.width
		<< playerRect.height << RotateCenter.x << RotateCenter.y << CardInfoPos.x << CardInfoPos.y;

	// 打包每张pai对象的数据
	int paiListSize = pai_list.size();
	packet << paiListSize;
	for (int i = 0; i < paiListSize; i++)
	{
		pai_list[i].packData(packet);
	}
	// 返回打包好的packet
}

void player::unpackData(sf::Packet& packet)
{
	int playerKind_int = -1;
	// 依次从packet中读取数据并设置类的成员变量
	packet >> playerID >> playerHp >> playerCardRotateCount >> hover_card_index >> Card_Info_Texture >> playerKind_int
		>> isPlayerAlive >> isCardInHandChange >> isCardsAngleSet >> isCardInPosition >> isHoverInCard
		>> isSetRotateAngle >> CardHomePosition.x >> CardHomePosition.y >> CardArea.left >> CardArea.top
		>> CardArea.width >> CardArea.height >> playerRect.left >> playerRect.top >> playerRect.width
		>> playerRect.height >> RotateCenter.x >> RotateCenter.y >> CardInfoPos.x >> CardInfoPos.y;

	playerKind = static_cast<PLAYER_KIND>(playerKind_int);

	// 解包每张pai对象的数据
	int paiListSize = 0;
	packet >> paiListSize;
	pai_list.resize(paiListSize);
	for (int i = 0; i < paiListSize; i++)
	{
		pai_list[i].unpackData(packet);
	}
}
