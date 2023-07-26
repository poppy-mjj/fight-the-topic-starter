#include "Game.h"

Game::Game()
{
	Window_width = BG_WIDTH;
	Window_height = BG_HEIGHT;
	window.create(sf::VideoMode(Window_width, Window_height), L"斗楼主", sf::Style::Fullscreen);

	socket.connect("192.168.0.101", 55001);
	socket.setBlocking(false);
}

Game::~Game() {
}
void Game::Initial() {
	if (is_first_round)
	{
		LoadMediaData();
		is_first_round = false;
	}

	soundMainMusic.stop();

	//listener.listen(55001);
	window.setFramerateLimit(120);//设置游戏帧频

	gameOver = false;
	gameRestart = false;
	gameQuit = false;
	is_game_start = false;

	is_send = false;
	is_game_start = false;
	is_selected = false;
	is_first_receive = true;
	received = 0;

	lastState = stSTART;
	gameState = stSTART;
	player a_player;
	players.emplace_back(a_player);
	players.emplace_back(a_player);
	players.emplace_back(a_player);

	responseState = no_card;

	players[0].playerKind = LOUZHU;
	players[1].playerKind = LEFT_BAYOU;
	players[2].playerKind = RIGHT_BAYOU;

	player1_hero_id = 9;
	player2_hero_id = 9;
	player3_hero_id = 9;


	roundOverRect = sf::FloatRect(100, 500, 131, 56);
	StartRect = sf::FloatRect(235, 540, 1022, 140);
	GuideRect = sf::FloatRect(235 + 1022, 540, 511, 140);
	ReStartRect = sf::FloatRect(800, 500, 306, 124);


	Num2GameState[0] = stPLAYER1;
	Num2GameState[1] = stPLAYER2;
	Num2GameState[2] = stPLAYER3;
	Num2GameState[3] = stRESPONSE1;
	Num2GameState[4] = stRESPONSE2;
	Num2GameState[5] = stRESPONSE3;
	Num2GameState[6] = stGET1;
	Num2GameState[7] = stGET2;
	Num2GameState[8] = stGET3;


	GameState2Num[stPLAYER1] = 0;
	GameState2Num[stPLAYER2] = 1;
	GameState2Num[stPLAYER3] = 2;
	GameState2Num[stRESPONSE1] = 3;
	GameState2Num[stRESPONSE2] = 4;
	GameState2Num[stRESPONSE3] = 5;
	GameState2Num[stGET1] = 6;
	GameState2Num[stGET2] = 7;
	GameState2Num[stGET3] = 8;
	GameState2Num[stSTART] = -1;
	GameState2Num[stCHOOCE] = -2;
	GameState2Num[stGUIDE] = -3;


	Player2GameState_Response[LOUZHU] = stRESPONSE1;
	Player2GameState_Response[LEFT_BAYOU] = stRESPONSE2;
	Player2GameState_Response[RIGHT_BAYOU] = stRESPONSE3;

	Player2Num[LOUZHU] = 0;
	Player2Num[LEFT_BAYOU] = 1;
	Player2Num[RIGHT_BAYOU] = 2;


	CardTypeCal[0] = 0;
	CardTypeCal[1] = 1;
	CardTypeCal[2] = 1;
	CardTypeCal[3] = 2;


	CardTypeCal[4] = 2;
	CardTypeCal[5] = 3;
	CardTypeCal[6] = 3;
	CardTypeCal[8] = 3;
	CardTypeCal[10] = 3;


	CardTypeCal[7] = 2;
	CardTypeCal[9] = 2;
	CardTypeCal[11] = 2;
	CardTypeCal[12] = 2;

	CardTypeCal[13] = 4;
	CardTypeCal[14] = 4;
	CardTypeCal[15] = 4;
	CardTypeCal[16] = 4;

	for (int i = 17; i <= 25; i++)
	{
		CardTypeCal[i] = 2;
	}

	if (is_local_restart)
	{
		is_local_restart = false;
		socket.setBlocking(true);
		socket.receive(Game_packet);
		socket.setBlocking(false);

		unpackData();
		IniData();
	}
	if (skRe)
	{
		std::string message = "resucceed";
		socket.send(message.c_str(), message.size() + 1); // 向服务端发送重启成功通知
		skRe = false;

		socket.setBlocking(true);  // 发送前把socket设成阻塞模式
		socket.receive(Game_packet);
		socket.setBlocking(false);  // 发送后把socket设成非阻塞模式

		unpackData();
		IniData();
	}
}
void Game::LoadMediaData()
{
	std::stringstream ss;

	ss << "data/images/Initialinterface.png";
	if (!tStart.loadFromFile(ss.str()))
	{
		std::cout << "Initialinterface.png没有找到" << std::endl;
	}
	sStart.setTexture(tStart);
	ss.str("");

	ss << "data/images/restart.png";
	if (!tReStart.loadFromFile(ss.str()))
	{
		std::cout << "restart.png没有找到" << std::endl;
	}
	sReStart.setTexture(tReStart);
	ss.str("");


	ss << "data/images/white.png";
	if (!tStartGround.loadFromFile(ss.str()))
	{
		std::cout << "white.png没有找到" << std::endl;
	}
	sStartGround.setTexture(tStartGround);
	ss.str("");

	ss << "data/images/pofangle.png";
	if (!tPoFang.loadFromFile(ss.str()))
	{
		std::cout << "white.png没有找到" << std::endl;
	}
	sPoFang.setTexture(tPoFang);
	ss.str("");

	ss << "data/images/Group1.png";
	if (!tStartButton.loadFromFile(ss.str()))
	{
		std::cout << "Group1.png没有找到" << std::endl;
	}
	sStartButton.setTexture(tStartButton);
	ss.str("");

	ss << "data/images/Group2.png";
	if (!tGuide.loadFromFile(ss.str()))
	{
		std::cout << "Group2.png没有找到" << std::endl;
	}
	sGuide.setTexture(tGuide);
	ss.str("");

	ss.str("");
	ss << "data/images/background2.png";
	if (!tBackground.loadFromFile(ss.str()))
	{
		std::cout << "BK image 没有找到" << std::endl;
	}
	sBackground.setTexture(tBackground);


	ss.str("");
	ss << "data/images/roundBackground.jpg";
	if (!tRoundBackGround.loadFromFile(ss.str()))
	{
		std::cout << "roundBackground.jpg没有找到" << std::endl;
	}
	ss.str("");

	ss.str("");
	ss << "data/images/choose.png";
	if (!tChooseBackground.loadFromFile(ss.str()))
	{
		std::cout << "choose.png没有找到" << std::endl;
	}
	ss.str("");
	sChooseBackground.setTexture(tChooseBackground);



	ss << "data/images/roundOver.png";
	if (!tRoundOver.loadFromFile(ss.str()))
	{
		std::cout << "roundOver.png没有找到" << std::endl;
	}
	sRoundOver.setTexture(tRoundOver);

	//初始化卡牌纹理
	for (int i = 0; i <= (CARDID_END - CARDID_BEGIN); i++)
	{
		ss.str("");
		ss << "data/images/LastCard/card" << i + CARDID_BEGIN << ".png";
		if (!t_tCard[i].loadFromFile(ss.str()))
		{
			std::cout << "data/images/LastCard/card" << i + CARDID_BEGIN << ".png没有找到" << std::endl;
		}
		ss.str("");
		ss << "data/images/LastCard_bigger/card" << i + CARDID_BEGIN << ".png";
		if (!t_tCardBigger[i].loadFromFile(ss.str()))
		{
			std::cout << "data/images/LastCard_bigger/card" << i + CARDID_BEGIN << ".png没有找到" << std::endl;
		}
	}

	// 初始化英雄纹理并绑定
	for (int i = 0; i < HEROS_AMOUNT; i++)
	{
		ss.str("");
		ss << "data/images/player" << i << ".png";
		if (!t_tHero[i].loadFromFile(ss.str()))
		{
			std::cout << "data/images/player" << i << ".png没有找到" << std::endl;
		}
		s_sHero[i].setTexture(t_tHero[i]);
		s_sHero[i].setScale(0.7, 0.7);  // 缩小英雄卡牌
	}
	// 初始化未选定英雄纹理先不用绑定
	ss.str("");
	ss << "data/images/player9.png";
	if (!tUnSelectedHero.loadFromFile(ss.str()))
	{
		std::cout << "player9.png没有找到" << std::endl;
	}

	ss.str("");
	ss << "data/images/pwh_bg.png";
	if (!tPWH_BG.loadFromFile(ss.str()))
	{
		std::cout << "pwh_bg.png没有找到" << std::endl;
	}


	ss.str(""); // 加载pwh英雄
	ss << "data/images/pwh.png";
	if (!pwh_texture.loadFromFile(ss.str()))
	{
		std::cout << "pwh.png" << std::endl;
	}

	pwh_animation.setTexture(&pwh_texture);
	pwh_animation.addFramesLine(48, 1, 0);
	AnimatedSprite temp_sprite(&pwh_animation, AnimatedSprite::Playing, sf::seconds(0.045));
	pwh_Hero = temp_sprite;
	pwh_Hero_copy1 = temp_sprite;
	pwh_Hero_copy2 = temp_sprite;
	sf::RectangleShape temp_goblin_Hero_selected_cover(GOBLIN_COVER_RECT);
	pwh_Hero_selected_cover = temp_goblin_Hero_selected_cover;

	font.loadFromFile("data/Fonts/simsun.ttc");
	texting.setFont(font);
	//////////////////////////////////////////
	/////////////下面加载游戏音效/////////////
	//////////////////////////////////////////
	//ss.str(""); // 加载菜单界面的背景音乐
	//ss << "data/Audios/menu_bg.ogg";
	//if (!menu_bgMusic.openFromFile(ss.str()))
	//{
	//	std::cout << "menu_bg.ogg没有找到" << std::endl;
	//}

	ss.str(""); // 加载选英雄界面的背景音乐
	ss << "data/Audios/choose_bg.ogg";
	if (!choose_bgMusic.openFromFile(ss.str()))
	{
		std::cout << "choose_bg.ogg没有找到" << std::endl;
	}

	ss.str(""); // 加载选蔡徐坤语音
	ss << "data/Audios/蔡徐坤.ogg";
	if (!sbCaiXukun.loadFromFile(ss.str()))
	{
		std::cout << "蔡徐坤.ogg没有找到" << std::endl;
	}
	soundCaiXukun.setBuffer(sbCaiXukun);
	soundCaiXukun.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载选丁真语音
	ss << "data/Audios/丁真.ogg";
	if (!sbDingZhen.loadFromFile(ss.str()))
	{
		std::cout << "丁真.ogg没有找到" << std::endl;
	}
	soundDingZhen.setBuffer(sbDingZhen);
	soundDingZhen.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载选失败的man语音
	ss << "data/Audios/失败的man.ogg";
	if (!sbSpiderMan.loadFromFile(ss.str()))
	{
		std::cout << "失败的man.ogg没有找到" << std::endl;
	}
	soundSpiderMan.setBuffer(sbSpiderMan);
	soundSpiderMan.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载孙笑川语音
	ss << "data/Audios/孙笑川.ogg";
	if (!sbSunXiaochuan.loadFromFile(ss.str()))
	{
		std::cout << "孙笑川.ogg没有找到" << std::endl;
	}
	soundSunXiaochuan.setBuffer(sbSunXiaochuan);
	soundSunXiaochuan.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载吴京语音
	ss << "data/Audios/吴京.ogg";
	if (!sbWujing.loadFromFile(ss.str()))
	{
		std::cout << "吴京.ogg没有找到" << std::endl;
	}
	soundWujing.setBuffer(sbWujing);
	soundWujing.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载香蕉君语音
	ss << "data/Audios/香蕉君.ogg";
	if (!sbXiangjiaojun.loadFromFile(ss.str()))
	{
		std::cout << "香蕉君.ogg没有找到" << std::endl;
	}
	soundXiangjiaojun.setBuffer(sbXiangjiaojun);
	soundXiangjiaojun.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载小鬼语音
	ss << "data/Audios/小鬼.ogg";
	if (!sbXiaogui.loadFromFile(ss.str()))
	{
		std::cout << "小鬼.ogg没有找到" << std::endl;
	}
	soundXiaogui.setBuffer(sbXiaogui);
	soundXiaogui.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载彭文海语音
	ss << "data/Audios/彭文海.ogg";
	if (!sbPWH.loadFromFile(ss.str()))
	{
		std::cout << "彭文海.ogg没有找到" << std::endl;
	}
	soundPWH.setBuffer(sbPWH);
	soundPWH.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载主菜单按钮划过音效
	ss << "data/Audios/按钮划过.ogg";
	if (!sbMenuButtom.loadFromFile(ss.str()))
	{
		std::cout << "按钮划过.ogg没有找到" << std::endl;
	}
	soundMenuButtom.setBuffer(sbMenuButtom);
	soundMenuButtom.setLoop(false); // 禁用循环播放


	ss.str(""); // 加载回合结束
	ss << "data/Audios/roundOver.ogg";
	if (!sbRoundButtom.loadFromFile(ss.str()))
	{
		std::cout << "roundOver.ogg没有找到" << std::endl;
	}
	soundRoundButtom.setBuffer(sbRoundButtom);
	soundRoundButtom.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载回血音效
	ss << "data/Audios/回血.ogg";
	if (!sbHpDown.loadFromFile(ss.str()))
	{
		std::cout << "回血.ogg没有找到" << std::endl;
	}
	soundHpDown.setBuffer(sbHpDown);
	soundHpDown.setLoop(false); // 禁用循环播放


	ss.str(""); // 加载掉血音效
	ss << "data/Audios/掉血.ogg";
	if (!sbHpUp.loadFromFile(ss.str()))
	{
		std::cout << "掉血.ogg没有找到" << std::endl;
	}
	soundHpUp.setBuffer(sbHpUp);
	soundHpUp.setLoop(false); // 禁用循环播放

	ss.str(""); // 加载main音效
	ss << "data/Audios/main.ogg";
	if (!sbMainMusic.loadFromFile(ss.str()))
	{
		std::cout << "main.ogg没有找到" << std::endl;
	}
	soundMainMusic.setBuffer(sbMainMusic);
	soundMainMusic.setLoop(true); // 禁用循环播放

	for (int i = 0; i < 11; i++)
	{
		ss.str("");
		ss << "data/images/新手引导/" << i + 1 << ".png";
		if (!tGuidePage[i].loadFromFile(ss.str()))
		{
			std::cout << i + 1 << ".png没有找到" << std::endl;
		}
		sGudiePage[i].setTexture(tGuidePage[i]);
	}

	menu_bgMusic.play(); // 当所有素材加载完成后，开始播放主菜单背景音乐
}
void Game::IniData() // 接收到服务端数据后，初始化抽牌堆的纹理，玩家抽牌
{
	if (true_playerID == 0)
	{
		players[0].CardArea = LOUZHU_CARD_AREA;
		players[0].CardHomePosition = LOUZHU_CARD_POSTION;
		players[0].RotateCenter = LOUZHU_ROTATE_ORIGIN;
		players[0].CardInfoPos = LOUZHU_CARD_INFO_POS;


		players[0].playerRect = LOCAL_LOUZHU_AREA;
		players[1].playerRect = LOCAL_LEFT_BAYOU_AREA;
		players[2].playerRect = LOCAL_RIGHT_BAYOU_AREA;

	}
	else
	{
		players[0].playerRect = LOUZHU_AREA;



		players[1].CardArea = LEFT_BAYOU_CARD_AREA;
		players[1].CardHomePosition = LEFT_CARD_POSTION;
		players[1].RotateCenter = LEFT_ROTATE_ORIGIN;
		players[1].playerRect = LEFT_BAYOU_AREA;

		players[1].CardInfoPos = L_CARD_INFO_POS;



		players[2].CardArea = RIGHT_BAYOU_CARD_AREA;
		players[2].CardHomePosition = RIGHT_CARD_POSTION;
		players[2].RotateCenter = RIGHT_ROTATE_ORIGIN;
		players[2].playerRect = RIGHT_BAYOU_AREA;

		players[2].CardInfoPos = R_CARD_INFO_POS;
	}


	//初始化抽牌堆
	int ID;
	srand(time(0));

	for (int i = 0; i < drawPiles.size(); i++) {   //myqueue_size 必须是固定值
		ID = drawPiles.front().cardId;
		pai tempCard(ID);
		tempCard.cardId = ID;
		tempCard.IniHaveObj();
		tempCard.IniHavePartner();
		tempCard.LoadCardIMG(t_tCard[ID - CARDID_BEGIN]); //减去CARDID_BEGIN是防止卡牌的ID不是从0开始

		drawPiles.push(tempCard);
		drawPiles.pop();

	}

	//初始化手牌，最初3名玩家的手牌的数量都相同，抢到楼主后再给楼主增加卡牌

	players[0].getCard(&drawPiles, LOUZHU_INITIAL_CARD_AMOUNT);
	players[1].getCard(&drawPiles, BAYOU_INITIAL_CARD_AMOUNT);
	players[2].getCard(&drawPiles, BAYOU_INITIAL_CARD_AMOUNT);
}
void Game::bind_player_textures() // 绑定玩家纹理
{
	if (player1_hero_id >= 0 && player1_hero_id < HEROS_AMOUNT) // 如果玩家已经选择，且英雄角色为非动态英雄
	{
		sPlayer1.setTexture(t_tHero[player1_hero_id]);
	}
	else if (player1_hero_id == 9)
	{
		sPlayer1.setTexture(tUnSelectedHero);
	}
	else if (player1_hero_id == HEROS_AMOUNT)
	{
		sPlayer1.setTexture(tPWH_BG);
		is_selected_pwh = true;
		pwh_is_player1 = true;
	}

	if (player2_hero_id >= 0 && player2_hero_id < HEROS_AMOUNT) // 如果玩家已经选择，且英雄角色为非动态英雄
	{
		sPlayer2.setTexture(t_tHero[player2_hero_id]);
	}
	else if (player2_hero_id == 9)
	{
		sPlayer2.setTexture(tUnSelectedHero);
	}
	else if (player2_hero_id == HEROS_AMOUNT)
	{
		sPlayer2.setTexture(tPWH_BG);
		is_selected_pwh = true;
		pwh_is_player2 = true;
	}

	if (player3_hero_id >= 0 && player3_hero_id < HEROS_AMOUNT) // 如果玩家已经选择，且英雄角色为非动态英雄
	{
		sPlayer3.setTexture(t_tHero[player3_hero_id]);
	}
	else if (player3_hero_id == 9)
	{
		sPlayer3.setTexture(tUnSelectedHero);
	}
	else if (player3_hero_id == HEROS_AMOUNT)
	{
		sPlayer3.setTexture(tPWH_BG);
		is_selected_pwh = true;
		pwh_is_player3 = true;
	}

}

void Game::Input()
{
	sf::Event event;
	window.setKeyRepeatEnabled(false);
	while (window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window.close();
			gameQuit = true;
		}
		if (event.type == sf::Event::EventType::KeyReleased && event.key.code == sf::Keyboard::Escape)
		{
			window.close();
			gameQuit = true;
		}

		if (players[0].playerHp >= LOUZHU_HP || players[1].playerHp >= BAYOU_HP || players[2].playerHp >= BAYOU_HP)
		{
			if (ReStartRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
			{
				sReStart.setColor(sf::Color::Yellow);
			}
			else
			{
				sReStart.setColor(sf::Color::White);
			}
		}

		if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
		{
			if (players[0].playerHp >= LOUZHU_HP || players[1].playerHp >= BAYOU_HP || players[2].playerHp >= BAYOU_HP)
				if (ReStartRect.contains(event.mouseButton.x, event.mouseButton.y))
				{
					gameRestart = true;//测试用
					is_local_restart = true;
					std::string message = "restart";
					socket.send(message.c_str(), message.size() + 1); // 请求服务端发送数据

				}
		}
		if (true_playerID == 0)
			switch (gameState)
			{
			case stGUIDE:
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (page_guide < 10)
						page_guide++;
					else
					{
						page_guide = 0;
						gameState = stSTART;
					}
				}
				break;
			case stSTART:
				if (StartRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					sStartButton.setColor(sf::Color::Yellow);
					if (!is_played_sound)
					{
						soundMenuButtom.play();
						is_played_sound = true;
					}
				}
				else
				{
					sStartButton.setColor(sf::Color::White);
					if (is_played_sound)
					{
						soundMenuButtom.stop();
						is_played_sound = false;
					}
				}
				if (GuideRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					sGuide.setColor(sf::Color::Yellow);
				}
				else
				{
					sGuide.setColor(sf::Color::White);
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (StartRect.contains(event.mouseButton.x, event.mouseButton.y))
					{
						lastState = gameState;

						gameState = stCHOOCE;

						is_played_sound = false;
						menu_bgMusic.stop(); // 停止播放主菜单的背景音乐
						choose_bgMusic.play(); // 播放选将页面的背景音乐
					}
					if (GuideRect.contains(event.mouseButton.x, event.mouseButton.y))
					{
						gameState = stGUIDE;
					}
				}
				break;
			case stCHOOCE:
				for (int i = 0; i < HEROS_AMOUNT; i++)
				{
					if (r_rHero[i].contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
					{
						s_sHero[i].setColor(sf::Color::Yellow);
					}
					else
					{
						s_sHero[i].setColor(sf::Color::White);
					}
				}
				if (r_pwh_Hero.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					pwh_Hero.setColor(sf::Color::Yellow);
					is_hover_pwh = true;
					//goblin_Hero_selected_cover.setFillColor(sf::Color(255, 255, 0, 120));
				}
				else
				{
					pwh_Hero.setColor(sf::Color::White);
					is_hover_pwh = false;
					//goblin_Hero_selected_cover.setFillColor(sf::Color(255, 255, 0, 0));
				}

				for (int i = 0; i < HEROS_AMOUNT; i++)
				{
					if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
					{
						if (r_rHero[i].contains(event.mouseButton.x, event.mouseButton.y))
						{
							lastState = gameState;
							is_game_start = true;
							is_send = false;
							is_selected = true;
							if (true_playerID == 0)
							{
								player1_hero_id = i;
								hero_id = i;
							}
							else if (true_playerID == 1)
							{
								player2_hero_id = i;
								hero_id = i;
							}
							else if (true_playerID == 2)
							{
								player3_hero_id = i;
								hero_id = i;
							}

							bind_player_textures(); // 绑定玩家纹理

							gameState = stGET2;//测试用
							if (gameState == stGET2)
							{
								draw_piles_getFromTrash();
								players[1].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
								card_full(players[1]);
							}

							startChangeSound();
						}
					}
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (r_pwh_Hero.contains(event.mouseButton.x, event.mouseButton.y))
					{
						lastState = gameState;
						is_game_start = true;
						is_send = false;
						is_selected = true;
						if (true_playerID == 0)
						{
							player1_hero_id = HEROS_AMOUNT;
							hero_id = HEROS_AMOUNT;
						}
						else if (true_playerID == 1)
						{
							player2_hero_id = HEROS_AMOUNT;
							hero_id = HEROS_AMOUNT;
						}
						else if (true_playerID == 2)
						{
							player3_hero_id = HEROS_AMOUNT;
							hero_id = HEROS_AMOUNT;
						}

						bind_player_textures(); // 绑定玩家纹理

						gameState = stGET2;//测试用
						if (gameState == stGET2)
						{
							draw_piles_getFromTrash();
							players[1].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
							card_full(players[1]);
						}
						startChangeSound();
					}
				}
				break;
			case stRESPONSE1:
				if (roundOverRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					sRoundOver.setColor(sf::Color::Green);
				}
				else
				{
					sRoundOver.setColor(sf::Color::White);
				}

				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (roundOverRect.contains(event.mouseButton.x, event.mouseButton.y))
					{
						sRoundOver.setColor(sf::Color(100, 100, 100, 255));
						//sRoundOver.setTexture(tRoundBackGround);
						soundRoundButtom.play();
						gameState = lastState;
						responseState = response_fail;
						sending_card.emplace_back(0);
					}
				}
				if (!players.empty())
					if (players[0].pai_list.size() > 0)
					{
						if (!players[0].isHoverInCard)
						{
							if (players[0].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[0]);
							}
						}
						else
						{
							if (players[0].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[0]);
							}
						}
						if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
						{

							LButtonResponse(sf::Mouse::getPosition(window), players[0]);
						}


					}

				break;
			case stPLAYER2:
			case stPLAYER3:
			case stRESPONSE2:
			case stRESPONSE3:
				sRoundOver.setColor(sf::Color(100, 100, 100, 255));
				if (!players.empty())
					if (players[0].pai_list.size() > 0)
					{
						if (!players[0].isHoverInCard)
						{
							if (players[0].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[0]);
							}
						}
						else
						{
							if (players[0].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[0]);
							}
						}
					}
				break;

			case stPLAYER1:
				if (roundOverRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					sRoundOver.setColor(sf::Color::Green);
				}
				else
				{
					sRoundOver.setColor(sf::Color::White);
				}

				if (clickingState_stack.empty())//类型四的卡牌默认不能直接打出
				{
					for (auto it = players[0].pai_list.begin(); it != players[0].pai_list.end(); it++)
					{
						if (CardTypeCal[it->cardId] == 4)
						{
							it->card_gray = true;
						}
					}
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (!players.empty())
					{
						if (roundOverRect.contains(event.mouseButton.x, event.mouseButton.y))
						{
							sRoundOver.setColor(sf::Color(100, 100, 100, 255));
							soundRoundButtom.play();
							lastState = gameState;
							gameState = Num2GameState[(true_playerID + 1) % 3 + 6];
							draw_piles_getFromTrash();
							if ((true_playerID + 1) % 3 == 0)
							{
								players[(true_playerID + 1) % 3].getCard(&drawPiles, LOUZHU_GET_CARD_AMOUNT);
							}
							else
							{
								players[(true_playerID + 1) % 3].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
							}
							card_full(players[(true_playerID + 1) % 3]);

						}

						//player_click(event, players[0]);
						player_click(event, players[1]);
						player_click(event, players[2]);
					}
				}

				if (!players.empty())
					if (players[0].pai_list.size() > 0)
					{
						if (!players[0].isHoverInCard)
						{
							if (players[0].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[0]);
							}
						}
						else
						{
							if (players[0].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[0]);
							}
						}
						if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
						{
							if (clickingState_stack.empty())
								LButtonDown(sf::Mouse::getPosition(window), players[0]);
							else
							{
								if (clickingState_stack.top() != click_need_obj)
									LButtonDown(sf::Mouse::getPosition(window), players[0]);

							}

						}

					}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
				{
					RButtonDown(players[0]);
				}

				break;

			}
		else
			if (gameState == stSTART)
			{

				if (StartRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					sStartButton.setColor(sf::Color::Yellow);
					if (!is_played_sound)
					{
						soundMenuButtom.play();
						is_played_sound = true;
					}
				}
				else
				{
					sStartButton.setColor(sf::Color::White);
					if (is_played_sound)
					{
						soundMenuButtom.stop();
						is_played_sound = false;
					}
				}
				if (GuideRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					sGuide.setColor(sf::Color::Yellow);
				}
				else
				{
					sGuide.setColor(sf::Color::White);
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (StartRect.contains(event.mouseButton.x, event.mouseButton.y))
					{
						lastState = gameState;
						gameState = stCHOOCE;

						is_played_sound = false;
						menu_bgMusic.stop(); // 停止播放主菜单的背景音乐
						choose_bgMusic.play();  // 播放选将页面背景音乐


						//gameState = stGET2;//测试用
						//draw_piles_getFromTrash();
						//players[1].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
						//card_full(players[1]);
					}
					if (GuideRect.contains(event.mouseButton.x, event.mouseButton.y))
					{
						gameState = stGUIDE;
					}
				}
			}
			else if (gameState == stGUIDE)
			{
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (page_guide < 10)
						page_guide++;
					else
					{
						page_guide = 0;
						gameState = stSTART;
					}
				}
			}
			else if (gameState == stCHOOCE)
			{
				for (int i = 0; i < HEROS_AMOUNT; i++)
				{
					if (r_rHero[i].contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
					{
						s_sHero[i].setColor(sf::Color::Yellow);
					}
					else
					{
						s_sHero[i].setColor(sf::Color::White);
					}
				}
				if (r_pwh_Hero.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{

					pwh_Hero.setColor(sf::Color::Yellow);
					is_hover_pwh = true;
					//goblin_Hero_selected_cover.setFillColor(sf::Color(255, 255, 0, 120));
				}
				else
				{

					pwh_Hero.setColor(sf::Color::White);
					is_hover_pwh = false;
					//goblin_Hero_selected_cover.setFillColor(sf::Color(255, 255, 0, 0));
				}

				for (int i = 0; i < HEROS_AMOUNT; i++)
				{
					if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
					{
						if (r_rHero[i].contains(event.mouseButton.x, event.mouseButton.y))
						{
							lastState = gameState;
							is_game_start = true;
							is_send = false;
							is_selected = true;
							if (true_playerID == 0)
							{
								player1_hero_id = i;
								hero_id = i;
							}
							else if (true_playerID == 1)
							{
								player2_hero_id = i;
								hero_id = i;
							}
							else if (true_playerID == 2)
							{
								player3_hero_id = i;
								hero_id = i;
							}

							bind_player_textures(); // 绑定玩家纹理

							gameState = stGET2;//测试用
							if (gameState == stGET2)
							{
								draw_piles_getFromTrash();
								players[1].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
								card_full(players[1]);
							}
							startChangeSound();
						}
					}
				}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (r_pwh_Hero.contains(event.mouseButton.x, event.mouseButton.y))
					{
						lastState = gameState;
						is_game_start = true;
						is_send = false;
						is_selected = true;
						if (true_playerID == 0)
						{
							player1_hero_id = HEROS_AMOUNT;
							hero_id = HEROS_AMOUNT;
						}
						else if (true_playerID == 1)
						{
							player2_hero_id = HEROS_AMOUNT;
							hero_id = HEROS_AMOUNT;
						}
						else if (true_playerID == 2)
						{
							player3_hero_id = HEROS_AMOUNT;
							hero_id = HEROS_AMOUNT;
						}

						bind_player_textures(); // 绑定玩家纹理

						gameState = stGET2;//测试用
						if (gameState == stGET2)
						{
							draw_piles_getFromTrash();
							players[1].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
							card_full(players[1]);
						}
						startChangeSound();
					}
				}
			}
			else if (gameState == Num2GameState[true_playerID + 3])
			{
				if (roundOverRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					sRoundOver.setColor(sf::Color::Green);
				}
				else
				{
					sRoundOver.setColor(sf::Color::White);
				}

				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (roundOverRect.contains(event.mouseButton.x, event.mouseButton.y))
					{
						sRoundOver.setColor(sf::Color(100, 100, 100, 255));
						soundRoundButtom.play();
						gameState = lastState;
						responseState = response_fail;
						sending_card.emplace_back(0);
					}
				}
				if (!players.empty())
					if (players[3 - true_playerID].pai_list.size() > 0)
					{
						if (!players[3 - true_playerID].isHoverInCard)
						{
							if (players[3 - true_playerID].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[3 - true_playerID]);

							}
						}
						else
						{
							if (players[3 - true_playerID].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[3 - true_playerID]);
							}
						}
					}
				if (!players.empty())
					if (players[true_playerID].pai_list.size() > 0)
					{
						if (!players[true_playerID].isHoverInCard)
						{
							if (players[true_playerID].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[true_playerID]);

							}
						}
						else
						{
							if (players[true_playerID].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[true_playerID]);
							}
						}
						if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
						{
							LButtonResponse(sf::Mouse::getPosition(window), players[true_playerID]);
						}


					}
			}
			else if (gameState == Num2GameState[0] || gameState == Num2GameState[(3 - true_playerID)] || gameState == Num2GameState[3] || gameState == Num2GameState[(6 - true_playerID)])
			{
				sRoundOver.setColor(sf::Color(100, 100, 100, 255));

				if (!players.empty())
					if (players[3 - true_playerID].pai_list.size() > 0)
					{
						if (!players[3 - true_playerID].isHoverInCard)
						{
							if (players[3 - true_playerID].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[3 - true_playerID]);
							}
						}
						else
						{
							if (players[3 - true_playerID].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[3 - true_playerID]);
							}
						}
					}
				if (!players.empty())
					if (players[true_playerID].pai_list.size() > 0)
					{
						if (!players[true_playerID].isHoverInCard)
						{
							if (players[true_playerID].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[true_playerID]);
							}
						}
						else
						{
							if (players[true_playerID].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[true_playerID]);
								//mouseOut(sf::Mouse::getPosition(window), player3);
							}
						}

					}
			}


			else if (gameState == Num2GameState[true_playerID])
			{
				if (roundOverRect.contains(sf::Mouse::getPosition(window).x, sf::Mouse::getPosition(window).y))
				{
					sRoundOver.setColor(sf::Color::Green);
				}
				else
				{
					sRoundOver.setColor(sf::Color::White);
				}
				if (clickingState_stack.empty())//类型四的卡牌默认不能直接打出
				{
					for (auto it = players[true_playerID].pai_list.begin(); it != players[true_playerID].pai_list.end(); it++)
					{
						if (CardTypeCal[it->cardId] == 4)
						{
							it->card_gray = true;
						}
					}
				}

				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
				{
					if (!players.empty())
					{
						if (roundOverRect.contains(event.mouseButton.x, event.mouseButton.y))
						{
							soundRoundButtom.play();
							sRoundOver.setColor(sf::Color(100, 100, 100, 255));
							//sRoundOver.setTexture(tRoundBackGround);
							lastState = gameState;
							gameState = Num2GameState[(true_playerID + 1) % 3 + 6];
							draw_piles_getFromTrash();
							if ((true_playerID + 1) % 3 == 0)
								players[(true_playerID + 1) % 3].getCard(&drawPiles, LOUZHU_GET_CARD_AMOUNT);
							else
								players[(true_playerID + 1) % 3].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
							card_full(players[(true_playerID + 1) % 3]);

						}

						player_click(event, players[0]);
						//					player_click(event, players[1]);
					//						player_click(event, players[2]);
					}
				}

				if (!players.empty())
					if (players[3 - true_playerID].pai_list.size() > 0)
					{
						if (!players[3 - true_playerID].isHoverInCard)
						{
							if (players[3 - true_playerID].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[3 - true_playerID]);
							}
						}
						else
						{
							if (players[3 - true_playerID].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[3 - true_playerID]);
							}
						}
					}
				if (!players.empty())
					if (players[true_playerID].pai_list.size() > 0)
					{
						if (!players[true_playerID].isHoverInCard)
						{
							if (players[true_playerID].isCardInPosition)
							{
								mouseHover(sf::Mouse::getPosition(window), players[true_playerID]);
							}
						}
						else
						{
							if (players[true_playerID].hover_card_index != -1)
							{
								mouseOut(sf::Mouse::getPosition(window), players[true_playerID]);
							}
						}
						if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
						{
							if (clickingState_stack.empty())
								LButtonDown(sf::Mouse::getPosition(window), players[true_playerID]);
							else
							{
								if (clickingState_stack.top() != click_need_obj)
									LButtonDown(sf::Mouse::getPosition(window), players[true_playerID]);

							}


						}

					}
				if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Right)
				{
					RButtonDown(players[true_playerID]);
				}
			}


	}

}
void Game::player_click(sf::Event event, player& a_player)
{
	if (a_player.playerRect.contains(event.mouseButton.x, event.mouseButton.y))
	{
		if (!clickingState_stack.empty())
		{
			if (clickingState_stack.top() == click_need_obj)
			{
				clickingState_stack.pop();
				if (clickingState_stack.empty())
				{
					clickingState_stack.emplace(click_need_obj);
					clickingState_stack.emplace(click_func);
				}
				else
				{
					if (clickingState_stack.top() == clicked_need_card)
					{
						if (is_part_function(playing_card[0], playing_card[1]))
						{
							clickingState_stack.emplace(click_need_obj);
							//se_clicked = true;
							playing_card_objs.emplace_back(Player2Num[a_player.playerKind]);//添加释放的接收者
							sending_card_objs.emplace_back(Player2Num[a_player.playerKind]);

							//lastState = gameState;
							//gameState = Num2GameState[a_player.playerKind+3];
							a_player.playerHp += 2 * card_demage(playing_card[1]);
							a_player.Hp_decreased = true;
							is_fadded_done = false;

							while (!playing_card_objs.empty())
							{
								playing_card_objs.pop_back();
							}
							while (!playing_card.empty())
							{
								playing_card.pop_back();
							}
							while (!clickingState_stack.empty())
							{
								clickingState_stack.pop();
							}
							clickingState_stack.emplace(click_sending);
							//objs.pop_back();
							//std::cout << "send3typeto" << sending_card_objs[1] << std::endl;
							//playing_card.pop_back();
						}


					}
					else
					{
						clickingState_stack.emplace(click_need_obj);
						clickingState_stack.emplace(click_func);
					}
				}

				if (clickingState_stack.top() != click_sending)
				{
					playing_card_objs.emplace_back(Player2Num[a_player.playerKind]);//添加释放的接收者
					sending_card_objs.emplace_back(Player2Num[a_player.playerKind]);
				}
				std::cout << "send to" << Player2Num[a_player.playerKind] << std::endl;


			}
		}
	}
}
void Game::mouseHover(sf::Vector2i mPoint, player& a_player)
{

	sf::Vector2i tran_mPoint;
	bool is_found = false;
	for (int i = 0; i < a_player.pai_list.size(); i++)
	{
		tran_mPoint = a_player.pai_list[i].beforeTran(mPoint);
		if (a_player.pai_list[i].init_rec.contains(tran_mPoint.x, tran_mPoint.y))
		{
			a_player.isCardInHandChange = true;
			a_player.isCardsAngleSet = true;
			a_player.isCardInPosition = false;
			a_player.isHoverInCard = true;

			if (!is_found)
			{
				a_player.pai_list[i].isHover = true; //标记鼠标悬停在这张卡牌上
				a_player.hover_card_index = i;
				is_found = true;
				continue;
			}
		}
		a_player.pai_list[i].isHover = false;
	}
	if (a_player.hover_card_index != -1)
	{
		for (int i = 0; i < a_player.hover_card_index; i++)
		{
			a_player.pai_list[i].rotationAngle = -HOVER_SHFIT;
			a_player.pai_list[i].endAngle = a_player.pai_list[i].startAngle + a_player.pai_list[i].rotationAngle;
		}
		for (int i = a_player.hover_card_index + 1; i < a_player.pai_list.size(); i++)
		{
			a_player.pai_list[i].rotationAngle = HOVER_SHFIT;
			a_player.pai_list[i].endAngle = a_player.pai_list[i].startAngle + a_player.pai_list[i].rotationAngle;
		}
	}

}
void Game::mouseOut(sf::Vector2i mPoint, player& a_player)
{
	//std::cout << a_player.hover_card_index << std::endl;
	pai hovered_card = a_player.pai_list[a_player.hover_card_index];
	sf::Vector2i tran_mPoint;
	tran_mPoint = hovered_card.beforeTran(mPoint);
	if (!hovered_card.init_rec.contains(tran_mPoint.x, tran_mPoint.y))
	{
		a_player.isCardInHandChange = true;
		a_player.isCardsAngleSet = false; //让卡牌复位
		a_player.isCardInPosition = false;
		a_player.isHoverInCard = false;
		a_player.hover_card_index = -1;
		for (int i = 0; i < a_player.pai_list.size(); i++)
		{
			a_player.pai_list[i].isHover = false; //把手牌中的每一张牌的isHover属性都设置成false
		}
	}

}
void Game::RButtonDown(player& a_player)
{
	if (!clickingState_stack.empty())
	{
		if (clickingState_stack.top() == click_need_obj)
		{
			clickingState_stack.pop();
			a_player.pai_list.emplace_back(trashPiles.top());
			trashPiles.pop();
			playing_card.pop_back();
			sending_card.pop_back();
			if (clickingState_stack.empty())
			{
				playing_card_objs.pop_back();
				sending_card_objs.pop_back();
			}
		}
		else if (clickingState_stack.top() == clicked_need_card)
		{
			clickingState_stack.pop();
			clickingState_stack.pop();
			a_player.pai_list.emplace_back(trashPiles.top());
			trashPiles.pop();
			playing_card.pop_back();
			sending_card.pop_back();
			//playing_card_objs.pop_back();
			//if(!sending_card_objs.empty())
			//sending_card_objs.pop_back();
		}
	}
}
void Game::LButtonDown(sf::Vector2i mPoint, player& a_player)
{
	sf::Vector2i tran_mPoint;
	for (auto it = a_player.pai_list.begin(); it != a_player.pai_list.end();)
	{

		tran_mPoint = it->beforeTran(mPoint);
		if (it->init_rec.contains(tran_mPoint.x, tran_mPoint.y) && it->isHover == true && it->card_gray == false)
		{

			if (clickingState_stack.empty())
			{
				if (it->have_partner == false)
				{
					playing_card.emplace_back(it->cardId);
					sending_card.emplace_back(it->cardId);
					if (playing_card_objs.empty() == true)
					{
						playing_card_objs.emplace_back(true_playerID);//释放的发出者放在数组首位
						sending_card_objs.emplace_back(true_playerID);
					}
					if (it->have_obj == true)
					{
						clickingState_stack.emplace(click_need_obj);
					}
					else
					{
						clickingState_stack.emplace(click_func);
					}
				}
				else
				{
					playing_card.emplace_back(it->cardId);
					sending_card.emplace_back(it->cardId);
					if (playing_card_objs.empty() == true)
					{
						playing_card_objs.emplace_back(true_playerID);//释放的发出者放在数组首位
						sending_card_objs.emplace_back(true_playerID);
					}
					clickingState_stack.emplace(click_func);

					for (auto jt = a_player.pai_list.begin(); jt != a_player.pai_list.end(); jt++)
					{

						if (jt == it)
							continue;
						if (is_part_function(it->cardId, jt->cardId))//不能够被组合起来  
						{
							jt->card_gray = false;
							//传递灰色不可被选中draw信息
						}
						else
						{
							jt->card_gray = true;

						}


					}
				}
			}
			else
			{
				if (clickingState_stack.top() == clicked_need_card)
				{
					if (is_part_function(playing_card[0], it->cardId))
					{
						playing_card.emplace_back(it->cardId);
						sending_card.emplace_back(it->cardId);
						if (it->have_obj == true)
						{
							clickingState_stack.emplace(click_need_obj);
						}
						else
						{
							clickingState_stack.emplace(click_func);

						}
					}
					else
					{
						/*a_player.pai_list.emplace_back(trashPiles.top());
						trashPiles.pop();*/
						//playing_card.pop_back();

						RButtonDown(players[1]);
						break;
					}
				}
			}

			trashPiles.push(*it);
			it = a_player.pai_list.erase(it); //删除卡牌
			a_player.isCardInHandChange = true;
			a_player.isCardInPosition = false;
			a_player.isCardsAngleSet = false;
			a_player.isHoverInCard = false;
			a_player.hover_card_index = -1;



		}
		else
		{
			it++;
		}
	}
}
void Game::LButtonResponse(sf::Vector2i mPoint, player& a_player)
{
	sf::Vector2i tran_mPoint;
	for (auto it = a_player.pai_list.begin(); it != a_player.pai_list.end();)
	{

		tran_mPoint = it->beforeTran(mPoint);
		if (it->init_rec.contains(tran_mPoint.x, tran_mPoint.y) && it->isHover == true && it->card_gray == false)
		{
			if (judge_card(received_card[0], it->cardId))
			{
				response_card = it->cardId;

				trashPiles.push(*it);
				it = a_player.pai_list.erase(it); //删除卡牌
				a_player.isCardInHandChange = true;
				a_player.isCardInPosition = false;
				a_player.isCardsAngleSet = false;
				a_player.isHoverInCard = false;
				a_player.hover_card_index = -1;
			}

		}
		else
		{
			it++;
		}
	}
}

void Game::Logic()
{
	if (gameRestart == true)
	{
		beforeReStart();
	}
	if (responseState == response_fail)
	{

		card_received_func(received_card[0]);
		response_card = 0;
		responseState = need_response_card;
	}
	if (gameState == Num2GameState[true_playerID + 3])
	{
		//std::cout << received_card[0] << "anding" << response_card << std::endl;
		if (!received_card.empty() && response_card <= 25 && response_card >= 1)
		{
			card_response_func(response_card);
		}

		if ((response_card <= 25 && response_card >= 1) || responseState == response_success)
		{

			card_received_func(received_card[0]);
			response_card = 0;
			responseState = need_response_card;
		}
	}
	else if (gameState == Num2GameState[true_playerID])
	{
		if (!playing_card.empty())
		{
			card_func(playing_card[0]);
		}
	}

}
void Game::receive_clear()
{
	while (!received_card.empty())
	{
		received_card.pop_back();
	}

	while (!received_card_objs.empty())
	{
		std::cout << "obj:" << received_card_objs.front() << std::endl;
		received_card_objs.pop_back();
	}
}
void Game::card_received_func(int cardID)
{
	switch (responseState)
	{
	case response_fail:
		switch (CardTypeCal[cardID])
		{
		case 1:

			players[received_card_objs[0]].playerHp += card_demage(cardID);
			players[received_card_objs[0]].Hp_increased = true;
			is_fadded_done = false;

			receive_clear();
			break;

		case 3:

			receive_clear();
			break;


		case 2:
			players[received_card_objs[1]].playerHp += card_demage(cardID);
			players[received_card_objs[1]].Hp_decreased = true;
			is_fadded_done = false;
			receive_clear();
			break;

		default:
			break;

		}
	case response_success:
		receive_clear();
	}

}
void Game::card_response_func(int cardID)
{
	if (judge_card(received_card[0], cardID))
	{
		responseState = response_success;
		sending_card.emplace_back(cardID);
	}
	else
	{
		response_card = response_fail;
		sending_card.emplace_back(cardID);
	}
	gameState = lastState;
	std::cout << "response:" << GameState2Num[gameState] << std::endl;
}
void Game::card_full(player& a_player)
{
	while (a_player.pai_list.size() >= 10)
	{
		a_player.pai_list.pop_back();
	}

}
bool Game::judge_card(int card1, int card2)
{
	switch (card1)
	{
	case 3:

		if ((card2 >= 17 && card2 <= 25) || (card2 >= 13 && card2 <= 16))
			return true;
		break;

	case 4:
	case 7:
	case 20:
	case 21:
	case 22:

		if ((card2 >= 20 && card2 <= 22) || (card2 >= 13 && card2 <= 16))
			return true;
		break;

	case 8:
	case 23:
	case 24:
	case 25:
		if ((card2 >= 23 && card2 <= 25) || (card2 >= 13 && card2 <= 16))
			return true;
		break;
	case 9:
	case 12:
	case 17:
	case 18:
	case 19:
		if ((card2 >= 17 && card2 <= 19) || (card2 >= 13 && card2 <= 16))
			return true;
		break;
	case 11:
		if (card2 == 24)
			return true;
		break;


	}
	return false;
}
void Game::card_func(int cardID)
{
	if (clickingState_stack.top() == click_func)
	{
		switch (CardTypeCal[cardID])
		{
		case 1:

			players[playing_card_objs[0]].playerHp += card_demage(cardID);
			players[playing_card_objs[0]].Hp_increased = true;
			is_fadded_done = false;

			playing_card_objs.pop_back();
			playing_card.pop_back();
			while (!clickingState_stack.empty())
			{
				clickingState_stack.pop();
			}
			clickingState_stack.emplace(click_sending);
			break;


		case 3:
			clickingState_stack.emplace(clicked_need_card);
			for (int i = 0; i < players[true_playerID].pai_list.size(); i++)
			{
				if (is_part_function(playing_card[0], players[true_playerID].pai_list[i].cardId))
				{
					players[true_playerID].pai_list[i].card_gray = false;
				}
				else
				{
					players[true_playerID].pai_list[i].card_gray = true;
				}
			}

			break;

		case 2:
			lastState = gameState;
			gameState = Num2GameState[playing_card_objs[1] + 3];


			playing_card_objs.pop_back();
			playing_card_objs.pop_back();
			playing_card.pop_back();
			while (!clickingState_stack.empty())
			{
				clickingState_stack.pop();
			}
			clickingState_stack.emplace(click_sending);
			break;
		default:
			break;

		}
	}



}
bool Game::is_part_function(int cardID1, int cardID2)
{
	if (cardID1 == 5 && cardID2 <= 25 && cardID2 >= 23)
		return true;
	if (cardID1 == 6 && cardID2 <= 25 && cardID2 >= 17)
		return true;
	if (cardID1 == 8 && cardID2 <= 19 && cardID2 >= 17)
		return true;
	if (cardID1 == 10 && cardID2 <= 22 && cardID2 >= 20)
		return true;
	return false;
}
int Game::card_demage(int cardID)
{
	switch (cardID)
	{
	case 1:
	case 2:
		return -10;



	case 3:

		return 8;

	case 4:
	case 19:
	case 22:
	case 25:
		return 15;

	case 7:
	case 11:
	case 12:
	case 18:
	case 21:
	case 24:
		return 10;

	case 9:
		return 12;

	case 13:
		return 9;

	case 17:
	case 20:
	case 23:
		return 5;
	}

}
void Game::beforeReStart()
{
	while (!drawPiles.empty())
	{
		drawPiles.pop();
	}
	while (!trashPiles.empty())
	{
		trashPiles.pop();
	}
	while (!trashPiles_backUp.empty())
	{
		trashPiles_backUp.pop();
	}
	while (!players.empty())
	{
		players.pop_back();
	}
	while (!playing_card.empty())
	{
		playing_card.pop_back();
	}
	while (!playing_card_objs.empty())
	{
		playing_card_objs.pop_back();
	}
	while (!clickingState_stack.empty())
	{
		clickingState_stack.pop();
	}
	//clickingState_stack.emplace(click_sending);

}


void Game::Draw()
{
	if (gameState == stSTART)
	{
		sf::Time delta = clock.restart();
		window.clear();
		sStart.setPosition(552, 188);
		sStartButton.setPosition(235, 540);
		sGuide.setPosition(1258, 540);

		window.draw(sStartGround);
		window.draw(sStart);
		window.draw(sStartButton);
		window.draw(sGuide);
		window.display();

	}
	else if (gameState == stCHOOCE)
	{
		playHeroSound();
		stopHeroSound(playIndex);

		sf::Time delta = clock.restart();
		window.clear();
		sChooseBackground.setPosition(0, 0);
		window.draw(sChooseBackground);
		DrawHeros();

		int x_coefficient = HEROS_AMOUNT % HEROS_ROW_AMOUNT;
		int y_coefficient = HEROS_AMOUNT / HEROS_ROW_AMOUNT;
		sf::Vector2f moveSet(HERO_STEP_X * x_coefficient * 1.0, HERO_STEP_Y * y_coefficient * 1.0);

		pwh_Hero.setPosition(HEROS_POS + moveSet);
		pwh_Hero.setScale(0.7, 0.7);
		r_pwh_Hero.left = pwh_Hero.getPosition().x;
		r_pwh_Hero.top = pwh_Hero.getPosition().y;
		r_pwh_Hero.width = PLAYER_WIDTH * 0.7;
		r_pwh_Hero.height = PLAYER_HEIGHT * 0.7;
		//goblin_Hero_selected_cover.setPosition(sf::Vector2f(r_goblin_Hero.left, r_goblin_Hero.top));

		pwh_Hero.update(delta); //update the animate sprite for possible frame change
		window.draw(pwh_Hero); //display the animation
		//window.draw(goblin_Hero_selected_cover);
		window.display();

	}
	else if (gameState == stGUIDE)
	{
		window.clear();
		sGudiePage[page_guide].setPosition(0, 0);
		window.draw(sGudiePage[page_guide]);
		window.display();
	}
	else
	{
		window.clear();
		sBackground.setPosition(0, 0);
		if (!players.empty())
		{
			DrawBG();
			DrawPWH();

			DrawButton();
			DrawPlayingCard_fromTrash();
			DrawCardInfo();
			DrawHp();
			DrawLineArrow();
			DrawCard();
			DrawWin();
		}
		window.display();

		if (gameState == stGET1)
		{

			lastState = gameState;
			gameState = stPLAYER1;

		}
		if (gameState == stGET2)//laststate  1 gamestate 8
		{

			lastState = gameState;
			gameState = stPLAYER2;
		}
		if (gameState == stGET3)
		{

			lastState = gameState;
			gameState = stPLAYER3;
		}
	}

}
void Game::DrawBG()
{
	if (!clickingState_stack.empty())
	{
		if (clickingState_stack.top() == click_need_obj)
		{
			sBackground.setColor(sf::Color(100, 100, 100, 255));
			sPlayer1.setColor(sf::Color(255, 255, 255, 255));
			sPlayer2.setColor(sf::Color(255, 255, 255, 255));
			sPlayer3.setColor(sf::Color(255, 255, 255, 255));
			for (int i = 0; i < players[true_playerID].pai_list.size(); i++)
			{
				players[true_playerID].pai_list[i].card_gray = true;
			}
		}
		else if (clickingState_stack.top() == clicked_need_card)
		{
			sBackground.setColor(sf::Color(100, 100, 100, 255));
			sPlayer1.setColor(sf::Color(100, 100, 100, 255));
			sPlayer2.setColor(sf::Color(100, 100, 100, 255));
			sPlayer3.setColor(sf::Color(100, 100, 100, 255));

		}
		else
		{
			sBackground.setColor(sf::Color(255, 255, 255, 255));
			sPlayer1.setColor(sf::Color(255, 255, 255, 255));
			sPlayer2.setColor(sf::Color(255, 255, 255, 255));
			sPlayer3.setColor(sf::Color(255, 255, 255, 255));
			for (int i = 0; i < players[true_playerID].pai_list.size(); i++)
			{
				players[true_playerID].pai_list[i].card_gray = false;
			}
		}
	}
	else
	{
		sBackground.setColor(sf::Color(255, 255, 255, 255));
		sPlayer1.setColor(sf::Color(255, 255, 255, 255));
		sPlayer2.setColor(sf::Color(255, 255, 255, 255));
		sPlayer3.setColor(sf::Color(255, 255, 255, 255));
		if (!players.empty())
		{
			if (gameState != Num2GameState[true_playerID + 3])
			{
				for (int i = 0; i < players[true_playerID].pai_list.size(); i++)//默认玩家二
				{
					players[true_playerID].pai_list[i].card_gray = false;
				}
			}
		}
	}
	if (gameState != Num2GameState[true_playerID] && gameState != Num2GameState[true_playerID + 3])
	{
		for (int i = 0; i < players[true_playerID].pai_list.size(); i++)//默认玩家二
		{
			players[true_playerID].pai_list[i].card_gray = true;
		}
	}
	window.draw(sBackground);
	//player1.drawCard(&window);
	if (true_playerID == 0) // 本地玩家为楼主
	{
		sPlayer1.setPosition(LOCAL_LOUZHU_POS);
		sPlayer2.setPosition(LOCAL_LEFT_BAYOU_POS);
		sPlayer3.setPosition(LOCAL_RIGHT_BAYOU_POS);
	}
	else
	{
		sPlayer1.setPosition(LOUZHU_POS);
		sPlayer2.setPosition(LEFT_BAYOU_POS);
		sPlayer3.setPosition(RIGHT_BAYOU_POS);
	}
	window.draw(sPlayer1);
	window.draw(sPlayer2);
	window.draw(sPlayer3);
}
void Game::DrawPWH()
{
	sf::Time delta = clock.restart();
	if (is_selected_pwh)
	{
		pwh_Hero.setScale(1, 1);
		if (is_game_start)
		{
			pwh_Hero.setColor(sf::Color::White);
			pwh_Hero_copy1.setColor(sf::Color::White);
			pwh_Hero_copy2.setColor(sf::Color::White);
		}
		if (pwh_is_player1 == true)
		{
			pwh_Hero.setPosition(sPlayer1.getPosition());
		}
		else
		{
			pwh_Hero.setPosition(sf::Vector2f(-1000.0, -1000.0));
		}

		if (pwh_is_player2 == true)
		{
			pwh_Hero_copy1.setPosition(sPlayer2.getPosition());
		}
		else
		{
			pwh_Hero_copy1.setPosition(sf::Vector2f(-1000.0, -1000.0));
		}

		if (pwh_is_player3 == true)
		{
			pwh_Hero_copy2.setPosition(sPlayer3.getPosition());
		}
		else
		{
			pwh_Hero_copy2.setPosition(sf::Vector2f(-1000.0, -1000.0));
		}
		pwh_Hero.update(delta);
		pwh_Hero_copy1.update(delta);
		pwh_Hero_copy2.update(delta);
		window.draw(pwh_Hero);
		window.draw(pwh_Hero_copy1);
		window.draw(pwh_Hero_copy2);
		/*if (player1_hero_id == 9 || player2_hero_id == 9 || player3_hero_id == 9)
		{
			if (true_playerID == 0)
			{
				if (player1_hero_id == HEROS_AMOUNT)
				{
					window.draw(pwh_Hero);
				}
			}
			if (true_playerID == 1)
			{
				if (player2_hero_id == HEROS_AMOUNT)
				{
					window.draw(pwh_Hero_copy1);
				}
			}
			if (true_playerID == 2)
			{
				if (player3_hero_id == HEROS_AMOUNT)
				{
					window.draw(pwh_Hero_copy2);
				}
			}
		}*/
	}
}
void Game::DrawHeros()
{
	for (int i = 0; i < HEROS_AMOUNT; i++)
	{
		int x_coefficient = i % HEROS_ROW_AMOUNT;
		int y_coefficient = i / HEROS_ROW_AMOUNT;
		sf::Vector2f moveSet(HERO_STEP_X * x_coefficient * 1.0, HERO_STEP_Y * y_coefficient * 1.0);
		s_sHero[i].setPosition(HEROS_POS + moveSet);
		r_rHero[i] = s_sHero[i].getGlobalBounds();

		window.draw(s_sHero[i]);
	}
}
void Game::DrawCard()
{
	if (!players.empty())
	{
		if (true_playerID == 0)
			players[0].drawCard(&window);
		else
		{
			players[2].drawCard(&window);
			players[1].drawCard(&window);
		}

	}

}
void Game::DrawPlayingCard_fromTrash()
{
	if (!trashPiles.empty())
	{
		//tTrashCard = t_tCardBigger[trashPiles.top().cardId-CARDID_BEGIN];
		sTrashCard.setTexture(trashPiles.top().tCard);
		sTrashCard.setRotation(0.0);
		sTrashCard.setOrigin(0.0, 0.0);
		//sTrashCard.setScale(sf::Vector2f(1.0/1.2, 1.0/1.2)); 
		sTrashCard.setScale(sf::Vector2f(2.0, 2.0));

		sTrashCard.setPosition(850, 250);
		sTrashCard.setColor(sf::Color(255, 255, 255, 255));
		window.draw(sTrashCard);
	}
	trashPiles_backUp = trashPiles;
	for (int i = 0; i < 4; i++)
	{
		if (!trashPiles.empty())
		{

			tTrashCard = trashPiles.top().tCard;
			sTrashCard = trashPiles.top().sCard;
			sTrashCard.setRotation(0.0);
			sTrashCard.setOrigin(0.0, 0.0);
			sTrashCard.setScale(sf::Vector2f(0.5, 0.5)); //放大2倍
			sTrashCard.setPosition(1150, 250 + 100 * i);
			sTrashCard.setColor(sf::Color(255, 255, 255, 255));
			window.draw(sTrashCard);
			trashPiles.pop();
		}
	}
	trashPiles = trashPiles_backUp;
}
void Game::draw_piles_getFromTrash()
{
	if (drawPiles.empty())
	{
		while (!trashPiles.empty())
		{
			drawPiles.emplace(trashPiles.top());
			trashPiles.pop();
		}
	}
	else
	{
		if (drawPiles.size() <= 3)
		{
			while (!trashPiles.empty())
			{
				drawPiles.emplace(trashPiles.top());
				trashPiles.pop();
			}
		}
	}
}
void Game::DrawCardInfo()
{
	if (true_playerID == 0)
	{
		if (players[0].hover_card_index != -1)
		{
			players[0].sCard_Info.setTexture(t_tCardBigger[players[0].pai_list[players[0].hover_card_index].cardId - CARDID_BEGIN]);
			players[0].sCard_Info.setRotation(0.0);
			players[0].sCard_Info.setOrigin(0.0, 0.0);
			players[0].sCard_Info.setScale(sf::Vector2f(0.7, 0.7)); //0.7倍
			sf::Vector2f Card_Info_Position(players[0].pai_list[players[0].hover_card_index].sCard.getGlobalBounds().left, players[0].pai_list[players[0].hover_card_index].sCard.getGlobalBounds().top);
			players[0].sCard_Info.setPosition(Card_Info_Position + CARD_INFO_MOVESET);
			/*players[0].sCard_Info.setPosition(players[0].CardInfoPos.x, players[0].CardInfoPos.y);*/
			players[0].sCard_Info.setColor(sf::Color(255, 255, 255, 255));
			window.draw(players[0].sCard_Info);
		}
	}
	else
	{
		if (players[1].hover_card_index != -1)
		{
			players[1].sCard_Info.setTexture(t_tCardBigger[players[1].pai_list[players[1].hover_card_index].cardId - CARDID_BEGIN]);
			players[1].sCard_Info.setRotation(0.0);
			players[1].sCard_Info.setOrigin(0.0, 0.0);
			players[1].sCard_Info.setScale(sf::Vector2f(0.7, 0.7)); //0.7倍
			sf::Vector2f Card_Info_Position(players[1].pai_list[players[1].hover_card_index].sCard.getGlobalBounds().left, players[1].pai_list[players[1].hover_card_index].sCard.getGlobalBounds().top);
			players[1].sCard_Info.setPosition(Card_Info_Position + CARD_INFO_MOVESET);
			/*players[1].sCard_Info.setPosition(players[1].CardInfoPos.x, players[1].CardInfoPos.y);*/
			players[1].sCard_Info.setColor(sf::Color(255, 255, 255, 255));
			window.draw(players[1].sCard_Info);
		}
		if (players[2].hover_card_index != -1)
		{
			players[2].sCard_Info.setTexture(t_tCardBigger[players[2].pai_list[players[2].hover_card_index].cardId - CARDID_BEGIN]);
			players[2].sCard_Info.setRotation(0.0);
			players[2].sCard_Info.setOrigin(0.0, 0.0);
			players[2].sCard_Info.setScale(sf::Vector2f(0.7, 0.7)); //0.7倍
			sf::Vector2f Card_Info_Position(players[2].pai_list[players[2].hover_card_index].sCard.getGlobalBounds().left, players[2].pai_list[players[2].hover_card_index].sCard.getGlobalBounds().top);
			players[2].sCard_Info.setPosition(Card_Info_Position + CARD_INFO_MOVESET);
			/*players[2].sCard_Info.setPosition(players[2].CardInfoPos.x, players[2].CardInfoPos.y);*/
			players[2].sCard_Info.setColor(sf::Color(255, 255, 255, 255));
			window.draw(players[2].sCard_Info);
		}
	}
}
void Game::DrawButton()
{
	if (players[0].playerHp < LOUZHU_HP && players[1].playerHp < BAYOU_HP && players[2].playerHp < BAYOU_HP)
	{
		sRoundOver.setPosition(100, 500);
		if (gameState == stPLAYER2 && sRoundOver.getTexture() != &tRoundOver)
			sRoundOver.setTexture(tRoundOver);
		window.draw(sRoundOver);
	}
}
void Game::DrawHp()
{
	float magnification;
	if (true_playerID == 0)
	{
		magnification = 1.0 * PLAYER_HEIGHT / LOUZHU_HP;
		sf::RectangleShape rect1(sf::Vector2f(HP_WIDTH, magnification * players[0].playerHp));//设置大小
		rect1.setPosition(LOCAL_LOUZHU_HP_POS);//设置位置
		rect1.setFillColor(sf::Color::Red);//设置颜色
		sf::RectangleShape rect1_bk(sf::Vector2f(HP_WIDTH, PLAYER_HEIGHT));//设置大小
		rect1_bk.setPosition(LOCAL_LOUZHU_HP_POS);//设置位置
		rect1_bk.setFillColor(sf::Color::Green);//设置颜色
		window.draw(rect1_bk);
		window.draw(rect1);

		magnification = 1.0 * PLAYER_HEIGHT / BAYOU_HP;
		sf::RectangleShape rect2(sf::Vector2f(HP_WIDTH, magnification * players[1].playerHp));//设置大小
		rect2.setPosition(LOCAL_LEFT_HP_POS);//设置位置
		rect2.setFillColor(sf::Color::Red);//设置颜色
		sf::RectangleShape rect2_bk(sf::Vector2f(HP_WIDTH, PLAYER_HEIGHT));//设置大小
		rect2_bk.setPosition(LOCAL_LEFT_HP_POS);//设置位置
		rect2_bk.setFillColor(sf::Color::Green);//设置颜色
		window.draw(rect2_bk);
		window.draw(rect2);

		sf::RectangleShape rect3(sf::Vector2f(HP_WIDTH, magnification * players[2].playerHp));//设置大小
		rect3.setPosition(LOCAL_RIGHT_HP_POS);//设置位置
		rect3.setFillColor(sf::Color::Red);//设置颜色
		sf::RectangleShape rect3_bk(sf::Vector2f(HP_WIDTH, PLAYER_HEIGHT));//设置大小
		rect3_bk.setPosition(LOCAL_RIGHT_HP_POS);//设置位置
		rect3_bk.setFillColor(sf::Color::Green);//设置颜色
		window.draw(rect3_bk);
		window.draw(rect3);
	}
	else
	{
		magnification = 1.0 * PLAYER_HEIGHT / LOUZHU_HP;
		sf::RectangleShape rect1(sf::Vector2f(HP_WIDTH, magnification * players[0].playerHp));//设置大小
		rect1.setPosition(LOUZHU_HP_POS);//设置位置
		rect1.setFillColor(sf::Color::Red);//设置颜色
		sf::RectangleShape rect1_bk(sf::Vector2f(HP_WIDTH, PLAYER_HEIGHT));//设置大小
		rect1_bk.setPosition(LOUZHU_HP_POS);//设置位置
		rect1_bk.setFillColor(sf::Color::Green);//设置颜色
		window.draw(rect1_bk);
		window.draw(rect1);

		magnification = 1.0 * PLAYER_HEIGHT / BAYOU_HP;
		sf::RectangleShape rect2(sf::Vector2f(HP_WIDTH, magnification * players[1].playerHp));//设置大小
		rect2.setPosition(LEFT_BAYOU_HP_POS);//设置位置
		rect2.setFillColor(sf::Color::Red);//设置颜色
		sf::RectangleShape rect2_bk(sf::Vector2f(HP_WIDTH, PLAYER_HEIGHT));//设置大小
		rect2_bk.setPosition(LEFT_BAYOU_HP_POS);//设置位置
		rect2_bk.setFillColor(sf::Color::Green);//设置颜色
		window.draw(rect2_bk);
		window.draw(rect2);

		sf::RectangleShape rect3(sf::Vector2f(HP_WIDTH, magnification * players[2].playerHp));//设置大小
		rect3.setPosition(RIGHT_BAYOU_HP_POS);//设置位置
		rect3.setFillColor(sf::Color::Red);//设置颜色
		sf::RectangleShape rect3_bk(sf::Vector2f(HP_WIDTH, PLAYER_HEIGHT));//设置大小
		rect3_bk.setPosition(RIGHT_BAYOU_HP_POS);//设置位置
		rect3_bk.setFillColor(sf::Color::Green);//设置颜色
		window.draw(rect3_bk);
		window.draw(rect3);
	}

	if (!is_fadded_done)
	{
		DrawHpChange();
	}
}
void Game::DrawHpChange()
{
	for (int i = 0; i < 3; i++)
	{
		if (players[i].playerKind == true_playerID)
		{
			if (players[i].Hp_decreased == true)
			{
				soundHpUp.play();
				start_transparency = COVER_RECT_TRANSPARENCY;
				temp_transparency = COVER_RECT_TRANSPARENCY;
				end_transparency = 0.0;
				fadded_speed = 1.0 * COVER_RECT_TRANSPARENCY / COVER_RECT_FADDED_SPEED;
				sf::Vector3i the_color(255, 0, 0);
				fill_color = the_color;

				sf::RectangleShape temp(sf::Vector2f(Window_width, Window_height)); // 设置一个游戏画面大小的蒙版
				cover_rect = temp;
				cover_rect.setPosition(sf::Vector2f(0.0, 0.0)); // 设置位置
				cover_rect.setFillColor(sf::Color::Color(fill_color.x, fill_color.y, fill_color.z, COVER_RECT_TRANSPARENCY)); // 设置颜色
				players[i].Hp_decreased = false;

				window.draw(cover_rect);

				return;
			}
			else if (players[i].Hp_increased == true)
			{
				soundHpDown.play();
				start_transparency = COVER_RECT_TRANSPARENCY;
				temp_transparency = COVER_RECT_TRANSPARENCY;
				end_transparency = 0.0;
				fadded_speed = 1.0 * COVER_RECT_TRANSPARENCY / COVER_RECT_FADDED_SPEED;
				sf::Vector3i the_color(0, 255, 0);
				fill_color = the_color;

				sf::RectangleShape temp(sf::Vector2f(Window_width, Window_height)); // 设置一个游戏画面大小的蒙版
				cover_rect = temp;
				cover_rect.setPosition(sf::Vector2f(0.0, 0.0)); // 设置位置
				cover_rect.setFillColor(sf::Color::Color(fill_color.x, fill_color.y, fill_color.z, COVER_RECT_TRANSPARENCY)); // 设置颜色
				players[i].Hp_increased = false;

				window.draw(cover_rect);

				return;
			}
		}
		else
		{
			if (players[i].Hp_decreased == true)
			{
				soundHpUp.play();
				start_transparency = COVER_RECT_TRANSPARENCY;
				temp_transparency = COVER_RECT_TRANSPARENCY;
				end_transparency = 0.0;
				fadded_speed = 1.0 * COVER_RECT_TRANSPARENCY / COVER_RECT_FADDED_SPEED;
				sf::Vector3i the_color(255, 0, 0);
				fill_color = the_color;

				sf::RectangleShape temp(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT)); // 设置一个玩家头像大小的蒙版
				cover_rect = temp;
				if (players[i].playerKind == LOUZHU)
				{
					if (true_playerID == 0)
					{
						cover_rect.setPosition(LOCAL_LOUZHU_POS); // 设置位置
					}
					else
					{
						cover_rect.setPosition(LOUZHU_POS); // 设置位置
					}
				}
				else if (players[i].playerKind == LEFT_BAYOU)
				{
					if (true_playerID == 0)
					{
						cover_rect.setPosition(LOCAL_LEFT_BAYOU_POS); // 设置位置
					}
					else
					{
						cover_rect.setPosition(LEFT_BAYOU_POS); // 设置位置
					}
				}
				else if (players[i].playerKind == RIGHT_BAYOU)
				{
					if (true_playerID == 0)
					{
						cover_rect.setPosition(LOCAL_RIGHT_BAYOU_POS); // 设置位置
					}
					else
					{
						cover_rect.setPosition(RIGHT_BAYOU_POS); // 设置位置
					}
				}

				cover_rect.setFillColor(sf::Color::Color(fill_color.x, fill_color.y, fill_color.z, COVER_RECT_TRANSPARENCY)); // 设置颜色
				players[i].Hp_decreased = false;

				window.draw(cover_rect);

				return;
			}
			else if (players[i].Hp_increased == true)
			{
				soundHpDown.play();
				start_transparency = COVER_RECT_TRANSPARENCY;
				temp_transparency = COVER_RECT_TRANSPARENCY;
				end_transparency = 0.0;
				fadded_speed = 1.0 * COVER_RECT_TRANSPARENCY / COVER_RECT_FADDED_SPEED;
				sf::Vector3i the_color(0, 255, 0);
				fill_color = the_color;

				sf::RectangleShape temp(sf::Vector2f(PLAYER_WIDTH, PLAYER_HEIGHT)); // 设置一个游戏画面大小的蒙版
				cover_rect = temp;
				if (players[i].playerKind == LOUZHU)
				{
					if (true_playerID == 0)
					{
						cover_rect.setPosition(LOCAL_LOUZHU_POS); // 设置位置
					}
					else
					{
						cover_rect.setPosition(LOUZHU_POS); // 设置位置
					}
				}
				else if (players[i].playerKind == LEFT_BAYOU)
				{
					if (true_playerID == 0)
					{
						cover_rect.setPosition(LOCAL_LEFT_BAYOU_POS); // 设置位置
					}
					else
					{
						cover_rect.setPosition(LEFT_BAYOU_POS); // 设置位置
					}
				}
				else if (players[i].playerKind == RIGHT_BAYOU)
				{
					if (true_playerID == 0)
					{
						cover_rect.setPosition(LOCAL_RIGHT_BAYOU_POS); // 设置位置
					}
					else
					{
						cover_rect.setPosition(RIGHT_BAYOU_POS); // 设置位置
					}
				}
				cover_rect.setFillColor(sf::Color::Color(fill_color.x, fill_color.y, fill_color.z, COVER_RECT_TRANSPARENCY)); // 设置颜色
				players[i].Hp_increased = false;

				window.draw(cover_rect);

				return;
			}
		}
	}



	temp_transparency = temp_transparency - fadded_speed;
	cover_rect.setFillColor(sf::Color::Color(fill_color.x, fill_color.y, fill_color.z, temp_transparency)); // 设置颜色


	cover_rect_fadded_cnt++;
	if (cover_rect_fadded_cnt >= COVER_RECT_FADDED_SPEED)
	{
		cover_rect_fadded_cnt = 0;
		fadded_speed = 0, 0;
		/*start_transparency = end_transparency;
		temp_transparency = end_transparency;*/
		is_fadded_done = true;

	}

	window.draw(cover_rect);
}
void Game::DrawLineArrow()
{
	if (sending_card_objs.size() >= 2 && !is_valued) // 这里假设打出的牌只能指定一个玩家
	{
		if (sending_card_objs.front() == 0)
		{
			if (true_playerID == 0)
			{
				start_point = LOCAL_LOUZHU_POINT_POS;
				temp_point = LOCAL_LOUZHU_POINT_POS;
			}
			else
			{
				start_point = LOUZHU_POINT_POS;
				temp_point = LOUZHU_POINT_POS;
			}
		}
		else if (sending_card_objs.front() == 1)
		{
			if (true_playerID == 0)
			{
				start_point = LOCAL_LEFT_BAYOU_POINT_POS;
				temp_point = LOCAL_LEFT_BAYOU_POINT_POS;
			}
			else
			{
				start_point = LEFT_BAYOU_POINT_POS;
				temp_point = LEFT_BAYOU_POINT_POS;
			}
		}
		else if (sending_card_objs.front() == 2)
		{
			if (true_playerID == 0)
			{
				start_point = LOCAL_RIGHT_BAYOU_POINT_POS;
				temp_point = LOCAL_RIGHT_BAYOU_POINT_POS;
			}
			else
			{
				start_point = RIGHT_BAYOU_POINT_POS;
				temp_point = RIGHT_BAYOU_POINT_POS;
			}
		}

		if (sending_card_objs[1] == 0)
		{
			if (true_playerID == 0)
			{
				end_point = LOCAL_LOUZHU_POINT_POS;
			}
			else
			{
				end_point = LOUZHU_POINT_POS;
			}
		}
		else if (sending_card_objs[1] == 1)
		{
			if (true_playerID == 0)
			{
				end_point = LOCAL_LEFT_BAYOU_POINT_POS;
			}
			else
			{
				end_point = LEFT_BAYOU_POINT_POS;
			}
		}
		else if (sending_card_objs[1] == 2)
		{
			if (true_playerID == 0)
			{
				end_point = LOCAL_RIGHT_BAYOU_POINT_POS;
			}
			else
			{
				end_point = RIGHT_BAYOU_POINT_POS;
			}
		}

		growing_step = (end_point - start_point);
		growing_step = sf::Vector2f(growing_step.x / LINE_ARROW_GROWING_SPEED, growing_step.y / LINE_ARROW_GROWING_SPEED);
		lastTime_line_arrow = 1;

		is_valued = true;
	}
	if (received_card_objs.size() >= 2 && !is_valued) // 这里假设打出的牌只能指定一个玩家
	{
		if (received_card_objs.front() == 0)
		{
			if (true_playerID == 0)
			{
				start_point = LOCAL_LOUZHU_POINT_POS;
				temp_point = LOCAL_LOUZHU_POINT_POS;
			}
			else
			{
				start_point = LOUZHU_POINT_POS;
				temp_point = LOUZHU_POINT_POS;
			}
		}
		else if (received_card_objs.front() == 1)
		{
			if (true_playerID == 0)
			{
				start_point = LOCAL_LEFT_BAYOU_POINT_POS;
				temp_point = LOCAL_LEFT_BAYOU_POINT_POS;
			}
			else
			{
				start_point = LEFT_BAYOU_POINT_POS;
				temp_point = LEFT_BAYOU_POINT_POS;
			}
		}
		else if (received_card_objs.front() == 2)
		{
			if (true_playerID == 0)
			{
				start_point = LOCAL_RIGHT_BAYOU_POINT_POS;
				temp_point = LOCAL_RIGHT_BAYOU_POINT_POS;
			}
			else
			{
				start_point = RIGHT_BAYOU_POINT_POS;
				temp_point = RIGHT_BAYOU_POINT_POS;
			}
		}

		if (received_card_objs[1] == 0)
		{
			if (true_playerID == 0)
			{
				end_point = LOCAL_LOUZHU_POINT_POS;
			}
			else
			{
				end_point = LOUZHU_POINT_POS;
			}
		}
		else if (received_card_objs[1] == 1)
		{
			if (true_playerID == 0)
			{
				end_point = LOCAL_LEFT_BAYOU_POINT_POS;
			}
			else
			{
				end_point = LEFT_BAYOU_POINT_POS;
			}
		}
		else if (received_card_objs[1] == 2)
		{
			if (true_playerID == 0)
			{
				end_point = LOCAL_RIGHT_BAYOU_POINT_POS;
			}
			else
			{
				end_point = RIGHT_BAYOU_POINT_POS;
			}
		}

		growing_step = (end_point - start_point);
		growing_step = sf::Vector2f(growing_step.x / LINE_ARROW_GROWING_SPEED, growing_step.y / LINE_ARROW_GROWING_SPEED);
		lastTime_line_arrow = 1;

		is_valued = true;
	}

	if (lastTime_line_arrow)
	{
		if (is_DrawLineArrow_inPos)
		{
			static_DrawLineArrow();
		}
		else
		{
			growing_DrawLineArrow();
		}
	}

}
void Game::static_DrawLineArrow()
{
	// 设置直线的粗细和颜色
	float lineThickness = 5.0f;
	sf::Color lineColor = sf::Color::Red;

	// 计算直线的长度和角度
	float lineLength = std::sqrt(std::pow(end_point.x - start_point.x, 2) + std::pow(end_point.y - start_point.y, 2));
	float lineRotation = std::atan2(end_point.y - start_point.y, end_point.x - start_point.x) * 180 / M_PI;

	// 创建矩形直线
	sf::RectangleShape line(sf::Vector2f(lineLength, lineThickness));

	line.setOrigin(0.0, lineThickness / 2.0);
	line.setPosition(start_point);
	line.setRotation(lineRotation);
	line.setFillColor(lineColor);

	window.draw(line);


	// 计算箭头指向的向量
	sf::Vector2f direction = end_point - start_point;
	float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	direction = direction / length;
	float beta = atan2(direction.y, direction.x) * 180.0 / M_PI;


	// 创建箭头形状
	sf::ConvexShape arrow;
	arrow.setPointCount(3); // 设置点的数量为3
	arrow.setPoint(0, sf::Vector2f(0, 0));
	arrow.setPoint(1, sf::Vector2f(0, 20));
	arrow.setPoint(2, sf::Vector2f(40, 10));
	// 设置箭头的填充颜色
	arrow.setFillColor(sf::Color::Red);
	// 设置箭头位置
	arrow.setOrigin(sf::Vector2f(0, 10));
	arrow.setPosition(end_point);
	arrow.setRotation(beta);


	window.draw(arrow);

	lastTime_line_arrow++;
	if (lastTime_line_arrow >= LINE_ARROW_LASTTIME)
	{
		is_DrawLineArrow_inPos = false;
		lastTime_line_arrow = 0;
		is_valued = false;
	}

}
void Game::growing_DrawLineArrow()
{
	temp_point = temp_point + growing_step;

	// 设置直线的粗细和颜色
	float lineThickness = 5.0f;
	sf::Color lineColor = sf::Color::Red;

	// 计算直线的长度和角度
	float lineLength = std::sqrt(std::pow(temp_point.x - start_point.x, 2) + std::pow(temp_point.y - start_point.y, 2));
	float lineRotation = std::atan2(temp_point.y - start_point.y, temp_point.x - start_point.x) * 180 / M_PI;

	// 创建矩形直线
	sf::RectangleShape line(sf::Vector2f(lineLength, lineThickness));
	line.setOrigin(0.0, lineThickness / 2.0);
	line.setPosition(start_point);
	line.setRotation(lineRotation);
	line.setFillColor(lineColor);

	growing_cnt++;
	if (growing_cnt >= LINE_ARROW_GROWING_SPEED)
	{
		is_DrawLineArrow_inPos = true;
		growing_step.x = 0.0;
		growing_step.y = 0.0;
		growing_cnt = 0;
	}

	window.draw(line);
}
void Game::DrawWin()
{
	if (!players.empty())
	{

		if (players[0].playerHp >= LOUZHU_HP)
		{
			gameOver = true;
			if (true_playerID == 0) // 本地玩家为楼主
			{
				sPoFang.setPosition(LOCAL_LOUZHU_POS);
			}
			else
			{
				sPoFang.setPosition(LOUZHU_POS);
			}

			sReStart.setPosition(800, 500);
			sReStart.setScale(2.0, 2.0);
			window.draw(sPoFang);
			window.draw(sReStart);
		}
		else if (players[1].playerHp >= BAYOU_HP)
		{
			gameOver = true;
			if (true_playerID == 0) // 本地玩家为楼主
			{
				sPoFang.setPosition(LOCAL_LEFT_BAYOU_POS);
			}
			else
			{
				sPoFang.setPosition(LEFT_BAYOU_POS);
			}
			//sPoFang.setPosition(800, 700);
			sReStart.setPosition(800, 500);
			sReStart.setScale(2.0, 2.0);
			window.draw(sPoFang);
			window.draw(sReStart);
		}
		else if (players[2].playerHp >= BAYOU_HP)
		{
			gameOver = true;
			if (true_playerID == 0) // 本地玩家为楼主
			{
				sPoFang.setPosition(LOCAL_RIGHT_BAYOU_POS);
			}
			else
			{
				sPoFang.setPosition(RIGHT_BAYOU_POS);
			}
			//sPoFang.setPosition(1020, 700);
			sReStart.setPosition(800, 500);
			sReStart.setScale(2.0, 2.0);
			window.draw(sPoFang);
			window.draw(sReStart);
		}
	}
}
void Game::playHeroSound()
{
	if (!is_played_sound)
	{
		for (int i = 0; i < HEROS_AMOUNT; i++)
		{
			if (s_sHero[i].getColor() == sf::Color::Yellow)
			{
				playIndex = i;
				break;
			}
		}
		if (playIndex == -1 && is_hover_pwh == true)
		{
			playIndex = HEROS_AMOUNT;
		}
		if (playIndex != -1)
		{
			choose_bgMusic.pause(); // 暂停播放选将界面的背景音乐
			switch (playIndex)
			{
			case 0:
				soundCaiXukun.play();
				break;
			case 1:
				soundSpiderMan.play();
				break;
			case 2:
				soundDingZhen.play();
				break;
			case 3:
				soundSunXiaochuan.play();
				break;
			case 4:
				soundWujing.play();
				break;
			case 5:
				soundXiangjiaojun.play();
				break;
			case 6:
				soundXiaogui.play();
				break;
			case HEROS_AMOUNT:
				soundPWH.play();
				soundPWH.setVolume(500);
				break;
			default:
				break;
			}
			is_played_sound = true;
		}
	}
}
void Game::stopHeroSound(int index)
{
	if (is_played_sound)
	{
		if (index < HEROS_AMOUNT)
		{
			if (s_sHero[index].getColor() == sf::Color::White)
			{
				choose_bgMusic.play(); // 重新播放选将界面的背景音乐
				switch (index)
				{
				case 0:
					soundCaiXukun.stop();
					break;
				case 1:
					soundSpiderMan.stop();
					break;
				case 2:
					soundDingZhen.stop();
					break;
				case 3:
					soundSunXiaochuan.stop();
					break;
				case 4:
					soundWujing.stop();
					break;
				case 5:
					soundXiangjiaojun.stop();
					break;
				case 6:
					soundXiaogui.stop();
					break;
				default:
					break;
				}
				playIndex = -1;
				is_played_sound = false;
			}
		}
		else if (!is_hover_pwh)
		{
			soundPWH.stop();
			choose_bgMusic.play(); // 重新播放选将界面的背景音乐
			playIndex = -1;
			is_played_sound = false;
		}
	}
}
void Game::startChangeSound()
{
	soundCaiXukun.stop();
	soundDingZhen.stop();
	soundWujing.stop();
	soundSunXiaochuan.stop();
	soundSpiderMan.stop();
	soundXiaogui.stop();
	soundXiangjiaojun.stop();
	soundPWH.stop();

	choose_bgMusic.stop();
	soundMainMusic.play();
}


void Game::receiving()
{
	Game_packet.clear();
	memset(before_receive, 0, sizeof before_receive); // 把before_receive清空
	if (socket.receive(before_receive, sizeof(before_receive), received) == sf::Socket::Done)
	{
		// 判断接收到的是哪一个数据
		char* temp_arr = before_receive;
		temp_arr[received] = '\0'; // 在接收到的数据长度处添加null字符
		if (strlen(temp_arr) == 3)
		{
			player1_hero_id = before_receive[0] - '0';
			player2_hero_id = before_receive[1] - '0';
			player3_hero_id = before_receive[2] - '0';

			bind_player_textures();

			is_send = false;
		}
		else
		{
			if (strcmp(before_receive, "clientSend") == 0)
			{
				skS = true;
			}
			if (strcmp(before_receive, "clientReceive") == 0)
			{
				skR = true;
			}
			if (strcmp(before_receive, "clientFirstReceive") == 0)
			{
				skI = true;
			}
			if (strcmp(before_receive, "restart") == 0)
			{
				skRe = true;
				gameRestart = true;
			}
		}
	}
	if (skR)
	{
		// 如果接收到了数据，向服务端发送成功接收信息
		std::string message = "serverSend";
		socket.send(message.c_str(), message.size() + 1); // 向服务端发送接收数据预知
		// 等待服务端回应
		socket.setBlocking(true);  // 发送前把socket设成阻塞模式
		socket.receive(Game_packet);
		socket.setBlocking(false); // 把socket还原回非阻塞模式
		skR = false;

		unpackData(); // 解包数据
	}
	if (skI)
	{
		// 如果接收到了数据，向服务端发送成功接收信息
		std::string message = "serverSend";
		socket.send(message.c_str(), message.size() + 1); // 向服务端发送接收数据预知
		// 等待服务端回应
		socket.setBlocking(true);  // 发送前把socket设成阻塞模式
		socket.receive(Game_packet);
		socket.setBlocking(false); // 把socket还原回非阻塞模式
		skI = false;

		unpackData(); // 解包数据

		IniData(); // 初始化数据

	}

}
void Game::sending()
{
	if (gameRestart == true)
		return;
	Game_packet.clear();
	if (is_selected)
	{
		if (!is_send)
		{
			char message_c[2];
			message_c[0] = hero_id + '0';
			message_c[1] = true_playerID + '0';
			std::string message(message_c, 0, 2);

			socket.send(message.c_str(), message.size() + 1); // 向服务端发送数据，且只发送一次
			is_send = true;
			is_selected = false;
		}
	}
	if (is_game_start)
	{
		if (responseState == need_response_card)
		{
			packData();
			if (!is_send)
			{
				std::string message = "serverReceive";
				socket.send(message.c_str(), message.size() + 1); // 向服务端发送接收数据预知，且只发送一次
				is_send = true;
			}

			if (skS)
			{
				// 需要等待服务端接收了message后，才可以继续发送Game_packet
				socket.setBlocking(true);  // 发送前把socket设成阻塞模式
				socket.send(Game_packet);
				socket.setBlocking(false);  // 发送后把socket设成非阻塞模式

				is_send = false;
				skS = false;
				responseState = no_card;
				std::cout << "sending_state      " << GameState2Num[gameState] << "   " << GameState2Num[lastState] << std::endl;
				lastState = gameState;
				while (!sending_card.empty())
				{
					sending_card.pop_back();
				}
				while (!sending_card_objs.empty())
				{
					sending_card_objs.pop_back();
				}
			}

		}
		if (!clickingState_stack.empty())
		{
			if (clickingState_stack.top() == click_sending)
			{
				packData();

				if (!is_send)
				{
					std::string message = "serverReceive";
					socket.send(message.c_str(), message.size() + 1); // 向服务端发送接收数据预知，且只发送一次
					is_send = true;
				}

				if (skS)
				{
					// 需要等待服务端接收了message后，才可以继续发送Game_packet
					socket.setBlocking(true);  // 发送前把socket设成阻塞模式
					socket.send(Game_packet);
					socket.setBlocking(false);  // 发送后把socket设成非阻塞模式
					is_send = false;
					skS = false;
					while (!sending_card.empty())
					{
						sending_card.pop_back();
					}
					while (!sending_card_objs.empty())
					{
						sending_card_objs.pop_back();
					}
					clickingState_stack.pop();
				}

			}
		}
		if (lastState != gameState && (lastState == Num2GameState[(true_playerID + 1) % 3 + 6] || (lastState == stSTART) || (((gameState == Num2GameState[(true_playerID + 1) % 3 + 3]) || (gameState == Num2GameState[(true_playerID + 2) % 3 + 3])) && !sending_card.empty())))
		{

			packData();

			if (!is_send)
			{
				std::string message = "serverReceive";
				socket.send(message.c_str(), message.size() + 1); // 向服务端发送接收数据预知，且只发送一次
				is_send = true;
			}

			if (skS)
			{
				// 需要等待服务端接收了message后，才可以继续发送Game_packet
				socket.setBlocking(true);  // 发送前把socket设成阻塞模式
				socket.send(Game_packet);
				socket.setBlocking(false);  // 发送后把socket设成非阻塞模式
				is_send = false;
				skS = false;

				if (gameState != lastState)
					std::cout << GameState2Num[gameState] << "   " << GameState2Num[lastState] << std::endl;
				lastState = gameState;
				while (!sending_card.empty())
				{
					sending_card.pop_back();
				}
				while (!sending_card_objs.empty())
				{
					sending_card_objs.pop_back();
				}
			}
		}
	}
}
void Game::packData()
{
	int gameState_int = static_cast<int>(gameState);
	Game_packet << gameState_int;
	//打包std::vector<int> sending_card
	int sending_card_size = sending_card.size();
	Game_packet << sending_card_size;
	for (int i = 0; i < sending_card_size; i++)
	{
		Game_packet << sending_card[i];
	}

	//打包std::vector<PLAYER_KIND>sending_card_objs
	int sending_card_objs_size = sending_card_objs.size();
	if (gameState == stRESPONSE3)
	{
		std::cout << "sending_card_num:" << sending_card_size << std::endl;
	}
	Game_packet << sending_card_objs_size;
	for (int i = 0; i < sending_card_objs_size; i++)
	{
		Game_packet << (sending_card_objs[i]);
		//std::cout << "sending" <<sending_card_objs[i] << std::endl;

	}
	if (!players.empty())
		Game_packet << players[0].playerHp << players[1].playerHp << players[2].playerHp;
	else
		Game_packet << 0 << 0 << 0;
}
void Game::unpackData()
{

	if (is_first_receive)
	{
		//解包std::queue<pai> drawPiles
		int drawPiles_size;
		Game_packet >> drawPiles_size;
		std::queue<pai> empty;
		swap(empty, drawPiles);  //清空队列内容
		for (int i = 0; i < drawPiles_size; i++)  //设置抽牌堆的精灵的纹理
		{
			pai temp_card;
			temp_card.unpackData(Game_packet); //解包每一张牌
			drawPiles.push(temp_card);  //把解包后的牌插入drawPiles队尾
		}

	}


	int gameState_int;// 
	Game_packet >> gameState_int;

	//if (lastState != Num2GameState[(true_playerID + 1) % 3 + 3] && lastState != Num2GameState[(true_playerID + 2) % 3 + 3])
	{
		lastState = gameState;
	}
	gameState = static_cast<GAME_STATE>(gameState_int);
	std::cout << "receiving_state:   " << GameState2Num[gameState] << "laststate:" << GameState2Num[lastState] << std::endl;
	if (gameState == stPLAYER1 && lastState == stPLAYER3)
	{
		draw_piles_getFromTrash();
		players[0].getCard(&drawPiles, LOUZHU_GET_CARD_AMOUNT);
		card_full(players[0]);
	}
	if (gameState == stPLAYER2 && lastState == stPLAYER1)
	{
		draw_piles_getFromTrash();
		players[1].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
		card_full(players[1]);
	}
	if (gameState == stPLAYER3 && lastState == stPLAYER2)
	{
		draw_piles_getFromTrash();
		players[2].getCard(&drawPiles, BAYOU_GET_CARD_AMOUNT);
		card_full(players[2]);
	}

	//解包std::vector<int> received_card
	int received_card_size;
	Game_packet >> received_card_size;
	received_card.resize(received_card_size);
	for (int i = 0; i < received_card_size; i++)
	{
		Game_packet >> received_card[i];
		if (received_card[0] == 0)
		{
			received_card.pop_back();
		}
	}

	//解包std::vector<PLAYER_KIND> received_card_objs
	int  received_card_objs_size;
	Game_packet >> received_card_objs_size;
	received_card_objs.resize(received_card_objs_size);
	for (int i = 0; i < received_card_objs_size; i++)
	{
		//整型先转换成枚举型，再解包到received_card_objs中

		Game_packet >> received_card_objs[i];
		std::cout << "rec" << i << "objs" << received_card_objs[i] << std::endl;
	}
	if (gameState == Num2GameState[true_playerID + 3])
	{
		for (auto it = players[true_playerID].pai_list.begin(); it != players[true_playerID].pai_list.end(); it++)
		{
			if (judge_card(received_card[0], it->cardId))
			{
				it->card_gray = false;
			}
			else
			{
				it->card_gray = true;
			}

		}
	}

	for (int i = 0; i < received_card.size(); i++)
	{
		player& last_player = players[(GameState2Num[lastState]) % 3];
		for (auto it = last_player.pai_list.begin(); it != last_player.pai_list.end();)
		{

			if (received_card[i] == it->cardId)
			{
				trashPiles.push(*it);
				it = last_player.pai_list.erase(it); //删除卡牌
				last_player.isCardInHandChange = true;
				last_player.isCardInPosition = false;
				last_player.isCardsAngleSet = false;
				last_player.isHoverInCard = false;
				last_player.hover_card_index = -1;
				break;
			}
			else
			{
				it++;
			}
		}

	}
	for (int i = 0; i < 3; i++)
	{
		players[i].LastplayerHp = players[i].playerHp;
	}
	Game_packet >> players[0].playerHp >> players[1].playerHp >> players[2].playerHp;
	for (int i = 0; i < 3; i++)
	{
		if (players[i].LastplayerHp > players[i].playerHp)
		{
			players[i].Hp_increased = true;
			is_fadded_done = false;
		}
		if (players[i].LastplayerHp < players[i].playerHp)
		{
			players[i].Hp_decreased = true;
			is_fadded_done = false;
		}

	}


	if (is_first_receive)
	{
		Game_packet >> true_playerID;
		is_first_receive = false;
	}
}

void Game::Run()
{
	int cout_cnt = 0;
	do {
		Initial();
		while (window.isOpen() && gameRestart == false)
		{
			if (cout_cnt++ > 3000000)
			{
				std::cout << player1_hero_id << " " << player2_hero_id << " " << player3_hero_id << std::endl;
				cout_cnt = 0;
			}
			receiving();
			if (!is_game_start || player1_hero_id != 9 && player2_hero_id != 9 && player3_hero_id != 9)
			{
				Input();
			}
			Logic();
			Draw();
			sending();
		}
	} while (!gameQuit);
}


