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
		if (!isCardsAngleSet) //�����ת�Ƕ�δ������
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

			float perAngle = (rotate_end_angle - rotate_start_angle) / (pai_list.size() + 1);//������ת�Ƕ�
			for (int i = 0; i < pai_list.size(); i++)
			{
				if (pai_list[i].startAngle == 0)
				{
					pai_list[i].endAngle = rotate_start_angle + perAngle * (i + 1); //���Ƹ���ת����λ��
					pai_list[i].startAngle = rotate_start_angle;
					pai_list[i].rotationAngle = pai_list[i].endAngle - pai_list[i].startAngle; //���㿨����Ҫ��ת�ĽǶ�
				}
				else
				{
					pai_list[i].endAngle = rotate_start_angle + perAngle * (i + 1); //���Ƹ���ת����λ��
					pai_list[i].rotationAngle = pai_list[i].endAngle - pai_list[i].startAngle; //���㿨����Ҫ��ת�ĽǶ�
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
		float angle = pai_list[i].rotationAngle / speed; //��ʱ��֡��Ӱ�쿨����ת�ٶ�
		if (isSetRotateAngle == false)
		{
			pai_list[i].startAngle += angle;  //��ʱ�޸�startAngle��������ڿ���ʵ�ֿ��Ƶ�������ת
			setCardPosition(pai_list[i], pai_list[i].startAngle);
		}
		else
		{
			pai_list[i].startAngle += angle;
			setCardPosition(pai_list[i], angle);
		}
		// ����hover���ɰ�
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
			pai_list[i].rotationAngle = 0; //��ת��ɺ󣬰�����Ҫ��ת�ĽǶ��ó�0
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
		// ����hover���ɰ�
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
void player::getCard(std::queue<pai>* draw_piles, int num)//����,numΪ������
{
	for (int i = 0; i < num; i++)
	{
		draw_piles->front().setParameter(CardHomePosition, CardArea, RotateCenter);
		pai_list.emplace_back(draw_piles->front()); // �����ƶ�β����³�ȡ����
		//std::cout << "��n���Ƶ�ID��: " << pai_list.back().cardId << std::endl;
		draw_piles->pop();
	}
	isCardInHandChange = true; //��ҳ��ƺ���������Ӧ�÷����任
	isCardsAngleSet = false;   //��ҳ��ƺ�������ת�ĽǶ�Ӧ��Ҫ���¼���
}
void player::setCardPosition(pai& a_card, float angle)
{
	a_card.sCard.setPosition(a_card.ini_pos.x + a_card.ori.x, a_card.ini_pos.y + a_card.ori.y);//����ori��Ϊ�������ϵ��ƫ����
	a_card.sCard.rotate(angle);
}

void player::packData(sf::Packet& packet)
{
	int playerKind_int = static_cast<int>(playerKind);
	// ��ÿ����Ա��������д��packet
	packet << playerID << playerHp << playerCardRotateCount << hover_card_index << Card_Info_Texture << playerKind_int
		<< isPlayerAlive << isCardInHandChange << isCardsAngleSet << isCardInPosition << isHoverInCard
		<< isSetRotateAngle << CardHomePosition.x << CardHomePosition.y << CardArea.left << CardArea.top
		<< CardArea.width << CardArea.height << playerRect.left << playerRect.top << playerRect.width
		<< playerRect.height << RotateCenter.x << RotateCenter.y << CardInfoPos.x << CardInfoPos.y;

	// ���ÿ��pai���������
	int paiListSize = pai_list.size();
	packet << paiListSize;
	for (int i = 0; i < paiListSize; i++)
	{
		pai_list[i].packData(packet);
	}
	// ���ش���õ�packet
}

void player::unpackData(sf::Packet& packet)
{
	int playerKind_int = -1;
	// ���δ�packet�ж�ȡ���ݲ�������ĳ�Ա����
	packet >> playerID >> playerHp >> playerCardRotateCount >> hover_card_index >> Card_Info_Texture >> playerKind_int
		>> isPlayerAlive >> isCardInHandChange >> isCardsAngleSet >> isCardInPosition >> isHoverInCard
		>> isSetRotateAngle >> CardHomePosition.x >> CardHomePosition.y >> CardArea.left >> CardArea.top
		>> CardArea.width >> CardArea.height >> playerRect.left >> playerRect.top >> playerRect.width
		>> playerRect.height >> RotateCenter.x >> RotateCenter.y >> CardInfoPos.x >> CardInfoPos.y;

	playerKind = static_cast<PLAYER_KIND>(playerKind_int);

	// ���ÿ��pai���������
	int paiListSize = 0;
	packet >> paiListSize;
	pai_list.resize(paiListSize);
	for (int i = 0; i < paiListSize; i++)
	{
		pai_list[i].unpackData(packet);
	}
}
