#include "s_Game.h"


Game::Game()
{
	Window_width = BG_WIDTH;
	Window_height = BG_HEIGHT;
	received = 0;

	socket1.setBlocking(false);
	socket2.setBlocking(false);
	socket3.setBlocking(false);
}
Game::~Game() {
}

void Game::Initial()
{
	is_first_send = true;

	is_send1 = false;
	is_send2 = false;
	is_send3 = false;

	sk1R = false;
	sk2R = false;
	sk3R = false;

	sk1S = false;
	sk2S = false;
	sk3S = false;

	sk1H = false; // ���Ӣ�۳�ʼ����Ӧ
	sk2H = false;
	sk3H = false;

	sk1Re = false;
	sk2Re = false;
	sk3Re = false;

	is_game_begin = false;
	is_gameRestart = false;

	hp1 = 0;
	hp2 = 0;
	hp3 = 0;
	sending_card.clear();
	sending_card_objs.clear();
	received_card_objs.clear();
	received_card.clear();
	std::queue<pai> empty;
	std::swap(empty, drawPiles);

	srand((unsigned)time(NULL));
	int length = 0;
	player1_hero_id = 9;
	player2_hero_id = 9;
	player3_hero_id = 9;
	for (int i = 0; i < 3; i++)
	{
		int num = rand() % 3; // ����0, 1, 2�����
		bool is_same = false;
		for (int j = 0; j < length; j++)
		{
			if (num == playerTrueId[j])
			{
				is_same = true;
				break;
			}
		}
		if (is_same == true)
		{
			i--;
			continue;
		}
		playerTrueId[i] = num;
		length++;
	}

	if (is_first_round)
	{
		listener.listen(55001); //���ü����Ķ˿�
		std::cout << "�ȴ���Ҽ���: " << std::endl;
		listener.accept(socket1);  //�ȴ��ͻ������ӣ����û��������һֱ�ȴ�
		std::cout << "��һ��������ӳɹ����ȴ�����������Ҽ���: " << socket1.getRemoteAddress() << std::endl;
		//std::string message = "connected";
		//socket1.send(message.c_str(), message.size() + 1); // �����1�������ӳɹ���Ϣ

		listener.accept(socket2);  //�ȴ��ͻ������ӣ����û��������һֱ�ȴ�
		std::cout << "�ڶ���������ӳɹ����ȴ�����һ����Ҽ���: " << socket2.getRemoteAddress() << std::endl;
		// message = "connected";
		// socket2.send(message.c_str(), message.size() + 1); // �����2�������ӳɹ���Ϣ

		listener.accept(socket3);  //�ȴ��ͻ������ӣ����û��������һֱ�ȴ�
		std::cout << "������������ӳɹ������ܶ�¥���������ɣ�����: " << socket3.getRemoteAddress() << std::endl;
		// message = "connected";
		// socket3.send(message.c_str(), message.size() + 1); // �����2�������ӳɹ���Ϣ

		//message = "GameBegin";
		//socket1.send(message.c_str(), message.size() + 1); // �����1������Ϸ��ʼ��Ϣ
		//socket2.send(message.c_str(), message.size() + 1); // �����2������Ϸ��ʼ��Ϣ
		//socket3.send(message.c_str(), message.size() + 1); // �����2������Ϸ��ʼ��Ϣ

	}


	IniData();
}
void Game::IniData()
{
	gameState = stSTART;
	//��ʼ�����ƶ�
	int ID;
	srand(time(0));
	for (int i = 0; i < CARDS_AMOUNT; i++)
	{
		// ����1��25֮��������
		int randomNum = std::rand() % 100;
		if (randomNum < 80) {
			// ����18��25֮��������
			ID = std::rand() % 8 + 18;
		}
		else {
			// ����0��17֮��������
			ID = std::rand() % 17 + 1;
		}
		pai tempCard(ID);
		tempCard.cardID = ID;
		tempCard.IniHaveObj();
		drawPiles.emplace(tempCard);
	}
}

void Game::packData()
{
	if (is_first_send)
	{
		//���std::queue<pai> drawPiles
		int drawPiles_size = drawPiles.size();
		Game_packet << drawPiles_size;
		for (int i = 0; i < drawPiles_size; i++)  //���ó��ƶѵľ��������
		{
			pai temp_card = drawPiles.front();
			temp_card.packData(Game_packet);  //���ÿһ�ų��ƶ������
			drawPiles.push(temp_card);
			drawPiles.pop();
		}
		is_first_send = false;
	}

	int gameState_int = static_cast<int>(gameState);
	Game_packet << gameState_int;
	//���std::vector<int> sending_card
	int sending_card_size = received_card.size();
	Game_packet << sending_card_size;
	for (int i = 0; i < sending_card_size; i++)
	{
		Game_packet << received_card[i];
	}

	int sending_card_objs_size = received_card_objs.size();
	Game_packet << sending_card_objs_size;
	for (int i = 0; i < sending_card_objs_size; i++)
	{
		//ö������ת�������ͣ��ٴ����packet��

		Game_packet << (received_card_objs[i]);
	}
	Game_packet << hp1 << hp2 << hp3;
}
void Game::unpackData()
{
	int gameState_int;// 
	Game_packet >> gameState_int;

	gameState = static_cast<GAME_STATE>(gameState_int);

	//���std::vector<int> received_card
	int received_card_size;
	Game_packet >> received_card_size;
	received_card.resize(received_card_size);
	for (int i = 0; i < received_card_size; i++)
	{
		Game_packet >> received_card[i];
	}

	//���std::vector<PLAYER_KIND> received_card_objs
	int  received_card_objs_size;
	Game_packet >> received_card_objs_size;
	received_card_objs.resize(received_card_objs_size);
	for (int i = 0; i < received_card_objs_size; i++)
	{
		//������ת����ö���ͣ��ٽ����received_card_objs��
		Game_packet >> received_card_objs[i];
	}
	Game_packet >> hp1 >> hp2 >> hp3;
}

void Game::Receive_packet()
{
	if (!is_game_begin)
	{
		if (socket1.receive(before_receive, sizeof(before_receive), received) == sf::Socket::Done)
		{
			//// ������յ�������
			//if (strcmp(before_receive, "serverHeroSend") == 0)
			//{
			//	sk1P = true;
			//}
			int i_hero_id = before_receive[0] - '0';
			int i_true_player_id = before_receive[1] - '0';

			if (i_true_player_id == 0)
			{
				player1_hero_id = i_hero_id;
			}
			else if (i_true_player_id == 1)
			{
				player2_hero_id = i_hero_id;
			}
			else if (i_true_player_id == 2)
			{
				player3_hero_id = i_hero_id;
			}

			sk1H = true;

		}
		if (socket2.receive(before_receive, sizeof(before_receive), received) == sf::Socket::Done)
		{
			//// ������յ�������
			//if (strcmp(before_receive, "serverHeroSend") == 0)
			//{
			//	sk2P = true;
			//}
			int i_hero_id = before_receive[0] - '0';
			int i_true_player_id = before_receive[1] - '0';

			if (i_true_player_id == 0)
			{
				player1_hero_id = i_hero_id;
			}
			else if (i_true_player_id == 1)
			{
				player2_hero_id = i_hero_id;
			}
			else if (i_true_player_id == 2)
			{
				player3_hero_id = i_hero_id;
			}

			sk2H = true;
		}
		if (socket3.receive(before_receive, sizeof(before_receive), received) == sf::Socket::Done)
		{
			//// ������յ�������
			//if (strcmp(before_receive, "serverHeroSend") == 0)
			//{
			//	sk3P = true;
			//}

			int i_hero_id = before_receive[0] - '0';
			int i_true_player_id = before_receive[1] - '0';

			if (i_true_player_id == 0)
			{
				player1_hero_id = i_hero_id;
			}
			else if (i_true_player_id == 1)
			{
				player2_hero_id = i_hero_id;
			}
			else if (i_true_player_id == 2)
			{
				player3_hero_id = i_hero_id;
			}

			sk3H = true;
		}


	}
	else
	{
		if (socket1.receive(before_receive, sizeof(before_receive), received) == sf::Socket::Done)
		{
			// ������յ�������
			if (strcmp(before_receive, "serverSend") == 0)
			{
				sk1S = true;
			}
			else if (strcmp(before_receive, "serverReceive") == 0)
			{
				sk1R = true;
			}
			else if (strcmp(before_receive, "restart") == 0)
			{
				sk1Re = true;
			}
		}
		else if (socket2.receive(before_receive, sizeof(before_receive), received) == sf::Socket::Done)
		{
			// ������յ�������
			if (strcmp(before_receive, "serverSend") == 0)
			{
				sk2S = true;
			}
			else if (strcmp(before_receive, "serverReceive") == 0)
			{
				sk2R = true;
			}
			else if (strcmp(before_receive, "restart") == 0)
			{
				sk2Re = true;
			}

		}
		else if (socket3.receive(before_receive, sizeof(before_receive), received) == sf::Socket::Done)
		{
			// ������յ�������
			if (strcmp(before_receive, "serverSend") == 0)
			{
				sk3S = true;
			}
			else if (strcmp(before_receive, "serverReceive") == 0)
			{
				sk3R = true;
			}
			else if (strcmp(before_receive, "restart") == 0)
			{
				sk3Re = true;
			}

		}

		//ÿ��ֻ�ܴ���һ����ҵ����ݽ��ջ���
		Game_packet.clear();  //���Game_packet�����ݣ��ٽ��н���
		if (sk1R)
		{
			// ������յ�������
			std::string message = "clientSend";
			socket1.send(message.c_str(), message.size() + 1); // ����ͻ���1��������

			socket1.setBlocking(true); // ��socket1���ó�����ģʽ����֤������������
			socket1.receive(Game_packet);
			socket1.setBlocking(false); // ��socket1��ԭ�ط�����ģʽ

			sk1R = false;

			message = "clientReceive";
			socket2.send(message.c_str(), message.size() + 1); // ��ͻ���2���ͽ�������Ԥ֪
			socket3.send(message.c_str(), message.size() + 1); // ��ͻ���3���ͽ�������Ԥ֪

			unpackData();  //������յ���Game_packet
		}
		else if (sk2R)
		{
			// ������յ�������
			std::string message = "clientSend";
			socket2.send(message.c_str(), message.size() + 1); // ����ͻ���2��������

			socket2.setBlocking(true); // ��socket2���ó�����ģʽ����֤������������
			socket2.receive(Game_packet);
			socket2.setBlocking(false); // ��socket2��ԭ�ط�����ģʽ

			sk2R = false;

			message = "clientReceive";
			socket1.send(message.c_str(), message.size() + 1); // ��ͻ���2���ͽ�������Ԥ֪
			socket3.send(message.c_str(), message.size() + 1); // ��ͻ���3���ͽ�������Ԥ֪

			unpackData();  //������յ���Game_packet
		}
		else if (sk3R)
		{
			// ������յ�������
			std::string message = "clientSend";
			socket3.send(message.c_str(), message.size() + 1); // ����ͻ���3��������

			socket3.setBlocking(true); // ��socket2���ó�����ģʽ����֤������������
			socket3.receive(Game_packet);
			socket3.setBlocking(false); // ��socket2��ԭ�ط�����ģʽ

			sk3R = false;

			message = "clientReceive";
			socket1.send(message.c_str(), message.size() + 1); // ��ͻ���1���ͽ�������Ԥ֪
			socket2.send(message.c_str(), message.size() + 1); // ��ͻ���2���ͽ�������Ԥ֪

			unpackData();  //������յ���Game_packet
		}
	}
}
void Game::Send_packet()
{
	if (is_first_send)
	{
		if (is_first_round)
		{
			is_first_round = false;
			Game_packet.clear();
			packData();  //������ݵ�Game_packet
			std::string message = "clientFirstReceive";
			socket1.send(message.c_str(), message.size() + 1); // ��ͻ���1���ͽ�������Ԥ֪
			socket2.send(message.c_str(), message.size() + 1); // ��ͻ���2���ͽ�������Ԥ֪
			socket3.send(message.c_str(), message.size() + 1); // ��ͻ���3���ͽ�������Ԥ֪

			int cnt = 0;
			bool socket1_ready = false;
			bool socket2_ready = false;
			bool socket3_ready = false;
			while (1)
			{
				if (socket1.receive(before_send, sizeof(before_send), received) == sf::Socket::Done)
				{
					socket1_ready = true;
				}
				if (socket2.receive(before_send, sizeof(before_send), received) == sf::Socket::Done)
				{
					socket2_ready = true;
				}
				if (socket3.receive(before_send, sizeof(before_send), received) == sf::Socket::Done)
				{
					socket3_ready = true;
				}
				if (cnt >= 100000)
				{
					std::cout << "is_first_send ���ճ�ʱ" << std::endl;
					std::cout << socket1_ready << " " << socket2_ready << " " << socket3_ready << std::endl;
					cnt = -10000000;
				}
				if (socket1_ready && socket2_ready && socket3_ready)
				{
					break;
				}
				cnt++;
			}
			sf::Packet temp_Game_packet1 = Game_packet;
			sf::Packet temp_Game_packet2 = Game_packet;
			sf::Packet temp_Game_packet3 = Game_packet;
			std::cout << playerTrueId[0] << playerTrueId[1] << playerTrueId[2] << std::endl;
			temp_Game_packet1 << playerTrueId[0];
			temp_Game_packet2 << playerTrueId[1];
			temp_Game_packet3 << playerTrueId[2];

			socket1.setBlocking(true);  // ����ǰ��socket1�������ģʽ
			socket2.setBlocking(true);  // ����ǰ��socket2�������ģʽ
			socket3.setBlocking(true);  // ����ǰ��socket3�������ģʽ
			socket1.send(temp_Game_packet1);
			socket2.send(temp_Game_packet2);
			socket3.send(temp_Game_packet3);
			socket1.setBlocking(false);  // ���ͺ��socket1��ɷ�����ģʽ
			socket2.setBlocking(false);  // ���ͺ��socket2��ɷ�����ģʽ
			socket3.setBlocking(false);  // ���ͺ��socket3��ɷ�����ģʽ
		}
		else
		{
			Game_packet.clear();
			packData();  //������ݵ�Game_packet
			int cnt = 0;
			bool socket1_ready = false;
			bool socket2_ready = false;
			bool socket3_ready = false;

			if (is_socket1_local_restart)
			{
				socket1_ready = true;
			}
			if (is_socket2_local_restart)
			{
				socket2_ready = true;
			}
			if (is_socket3_local_restart)
			{
				socket3_ready = true;
			}

			while (1)
			{
				if (socket1.receive(before_send, sizeof(before_send), received) == sf::Socket::Done)
				{
					socket1_ready = true;
				}
				if (socket2.receive(before_send, sizeof(before_send), received) == sf::Socket::Done)
				{
					socket2_ready = true;
				}
				if (socket3.receive(before_send, sizeof(before_send), received) == sf::Socket::Done)
				{
					socket3_ready = true;
				}
				if (cnt >= 100000)
				{
					std::cout << "is_first_send ���ճ�ʱ" << std::endl;
					std::cout << socket1_ready << " " << socket2_ready << " " << socket3_ready << std::endl;
					cnt = -10000000;
				}
				if (socket1_ready && socket2_ready && socket3_ready)
				{
					break;
				}
				cnt++;
			}
			sf::Packet temp_Game_packet1 = Game_packet;
			sf::Packet temp_Game_packet2 = Game_packet;
			sf::Packet temp_Game_packet3 = Game_packet;
			std::cout << playerTrueId[0] << playerTrueId[1] << playerTrueId[2] << std::endl;
			temp_Game_packet1 << playerTrueId[0];
			temp_Game_packet2 << playerTrueId[1];
			temp_Game_packet3 << playerTrueId[2];

			socket1.setBlocking(true);  // ����ǰ��socket1�������ģʽ
			socket2.setBlocking(true);  // ����ǰ��socket2�������ģʽ
			socket3.setBlocking(true);  // ����ǰ��socket3�������ģʽ
			socket1.send(temp_Game_packet1);
			socket2.send(temp_Game_packet2);
			socket3.send(temp_Game_packet3);
			socket1.setBlocking(false);  // ���ͺ��socket1��ɷ�����ģʽ
			socket2.setBlocking(false);  // ���ͺ��socket2��ɷ�����ģʽ
			socket3.setBlocking(false);  // ���ͺ��socket3��ɷ�����ģʽ

			is_socket1_local_restart = false;
			is_socket2_local_restart = false;
			is_socket3_local_restart = false;
		}
	}


	if (!is_game_begin)
	{
		if (sk1H && sk2H && sk3H)
		{
			// ������յ�������
			char message_c[3];
			message_c[0] = player1_hero_id + '0';
			message_c[1] = player2_hero_id + '0';
			message_c[2] = player3_hero_id + '0';
			std::string message(message_c, 0, 3);

			socket1.send(message.c_str(), message.size() + 1); // ��ͻ���1��������
			socket2.send(message.c_str(), message.size() + 1); // ��ͻ���2��������
			socket3.send(message.c_str(), message.size() + 1); // ��ͻ���3��������

			is_game_begin = true;
		}

	}

	if (sk2S && sk3S) // socket1���յ����ݷ��͸�socket2��socket3
	{
		Game_packet.clear();
		packData();  //������ݵ�Game_packet


		socket2.setBlocking(true);  // ����ǰ��socket2�������ģʽ
		socket2.send(Game_packet);
		socket2.setBlocking(false);  // ���ͺ��socket2��ɷ�����ģʽ

		socket3.setBlocking(true);  // ����ǰ��socket3�������ģʽ
		socket3.send(Game_packet);
		socket3.setBlocking(false);  // ���ͺ��socket3��ɷ�����ģʽ

		sk2S = false;
		sk3S = false;

	}
	if (sk1S && sk3S) // socket2���յ����ݷ��͸�socket1��socket3
	{
		Game_packet.clear();
		packData();  //������ݵ�Game_packet

		socket1.setBlocking(true);  // ����ǰ��socket1�������ģʽ
		socket1.send(Game_packet);
		socket1.setBlocking(false);  // ���ͺ��socket1��ɷ�����ģʽ

		socket3.setBlocking(true);  // ����ǰ��socket3�������ģʽ
		socket3.send(Game_packet);
		socket3.setBlocking(false);  // ���ͺ��socket3��ɷ�����ģʽ

		sk1S = false;
		sk3S = false;
	}
	if (sk1S && sk2S) // socket3���յ����ݷ��͸�socket1��socket2
	{
		Game_packet.clear();
		packData();  //������ݵ�Game_packet

		socket1.setBlocking(true);  // ����ǰ��socket1�������ģʽ
		socket1.send(Game_packet);
		socket1.setBlocking(false);  // ���ͺ��socket1��ɷ�����ģʽ

		socket2.setBlocking(true);  // ����ǰ��socket2�������ģʽ
		socket2.send(Game_packet);
		socket2.setBlocking(false);  // ���ͺ��socket2��ɷ�����ģʽ

		sk1S = false;
		sk2S = false;
	}


	if (sk1Re)
	{
		std::string message = "restart";
		socket2.send(message.c_str(), message.size() + 1); // ��ͻ���2�������¿�ʼָ��
		socket3.send(message.c_str(), message.size() + 1); // ��ͻ���3�������¿�ʼָ��
		is_socket1_local_restart = true;

		sk1Re = false;
		is_gameRestart = true;
	}
	else if (sk2Re)
	{
		std::string message = "restart";
		socket1.send(message.c_str(), message.size() + 1); // ��ͻ���1�������¿�ʼָ��
		socket3.send(message.c_str(), message.size() + 1); // ��ͻ���3�������¿�ʼָ��
		is_socket2_local_restart = true;

		sk2Re = false;
		is_gameRestart = true;
	}
	else if (sk3Re)
	{
		std::string message = "restart";
		socket1.send(message.c_str(), message.size() + 1); // ��ͻ���1�������¿�ʼָ��
		socket2.send(message.c_str(), message.size() + 1); // ��ͻ���2�������¿�ʼָ��
		is_socket3_local_restart = true;

		sk3Re = false;
		is_gameRestart = true;
	}
}

void Game::Run()
{
	int cout_cnt = 0;

	do {
		Initial();
		Send_packet();
		while (!is_gameRestart)
		{
			if (cout_cnt++ > 3000000)
			{
				std::cout << player1_hero_id << " " << player2_hero_id << " " << player3_hero_id << std::endl;
				cout_cnt = 0;
			}

			Receive_packet();

			Send_packet();


		}
	} while (1);
}


