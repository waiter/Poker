#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__
#include "cocos2d.h"
class Button;
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

	Button* _selfPoker[13];

	int _currentSelect;
	bool _isActived[13];
	int _allPoker[52];
	int _needToShowCard;
	int _currentPlayer;

	void menuCloseCallback(cocos2d::CCObject* pSender);
	void pokerCallBack(cocos2d::CCObject* pSender);
	void moveEndCallBack();
};

#endif // __HELLOWORLD_SCENE_H__
