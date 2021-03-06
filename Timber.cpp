// Timber.cpp : Defines the entry point for the console application.
// Kaumudi P
// tree log cutting 
// single player game

#include "stdafx.h"
#include<SFML/Graphics.hpp>
#include<SFML/Audio.hpp>
#include<sstream>

using namespace sf;

void updateBranches(int seed);
const  int NUM_BRANCHES = 6;
Sprite branches[NUM_BRANCHES];

// Side
enum class side {LEFT, RIGHT, NONE};
side branchPosition[NUM_BRANCHES];


int main()
{
	//Create video mode object
	VideoMode vm(1920, 1080);  //1280, 720);

	// Create and open window in game
	RenderWindow window(vm, "Timber!", Style::Fullscreen); //Resize);

	//Texture - to hold graphics in GPU
	Texture textureBackground;

	//Load graphics to texture
	textureBackground.loadFromFile("graphics/background.png");

	Sprite spriteBackground;
	// Attach texture to sprite
	spriteBackground.setTexture(textureBackground);
	// Set sprite 
	spriteBackground.setPosition(0, 0);

	//Make tree sprite
	/// Not moving
	Texture textureTree;
	textureTree.loadFromFile("graphics/tree.png");
	Sprite spriteTree;
	spriteTree.setTexture(textureTree);
	spriteTree.setPosition(810, 0);
	
	// Enhanced version - more trees
	Texture textureTree2;
	textureTree2.loadFromFile("graphics/tree2.png");
	Sprite spriteTree2;
	spriteTree2.setTexture(textureTree2);
	Sprite spriteTree3;
	spriteTree3.setTexture(textureTree2);
	Sprite spriteTree4;
	spriteTree4.setTexture(textureTree2);
	Sprite spriteTree5;
	spriteTree5.setTexture(textureTree2);
	Sprite spriteTree6;
	spriteTree6.setTexture(textureTree2);


	spriteTree2.setPosition(20, 0);
	spriteTree3.setPosition(300, -400);
	spriteTree4.setPosition(1300, -400);
	spriteTree5.setPosition(1500, -500);
	spriteTree6.setPosition(1900, 0);
	// Moving Bee
	Texture textureBee;
	textureBee.loadFromFile("graphics/bee.png");
	Sprite spriteBee;
	spriteBee.setTexture(textureBee);
	spriteBee.setPosition(0, 800);
	// Is bee currently moving 
	bool beeActive = false;
	// Speed of bee
	float beeSpeed = 0.0f;

	// MAke 3 clouds
	//Load to texture

	Texture textureCloud;
	textureCloud.loadFromFile("graphics/cloud.png");
	//Add clouds
	Sprite spriteCloud1;
	Sprite spriteCloud2;
	Sprite spriteCloud3;
	spriteCloud1.setTexture(textureCloud);
	spriteCloud2.setTexture(textureCloud);
	spriteCloud3.setTexture(textureCloud);
	// Position clouds
	spriteCloud1.setPosition(0, 0);
	spriteCloud2.setPosition(500, 0);
	spriteCloud3.setPosition(1200, 10);

	// Are clouds active on screen
	bool cloud1Active = false;
	bool cloud2Active = false;
	bool cloud3Active = false;

	//Cloud speed
	float cloud1Speed = 0.0f;
	float cloud2Speed = 0.0f;
	float cloud3Speed = 0.0f;

	// Control time
	Clock clock;

	// Time Bar
	RectangleShape timeBar;
	float timeBarStartWidth = 400;
	float timeBarHeight = 80;
	timeBar.setSize(Vector2f(timeBarStartWidth, timeBarHeight));
	timeBar.setFillColor(Color::Magenta);
	timeBar.setPosition((1920 / 2) - timeBarStartWidth / 2, 980);

	Time gameTimeTotal;
	float timeRemaining = 6.0f, timeBarWidthPerSec = timeBarStartWidth / timeRemaining;

	// Track game status
	bool paused = true;

	/*    Adding text - score and message display */

	int score = 0;
	Text messageText;
	Text scoreText;
	// Font
	Font font;
	font.loadFromFile("fonts/KOMIKAP_.ttf");

	//Set font to msg
	messageText.setFont(font);
	scoreText.setFont(font);

	// Assign message text
	messageText.setString("Press Enter to start!");
	scoreText.setString("Score 0");

	// Set font size
	messageText.setCharacterSize(75);
	scoreText.setCharacterSize(60);

	// Set text color
	messageText.setFillColor(Color::White);
	scoreText.setFillColor(Color::White);
	// Position text
	FloatRect textRect = messageText.getLocalBounds();
	messageText.setOrigin(textRect.left +
		textRect.width / 2.0f,
		textRect.top + textRect.height / 2.0f);

	messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
	scoreText.setPosition(20, 20);

	//Add Branches
	Texture  textureBranch;
	textureBranch.loadFromFile("graphics/branch.png");
	//set texture for each branch
	for (int i = 0; i < NUM_BRANCHES; i++)
	{
		branches[i].setTexture(textureBranch);
		branches[i].setPosition(-2000, -2000);
		branches[i].setOrigin(220, 20);
	}

	// Add player
	Texture texturePlayer;
	texturePlayer.loadFromFile("graphics/player.png");
	Sprite spritePlayer;
	spritePlayer.setTexture(texturePlayer);
	spritePlayer.setPosition(500, 720);

	//start from left
	side playerSide = side::LEFT;

	// RIP
	Texture textureRIP;
	textureRIP.loadFromFile("graphics/rip.png");
	Sprite spriteRIP;
	spriteRIP.setTexture(textureRIP);
	spriteRIP.setPosition(675, 2000); // initialize it put of screen

	//Prepare axe
	Texture textureAxe;
	textureAxe.loadFromFile("graphics/axe.png");
	Sprite spriteAxe;
	spriteAxe.setTexture(textureAxe);
	spriteAxe.setPosition(700, 830);

	//Line axe with tree
	const float AXE_POSITION_LEFT = 700;
	const float AXE_POSITION_RIGHT = 1075;

	//Prepare flying log
	Texture textureLog;
	textureLog.loadFromFile("graphics/log.png");
	Sprite spriteLog;
	spriteLog.setTexture(textureLog);
	spriteLog.setPosition(810, 720);

	//log variables
	bool logActive = false;
	float logSpeedX = 1000;
	float logSpeedY = -1500;

	// Control player input
	bool acceptInput = false;

	// Adding sound effect
	SoundBuffer chopSound;
	chopSound.loadFromFile("sound/chop.wav");
	Sound chop;
	chop.setBuffer(chopSound);

	SoundBuffer gameOverSound;
	gameOverSound.loadFromFile("sound/death.wav");
	Sound gameOver;
	gameOver.setBuffer(gameOverSound);

	SoundBuffer ootSound;
	ootSound.loadFromFile("sound/out_of_time.wav");
	Sound outOfTime;
	outOfTime.setBuffer(ootSound);

	while (window.isOpen())
	{
		//Handle player input

		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::KeyReleased && !paused)
			{
				//accept
				acceptInput = true;

				//hide axe
				spriteAxe.setPosition(2000, spriteAxe.getPosition().y);
			}
		}


		//if esc key is pressed
		if (Keyboard::isKeyPressed(Keyboard::Escape))
		{
			window.close();
		}

		if (Keyboard::isKeyPressed(Keyboard::Return))
		{
			paused = false;
			//score = 0;
			//timeRemaining = 5;

			//Make all the branches disappear
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				branchPosition[i] = side::NONE;
			}

			//hide graveyard
			spriteRIP.setPosition(675, 2000); // out of screen

			// player to starting location
			spritePlayer.setPosition(500, 720);
			acceptInput = true;

		}
		if (Keyboard::isKeyPressed(Keyboard::Space))
		{
			paused = true;
		}

		// ready to accept input
		if (acceptInput && !paused && timeRemaining > 0.0f)
		{
			// if arrow keys are pressed
			// right key pressed
			if (Keyboard::isKeyPressed(Keyboard::Right))
			{
				//move player to right side
				playerSide = side::RIGHT;
				score++;

				//Player lives , add time
				timeRemaining += (2 / score) + .15;

				// change axe position also
				spriteAxe.setPosition(AXE_POSITION_RIGHT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(1200, 720);

				// update branche
				updateBranches(score);

				// set log flying
				spriteLog.setPosition(810, 720);
				logSpeedX = -5000;
				logActive = true;
				acceptInput = false;

				chop.play();

			}

			// left key pressed
			if (Keyboard::isKeyPressed(Keyboard::Left))
			{
				//move player to right side
				playerSide = side::LEFT;
				score++;

				//Player lives , add time
				timeRemaining += (2 / score) + .15;

				// change axe position also
				spriteAxe.setPosition(AXE_POSITION_LEFT, spriteAxe.getPosition().y);

				spritePlayer.setPosition(580, 720);

				// update branche
				updateBranches(score);

				// set log flying
				spriteLog.setPosition(810, 720);
				logSpeedX = 5000;
				logActive = true;
				acceptInput = false;

				chop.play();

			}//left

		}


		if (!paused)
		{
			// Update scene for each time frame 
			Time dt = clock.restart();

			timeRemaining -= dt.asSeconds();

			// resize timeBar
			if (timeRemaining > 0.0f) {
				timeBar.setSize(Vector2f(timeBarWidthPerSec*timeRemaining, timeBarHeight));
			}
			else
			{
				timeBar.setSize(Vector2f(0, timeBarHeight));
			}

			//if time Runs out
			if (timeRemaining <= 0.0f)
			{
				paused = true;
				// change message text
				messageText.setString("Time Out!");

				// Reposition txt
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				messageText.setFillColor(Color::Red);
				messageText.setCharacterSize(120);

				outOfTime.play();
			}

			/****    BEE            **/
			if (!beeActive)
			{
				// Activate Bee
				srand((int)time(0) * 10);
				beeSpeed = (rand() % 200) + 200;

				//Area thebee can travel
				srand((int)time(0) * 10);
				float height = (rand() % 500) + 500;
				spriteBee.setPosition(2000, height);
				beeActive = true;
			}
			else
			{
				//Move the bee
				spriteBee.setPosition(spriteBee.getPosition().x -
					(beeSpeed *dt.asSeconds()), spriteBee.getPosition().y);

				if (spriteBee.getPosition().x < -100)
				{
					beeActive = false;
				}
			}
			/***********  CLOUDS   ******************/
			//Manage clouds
			if (!cloud1Active)
			{
				// Activate Bee
				srand((int)time(0) * 10);
				cloud1Speed = (rand() % 200);

				//Area the cloud can travel
				srand((int)time(0) * 10);
				float height = (rand() % 150);
				spriteCloud1.setPosition(100, height);
				cloud1Active = true;
			}
			else
			{
				//Move the cloud
				spriteCloud1.setPosition(spriteCloud1.getPosition().x +
					(cloud1Speed *dt.asSeconds()), spriteCloud1.getPosition().y);

				if (spriteCloud1.getPosition().x > 1920)
				{
					cloud1Active = false;
				}
			}


			//Manage clouds 2
			if (!cloud2Active)
			{
				// Activate Bee
				srand((int)time(0) * 20);
				cloud2Speed = (rand() % 250);

				//Area the cloud can travel
				srand((int)time(0) * 20);
				float height = (rand() % 160);
				spriteCloud2.setPosition(600, height);
				cloud2Active = true;
			}
			else
			{
				//Move the cloud
				spriteCloud2.setPosition(spriteCloud2.getPosition().x +
					(cloud2Speed *dt.asSeconds()), spriteCloud2.getPosition().y);

				if (spriteCloud2.getPosition().x > 1920)
				{
					cloud2Active = false;
				}
			}

			//Manage clouds 3
			if (!cloud3Active)
			{
				// Activate Bee
				srand((int)time(0) * 30);
				cloud3Speed = (rand() % 300);

				//Area the cloud can travel
				srand((int)time(0) * 30);
				float height = (rand() % 170);
				spriteCloud3.setPosition(900, height);
				cloud3Active = true;
			}
			else
			{
				//Move the cloud
				spriteCloud3.setPosition(spriteCloud3.getPosition().x +
					(cloud3Speed *dt.asSeconds()), spriteCloud3.getPosition().y);

				if (spriteCloud3.getPosition().x > 1920)
				{
					cloud3Active = false;
				}
			}

			/*******CLOUDS done************/

			std::stringstream ss;
			ss << "Score " << score;
			scoreText.setString(ss.str());

			// Update branch sprite
			for (int i = 0; i < NUM_BRANCHES; i++)
			{
				float height = i * 150;
				if (branchPosition[i] == side::LEFT)
				{
					// Left side 
					branches[i].setPosition(610, height);
					//set sprite rotation
					branches[i].setRotation(180);
				}
				else if (branchPosition[i] == side::RIGHT)
				{
					branches[i].setPosition(1330, height);
					//set sprite rotation
					branches[i].setRotation(0);
				}
				else
				{
					branches[i].setPosition(3000, height);
				}
			}

			if (logActive)
			{
				spriteLog.setPosition(spriteLog.getPosition().x +
					(logSpeedX*dt.asSeconds()),
					spriteLog.getPosition().y +
					(logSpeedY*dt.asSeconds()));

				//has log reached the right edge
				if (spriteLog.getPosition().x < -100 ||
					spriteLog.getPosition().y > 2000)
				{
					//add new log
					logActive = false;
					spriteLog.setPosition(810, 720);

				}
			}

			// if player squished by branch
			if (branchPosition[5] == playerSide)
			{
				paused = true;
				acceptInput = false;

				// draw gravestone
				spriteRIP.setPosition(650, 750);
				window.draw(spriteRIP);

				//hide player
				spritePlayer.setPosition(800, 2000);

				// hide axe
				spriteAxe.setPosition(800, 2000);

				// hide flying log
				spriteLog.setPosition(800, 2000);

				//Change text message
				messageText.setString("Squished!");

				// Reposition txt
				FloatRect textRect = messageText.getLocalBounds();
				messageText.setOrigin(textRect.left +
					textRect.width / 2.0f,
					textRect.top + textRect.height / 2.0f);

				messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);
				messageText.setFillColor(Color::Red);
				messageText.setCharacterSize(120);

				timeRemaining = -10;

				gameOver.play();
			}


		}// endif not paused

		//clear last frame
		window.clear();

		//draw scene
		window.draw(spriteBackground);

		// draw as per layered.. background, then 
		// cloud then tree or they will overlap
		window.draw(spriteCloud1);
		window.draw(spriteCloud2);
		window.draw(spriteCloud3);

		for (int i = 0; i < NUM_BRANCHES; i++)
		{
			window.draw(branches[i]);
		}
		window.draw(spriteTree);

		//Draw player
		window.draw(spritePlayer);

		// Draw axe
		window.draw(spriteAxe);

		//draw log flying
		window.draw(spriteLog);

		// draw graveyard
		window.draw(spriteRIP);

		window.draw(spriteBee);

		window.draw(timeBar);

		// Draw text
		window.draw(scoreText);
		if (timeRemaining > 0.0f && paused)
		{
			// then only show Enter message
			messageText.setString("Press Enter to start!");
			// Set font size
			messageText.setCharacterSize(75);
			// Set text color
			messageText.setFillColor(Color::White);
			// Position text
			FloatRect textRect = messageText.getLocalBounds();
			messageText.setOrigin(textRect.left +
				textRect.width / 2.0f,
				textRect.top + textRect.height / 2.0f);

			messageText.setPosition(1920 / 2.0f, 1080 / 2.0f);


		}

		if (paused || timeRemaining < 0.0f) {
			window.draw(messageText);
		}
	
		//double buffering - window hidden and 
		// displayed when triggerred
		//Show everything
		window.display();
	}

    return 0;
}

void updateBranches(int seed)
{
	// Move branches down
	for (int j = NUM_BRANCHES - 1; j > 0; j--)
	{
		branchPosition[j] = branchPosition[j - 1];
	}
	// Spawn new branch at position 0


	srand((int)time(0) + seed);
	int r = (rand() % 5);
	switch (r)
	{
	case 0:
		branchPosition[0] = side::LEFT;
		break;
	case 1:
		branchPosition[0] = side::RIGHT;
		break;
	default:
		branchPosition[0] = side::NONE;
		break;
	}
}
