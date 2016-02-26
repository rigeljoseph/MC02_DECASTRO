#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class HelloWorld : public cocos2d::Layer
{
public:
	static cocos2d::Scene* createScene();
	virtual bool init();


	CREATE_FUNC(HelloWorld);

public:
	cocos2d::Sprite* paddle;
	cocos2d::Sprite* ball;
	cocos2d::Sprite* bg;
	cocos2d::Sprite* wall;
	PhysicsWorld* physics;
	void setPhysicsWorld(PhysicsWorld* world) { physics = world; };
	bool onContactBegin(PhysicsContact& collision);
	void update(float delta);

	
private:
	cocos2d::Label* score;


public:
	int scr = 0;
	int moving = 0;
	int direction = 1;
	int start = 0;

	

};