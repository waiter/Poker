#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
class Button;
class AI;
class HelloWorld : public cocos2d::CCLayer
{
public:
	HelloWorld();
	~HelloWorld();
    virtual bool init();  

    static cocos2d::CCScene* scene();
    
    CREATE_FUNC(HelloWorld);
private:
	cocos2d::CCSprite* _poker[52];
	cocos2d::CCSprite* _pokerBack[3];
	cocos2d::CCLabelTTF* _pokerCount[3];
	cocos2d::CCLabelTTF* _whosTurn;
	cocos2d::CCSprite* _actionPoker;

	Button* _selfPoker[13];
	AI* _ais[3];

	int _currentSelect;
	bool _isActived[13];
	int _userPoker[13];
	int _needToShowCard;
	int _currentPlayer;

	void menuCloseCallback(cocos2d::CCObject* pSender);
	void pokerCallBack(cocos2d::CCObject* pSender);
	void moveEndCallBack();

	void show7();
};

#endif // __HELLOWORLD_SCENE_H__
