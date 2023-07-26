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

	sk1H = false; // 玩家英雄初始化响应
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
		int num = rand() % 3; // 返回0, 1, 2随机数
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
		listener.listen(55001); //设置监听的端口
		std::cout << "等待玩家加入: " << std::endl;
		listener.accept(socket1);  //等待客户端连接，如果没有连接则将一直等待
		std::cout << "第一名玩家连接成功，等待其余两名玩家加入: " << socket1.getRemoteAddress() << std::endl;
		//std::string message = "connected";
		//socket1.send(message.c_str(), message.size() + 1); // 向玩家1发送连接成功消息

		listener.accept(socket2);  //等待客户端连接，如果没有连接则将一直等待
		std::cout << "第二名玩家连接成功，等待其余一名玩家加入: " << socket2.getRemoteAddress() << std::endl;
		// message = "connected";
		// socket2.send(message.c_str(), message.size() + 1); // 向玩家2发送连接成功消息

		listener.accept(socket3);  //等待客户端连接，如果没有连接则将一直等待
		std::cout << "第三名玩家连接成功，享受斗楼主的魅力吧！！！: " << socket3.getRemoteAddress() << std::endl;
		// message = "connected";
		// socket3.send(message.c_str(), message.size() + 1); // 向玩家2发送连接成功消息

		//message = "GameBegin";
		//socket1.send(message.c_str(), message.size() + 1); // 向玩家1发送游戏开始消息
		//socket2.send(message.c_str(), message.size() + 1); // 向玩家2发送游戏开始消息
		//socket3.send(message.c_str(), message.size() + 1); // 向玩家2发送游戏开始消息

	}


	IniData();
}
void Game::IniData()
{
	gameState = stSTART;
	//初始化抽牌堆
	int ID;
	srand(time(0));
	for (int i = 0; i < CARDS_AMOUNT; i++)
	{
		// 生成1到25之间的随机数
		int randomNum = std::rand() % 100;
		if (randomNum < 80) {
			// 生成18到25之间的随机数
			ID = std::rand() % 8 + 18;
		}
		else {
			// 生成0到17之间的随机数
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
		//打包std::queue<pai> drawPiles
		int drawPiles_size = drawPiles.size();
		Game_packet << drawPiles_size;
		for (int i = 0; i < drawPiles_size; i++)  //设置抽牌堆的精灵的纹理
		{
			pai temp_card = drawPiles.front();
			temp_card.packData(Game_packet);  //打包每一张抽牌堆里的牌
			drawPiles.push(temp_card);
			drawPiles.pop();
		}
		is_first_send = false;
	}

	int gameState_int = static_cast<int>(gameState);
	Game_packet << gameState_int;
	//打包std::vector<int> sending_card
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
		//枚举型先转换成整型，再打包到packet中

		Game_packet << (received_card_objs[i]);
	}
	Game_packet << hp1 << hp2 << hp3;
}
void Game::unpackData()
{
	int gameState_int;// 
	Game_packet >> gameState_int;

	gameState = static_cast<GAME_STATE>(gameState_int);

	//解包std::vector<int> received_card
	int received_card_size;
	Game_packet >> received_card_size;
	received_card.resize(received_card_size);
	for (int i = 0; i < received_card_size; i++)
	{
		Game_packet >> received_card[i];
	}

	//解包std::vector<PLAYER_KIND> received_card_objs
	int  received_card_objs_size;
	Game_packet >> received_card_objs_size;
	received_card_objs.resize(received_card_objs_size);
	for (int i = 0; i < received_card_objs_size; i++)
	{
		//整型先转换成枚举型，再解包到received_card_objs中
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
			//// 如果接收到了数据
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
			//// 如果接收到了数据
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
			//// 如果接收到了数据
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
			// 如果接收到了数据
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
			// 如果接收到了数据
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
			// 如果接收到了数据
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

		//每次只能处理一名玩家的数据接收或发送
		Game_packet.clear();  //清空Game_packet的内容，再进行接收
		if (sk1R)
		{
			// 如果接收到了数据
			std::string message = "clientSend";
			socket1.send(message.c_str(), message.size() + 1); // 请求客户端1发送数据

			socket1.setBlocking(true); // 把socket1设置成阻塞模式，保证数据完整接收
			socket1.receive(Game_packet);
			socket1.setBlocking(false); // 把socket1还原回非阻塞模式

			sk1R = false;

			message = "clientReceive";
			socket2.send(message.c_str(), message.size() + 1); // 向客户端2发送接收数据预知
			socket3.send(message.c_str(), message.size() + 1); // 向客户端3发送接收数据预知

			unpackData();  //解包接收到的Game_packet
		}
		else if (sk2R)
		{
			// 如果接收到了数据
			std::string message = "clientSend";
			socket2.send(message.c_str(), message.size() + 1); // 请求客户端2发送数据

			socket2.setBlocking(true); // 把socket2设置成阻塞模式，保证数据完整接收
			socket2.receive(Game_packet);
			socket2.setBlocking(false); // 把socket2还原回非阻塞模式

			sk2R = false;

			message = "clientReceive";
			socket1.send(message.c_str(), message.size() + 1); // 向客户端2发送接收数据预知
			socket3.send(message.c_str(), message.size() + 1); // 向客户端3发送接收数据预知

			unpackData();  //解包接收到的Game_packet
		}
		else if (sk3R)
		{
			// 如果接收到了数据
			std::string message = "clientSend";
			socket3.send(message.c_str(), message.size() + 1); // 请求客户端3发送数据

			socket3.setBlocking(true); // 把socket2设置成阻塞模式，保证数据完整接收
			socket3.receive(Game_packet);
			socket3.setBlocking(false); // 把socket2还原回非阻塞模式

			sk3R = false;

			message = "clientReceive";
			socket1.send(message.c_str(), message.size() + 1); // 向客户端1发送接收数据预知
			socket2.send(message.c_str(), message.size() + 1); // 向客户端2发送接收数据预知

			unpackData();  //解包接收到的Game_packet
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
			packData();  //打包数据到Game_packet
			std::string message = "clientFirstReceive";
			socket1.send(message.c_str(), message.size() + 1); // 向客户端1发送接收数据预知
			socket2.send(message.c_str(), message.size() + 1); // 向客户端2发送接收数据预知
			socket3.send(message.c_str(), message.size() + 1); // 向客户端3发送接收数据预知

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
					std::cout << "is_first_send 接收超时" << std::endl;
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

			socket1.setBlocking(true);  // 发送前把socket1设成阻塞模式
			socket2.setBlocking(true);  // 发送前把socket2设成阻塞模式
			socket3.setBlocking(true);  // 发送前把socket3设成阻塞模式
			socket1.send(temp_Game_packet1);
			socket2.send(temp_Game_packet2);
			socket3.send(temp_Game_packet3);
			socket1.setBlocking(false);  // 发送后把socket1设成非阻塞模式
			socket2.setBlocking(false);  // 发送后把socket2设成非阻塞模式
			socket3.setBlocking(false);  // 发送后把socket3设成非阻塞模式
		}
		else
		{
			Game_packet.clear();
			packData();  //打包数据到Game_packet
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
					std::cout << "is_first_send 接收超时" << std::endl;
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

			socket1.setBlocking(true);  // 发送前把socket1设成阻塞模式
			socket2.setBlocking(true);  // 发送前把socket2设成阻塞模式
			socket3.setBlocking(true);  // 发送前把socket3设成阻塞模式
			socket1.send(temp_Game_packet1);
			socket2.send(temp_Game_packet2);
			socket3.send(temp_Game_packet3);
			socket1.setBlocking(false);  // 发送后把socket1设成非阻塞模式
			socket2.setBlocking(false);  // 发送后把socket2设成非阻塞模式
			socket3.setBlocking(false);  // 发送后把socket3设成非阻塞模式

			is_socket1_local_restart = false;
			is_socket2_local_restart = false;
			is_socket3_local_restart = false;
		}
	}


	if (!is_game_begin)
	{
		if (sk1H && sk2H && sk3H)
		{
			// 如果接收到了数据
			char message_c[3];
			message_c[0] = player1_hero_id + '0';
			message_c[1] = player2_hero_id + '0';
			message_c[2] = player3_hero_id + '0';
			std::string message(message_c, 0, 3);

			socket1.send(message.c_str(), message.size() + 1); // 向客户端1发送数据
			socket2.send(message.c_str(), message.size() + 1); // 向客户端2发送数据
			socket3.send(message.c_str(), message.size() + 1); // 向客户端3发送数据

			is_game_begin = true;
		}

	}

	if (sk2S && sk3S) // socket1接收到数据发送给socket2、socket3
	{
		Game_packet.clear();
		packData();  //打包数据到Game_packet


		socket2.setBlocking(true);  // 发送前把socket2设成阻塞模式
		socket2.send(Game_packet);
		socket2.setBlocking(false);  // 发送后把socket2设成非阻塞模式

		socket3.setBlocking(true);  // 发送前把socket3设成阻塞模式
		socket3.send(Game_packet);
		socket3.setBlocking(false);  // 发送后把socket3设成非阻塞模式

		sk2S = false;
		sk3S = false;

	}
	if (sk1S && sk3S) // socket2接收到数据发送给socket1、socket3
	{
		Game_packet.clear();
		packData();  //打包数据到Game_packet

		socket1.setBlocking(true);  // 发送前把socket1设成阻塞模式
		socket1.send(Game_packet);
		socket1.setBlocking(false);  // 发送后把socket1设成非阻塞模式

		socket3.setBlocking(true);  // 发送前把socket3设成阻塞模式
		socket3.send(Game_packet);
		socket3.setBlocking(false);  // 发送后把socket3设成非阻塞模式

		sk1S = false;
		sk3S = false;
	}
	if (sk1S && sk2S) // socket3接收到数据发送给socket1、socket2
	{
		Game_packet.clear();
		packData();  //打包数据到Game_packet

		socket1.setBlocking(true);  // 发送前把socket1设成阻塞模式
		socket1.send(Game_packet);
		socket1.setBlocking(false);  // 发送后把socket1设成非阻塞模式

		socket2.setBlocking(true);  // 发送前把socket2设成阻塞模式
		socket2.send(Game_packet);
		socket2.setBlocking(false);  // 发送后把socket2设成非阻塞模式

		sk1S = false;
		sk2S = false;
	}


	if (sk1Re)
	{
		std::string message = "restart";
		socket2.send(message.c_str(), message.size() + 1); // 向客户端2发送重新开始指令
		socket3.send(message.c_str(), message.size() + 1); // 向客户端3发送重新开始指令
		is_socket1_local_restart = true;

		sk1Re = false;
		is_gameRestart = true;
	}
	else if (sk2Re)
	{
		std::string message = "restart";
		socket1.send(message.c_str(), message.size() + 1); // 向客户端1发送重新开始指令
		socket3.send(message.c_str(), message.size() + 1); // 向客户端3发送重新开始指令
		is_socket2_local_restart = true;

		sk2Re = false;
		is_gameRestart = true;
	}
	else if (sk3Re)
	{
		std::string message = "restart";
		socket1.send(message.c_str(), message.size() + 1); // 向客户端1发送重新开始指令
		socket2.send(message.c_str(), message.size() + 1); // 向客户端2发送重新开始指令
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


