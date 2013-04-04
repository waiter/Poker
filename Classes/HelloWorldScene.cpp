#include "HelloWorldScene.h"
#include "util/Button.h"
USING_NS_CC;

CCScene* HelloWorld::scene()
{
    CCScene *scene = CCScene::create();
    HelloWorld *layer = HelloWorld::create();
    scene->addChild(layer);
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCLayer::init());
		CCSpriteFrameCache* cache = CCSpriteFrameCache::sharedSpriteFrameCache();
		cache->addSpriteFramesWithFile("poker.plist");
		srand((unsigned)time(NULL));
		char s[100] = {0};

		CCMenuItemImage *pCloseItem = CCMenuItemImage::create(
			"CloseNormal.png",
			"CloseSelected.png",
			this,
			menu_selector(HelloWorld::menuCloseCallback));
		pCloseItem->setPosition(ccp(854 - pCloseItem->getContentSize().width/2 ,pCloseItem->getContentSize().height/2));
		CCMenu* pMenu = CCMenu::create(pCloseItem, NULL);
		pMenu->setPosition(CCPointZero);
		this->addChild(pMenu, 1);

		for(int i = 0 ; i < 52 ; i++){
			_allPoker[i] = i;
		}
		for(int j = 0 ; j < 20 ; j++){
			for(int i = 0 ; i < 52 ; i++){
				int temp = _allPoker[i];
				int rdI = (int)(52.0f * rand() / (RAND_MAX + 1.0f)) ;
				if(rdI != i){
					_allPoker[i] = _allPoker[rdI];
					_allPoker[rdI] = temp;
				}
			}
		}

		CCSpriteBatchNode* bn = CCSpriteBatchNode::create("poker.png" , 52 + 13);
		for(int i = 0 ; i < 52 ; i++){
			sprintf(s , "pc_%d.png" , i+1);
			_poker[i] = CCSprite::createWithSpriteFrameName(s);
			_poker[i]->setPosition(ccp(190 + 40 * (i % 13) , 400 - 60 * (int)(i / 13) ));
			bn->addChild(_poker[i]);
			_poker[i]->retain();

			_poker[i]->setVisible(false);
		}
		addChild(bn);

		for(int  i = 0 ; i < 13 ; i++){
			sprintf(s , "pc_%d.png" , _allPoker[i] + 1);
			_selfPoker[i] = Button::create(cache->spriteFrameByName(s),this,menu_selector(HelloWorld::pokerCallBack));
			_selfPoker[i]->setPosition(ccp(60 + 50 * (i % 13) , 30 ));
			bn->addChild(_selfPoker[i] , 3 + i);
			_selfPoker[i]->retain();

			_isActived[i] = false;
		}
		

		bRet = true;
	} while (0);
	return bRet;
}


void HelloWorld::menuCloseCallback(CCObject* pSender)
{
    CCDirector::sharedDirector()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}

HelloWorld::HelloWorld()
	:_currentSelect(-1)
	,_currentPlayer(0)
	,_needToShowCard(0)
{

}

HelloWorld::~HelloWorld()
{
	for(int i = 0 ; i < 52 ; i++){
		CC_SAFE_RELEASE_NULL(_poker[i]);
	}
	for(int i = 0 ; i < 13 ; i++){
		CC_SAFE_RELEASE_NULL(_selfPoker[i]);
	}
}

void HelloWorld::pokerCallBack( cocos2d::CCObject* pSender )
{
	if(_currentPlayer == 0){
		CCNode* n = (CCNode*)pSender;
		int index = n->getZOrder() - 3;
		if(!_isActived[index]){
			if(_currentSelect == index){
				_currentPlayer++;
				_needToShowCard = _allPoker[index];
				_isActived[index] = true;
				_selfPoker[index]->setEnabled(false);
				_selfPoker[index]->stopAllActions();
				_selfPoker[index]->runAction(CCSequence::create(
					CCEaseSineInOut::create(CCMoveTo::create(0.5f , _poker[_needToShowCard]->getPosition())),
					CCDelayTime::create(0.5f),
					CCCallFunc::create(this, callfunc_selector(HelloWorld::moveEndCallBack)),
					NULL));
				_selfPoker[index]->runAction(CCSequence::createWithTwoActions(
					CCScaleTo::create(0.5f, 1.3f),
					CCScaleTo::create(0.5f, 1.0f)
					));
			}else{
				if(_currentSelect > -1 && !_isActived[_currentSelect]){
					_selfPoker[_currentSelect]->stopAllActions();
					_selfPoker[_currentSelect]->runAction(CCEaseSineInOut::create(CCMoveTo::create(0.3f , ccp(_selfPoker[_currentSelect]->getPositionX() , 30))));
				}
				_currentSelect = index;
				_selfPoker[_currentSelect]->stopAllActions();
				_selfPoker[_currentSelect]->runAction(CCEaseSineInOut::create(CCMoveTo::create(0.3f , ccp(_selfPoker[_currentSelect]->getPositionX() , 60))));
			}
		}
	}
}

void HelloWorld::moveEndCallBack()
{
	_selfPoker[_currentSelect]->setVisible(false);
	_poker[_needToShowCard]->setVisible(true);
	_currentPlayer = 0;
	_currentSelect = -1;
}
