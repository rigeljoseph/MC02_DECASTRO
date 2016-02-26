#include "HelloWorldScene.h"
#define COCOS2D_DEBUG 1
#define CCRANDOM_10_710()
USING_NS_CC;


Scene* HelloWorld::createScene()
{
	
	// 'scene' is an autorelease object
	auto scene = Scene::createWithPhysics();
	scene->getPhysicsWorld()->setGravity(Vect(0.0f, 0.0f));
	auto layer = HelloWorld::create();
	scene->addChild(layer);

	return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}

	/*
	score = cocos2d::Label::create();
	score->setPosition(Vec2(650, 480));
	score->setString(std::to_string(scr));
	this->addChild(score);*/
	score = cocos2d::Label::create();
	score->setPosition(Vec2(360, 270));
	score->setString(std::to_string(scr));
	score->setSystemFontSize(200);

	this->addChild(score);
	// the walls
	wall = Sprite::create();
	auto wallBody = PhysicsBody::createEdgeBox(Size(720, 540), PHYSICSBODY_MATERIAL_DEFAULT);
	wallBody->getShape(0)->setRestitution(1);
	wallBody->getShape(0)->setFriction(0);
	wallBody->getShape(0)->setDensity(1);
	wall->setPosition(Point(360, 270));
	wall->setPhysicsBody(wallBody);
	wallBody->setContactTestBitmask(true);
	wall->setTag(1);
	this->addChild(wall);

/*	ball = Sprite::create("ball.png");
	ball->setPosition(360, 140);
	auto ballBody = PhysicsBody::createCircle(20);
	ballBody->setGravityEnable(false);
	Vect velocity = Vect(500000.0, 500000.0);
	ballBody->getShape(0)->setRestitution(1);
	ballBody->getShape(0)->setFriction(0);
	ballBody->getShape(0)->setDensity(1);
	ballBody->applyImpulse(velocity);
	ballBody->setContactTestBitmask(true);
	ball->setTag(2);
	ball->setPhysicsBody(ballBody);
	this->addChild(ball);*/

	paddle = Sprite::create("paddle.png");
	paddle->setPosition(360, 100);
	auto padBod = PhysicsBody::createBox(Size(110,20),PHYSICSBODY_MATERIAL_DEFAULT);
	padBod->setGravityEnable(false);
	padBod->getShape(0)->setRestitution(1);
	padBod->getShape(0)->setFriction(0);
	padBod->getShape(0)->setDensity(1);
	padBod->setDynamic(false);
	padBod->setContactTestBitmask(true);
	paddle->setPhysicsBody(padBod);
	paddle->setTag(3);
	this->addChild(paddle);
	this->schedule(schedule_selector(HelloWorld::update) );
	

	auto eventListener = EventListenerKeyboard::create();

	eventListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event* event) {
		EventKeyboard::KeyCode pressedKey = keyCode;
		
		switch (pressedKey) {
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			direction = 1;
			moving = 1;

			break;
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW:
			direction = 2;
			moving = 1;
			break;
		case EventKeyboard::KeyCode::KEY_SPACE:
			
			if (start == 0){
			start = 1;
			}
			
			break;
		}

	};
	eventListener->onKeyReleased = [&](EventKeyboard::KeyCode keyCode, Event* event){
		switch(keyCode){
		case EventKeyboard::KeyCode::KEY_RIGHT_ARROW :
			moving = 0;
			break;
		case EventKeyboard::KeyCode::KEY_LEFT_ARROW:
			moving = 0;
			break;
		}
	};
	this->_eventDispatcher->addEventListenerWithSceneGraphPriority(eventListener, paddle);



	int c = 500;
	for (int i = 0; i < 6; i++) {
	cocos2d::Sprite* brick;
	int b = 85;
		for (int a = 0; a < 6 ; a++){
			auto brickBody = PhysicsBody::createBox(Size(90, 20), PHYSICSBODY_MATERIAL_DEFAULT);
			brickBody->getShape(0)->setRestitution(1);
			brickBody->getShape(0)->setFriction(0);
			brickBody->getShape(0)->setDensity(10); 
			brickBody->setDynamic(false);
			brickBody->setContactTestBitmask(true);
			if (i % 3 == 0) {
				brick = cocos2d::Sprite::create("greena.png");
			}
			else if (i % 3 == 1) {
				brick = cocos2d::Sprite::create("greenb.png");
			}
			else if (i % 3 == 2) {
				brick = cocos2d::Sprite::create("greenc.png");
			}
		brick->setPosition(b,c);
		brick->setTag(4);
		brick->setPhysicsBody(brickBody);
		this->addChild(brick);
		b = b + 110;
		
		}
	c = c - 30;
	}

	bg = Sprite::create("start.png");
	bg->setPosition(Vec2(740 / 2, 540 / 2));
	this->addChild(bg);
	
	auto dispatcher = Director::getInstance()->getEventDispatcher();
	auto contactListener = EventListenerPhysicsContact::create();
	contactListener->onContactBegin = CC_CALLBACK_1(HelloWorld::onContactBegin, this);
	dispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

	
	return true;
}

bool HelloWorld::onContactBegin(PhysicsContact& collide) {
	
	auto object1 = (Sprite*)collide.getShapeA()->getBody()->getNode();
	auto object2 = (Sprite*)collide.getShapeB()->getBody()->getNode();

	
	int	first = object1->getTag();
	int second = object2->getTag();

	if (first == 4) {
		this->removeChild(object1, true);
		scr++;
		score->setString(std::to_string(scr));
		if (scr == 36){
			bg = cocos2d::Sprite::create("winner.png");
			bg->setPosition(740 / 2, 540 / 2);
			bg->setTag(5);
			this->addChild(bg);
		}
	
	}
	if (second == 4) {
		this->removeChild(object2, true);
		scr++;
		score->setString(std::to_string(scr));
		if (scr == 36) {
			bg = cocos2d::Sprite::create("winner.png");
			bg->setPosition(740 / 2, 540 / 2);
			bg->setTag(5);
			this->addChild(bg);
		}
	}
	if ((first == 1 || second == 1) && (ball->getPositionY() < paddle->getPositionY())) {
		cocos2d::Sprite* bg;
		CCLOG("GET");
		bg = cocos2d::Sprite::create("lose.png");
		//start = 2;
		bg->setPosition(740 / 2, 540	 / 2);
		bg->setTag(5);
		this->addChild(bg);
	}

	return true;
}

void::HelloWorld::update(float delta){
	auto position = paddle->getPosition();
	if (start == 1) {
		start = 2;
		this->removeChild(bg);
		ball = Sprite::create("ball.png");
		ball->setPosition(360, 140);
		auto ballBody = PhysicsBody::createCircle(20);
		ballBody->setGravityEnable(false);
		Vect velocity = Vect(500000.0, 500000.0);
		ballBody->getShape(0)->setRestitution(1);
		ballBody->getShape(0)->setFriction(0);
		ballBody->getShape(0)->setDensity(1);
		ballBody->applyImpulse(velocity);
		ballBody->setContactTestBitmask(true);
		ball->setTag(2);
		ball->setPhysicsBody(ballBody);
		this->addChild(ball);
		;
	}
	if (moving && (direction == 1 && position.x > 60)) {
		paddle->setPosition(position.x - 20, position.y);
	}
	else if (moving && (direction == 2 && position.x < 680)) {
		paddle->setPosition(position.x + 20, position.y);
	}


}