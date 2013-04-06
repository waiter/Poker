#include "HelloWorldScene.h"
#include "util/Button.h"
#include "ai/AI.h"
#include "GameCenter.h"
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

		GameCenter::sharedGameCenter()->shuffle();

		for(int i = 0 ; i < 3 ; i++){
			_ais[i] = new AI();
			_ais[i]->resetWithPoker(GameCenter::sharedGameCenter()->getPlayerPoker(i + 1));
		}

		CCSpriteBatchNode* bn = CCSpriteBatchNode::create("poker.png" , 52 + 13 + 1 + 3);
		addChild(bn);
		for(int i = 0 ; i < 52 ; i++){
			sprintf(s , "pc_%d.png" , i+1);
			_poker[i] = CCSprite::createWithSpriteFrameName(s);
			_poker[i]->setPosition(ccp(190 + 40 * (i % 13) , 400 - 60 * (int)(i / 13) ));
			bn->addChild(_poker[i]);
			_poker[i]->retain();

			_poker[i]->setVisible(false);
		}
		for(int i = 0 ; i < 3 ; i++){
			_pokerBack[i] = CCSprite::createWithSpriteFrameName("pc_0.png");
			_pokerBack[i]->setPosition(ccp(30 + 730 * (int)(i / 2) , 190 + 150 * (int)((i+1) / 2)));
			_pokerBack[i]->setScale(0.5f);
			bn->addChild(_pokerBack[i]);
			_pokerBack[i]->retain();

			_pokerCount[i] = CCLabelTTF::create("X13","default",35);
			_pokerCount[i]->setAnchorPoint(ccp(0,0.5f));
			_pokerCount[i]->setPosition(ccpAdd(_pokerBack[i]->getPosition(),ccp(30,0)));
			addChild(_pokerCount[i]);
			_pokerCount[i]->retain();
		}

		std::vector<int> userp = GameCenter::sharedGameCenter()->getPlayerPoker(0);
		for(int  i = 0 ; i < 13 ; i++){
			_userPoker[i] = userp.at(i);
			sprintf(s , "pc_%d.png" , _userPoker[i] + 1);
			_selfPoker[i] = Button::create(cache->spriteFrameByName(s),this,menu_selector(HelloWorld::pokerCallBack));
			_selfPoker[i]->setPosition(ccp(60 + 50 * (i % 13) , 30 ));
			bn->addChild(_selfPoker[i] , 3 + i);
			_selfPoker[i]->retain();

			_isActived[i] = false;
		}
		
		_actionPoker = CCSprite::createWithSpriteFrameName("pc_1.png");
		bn->addChild(_actionPoker , 3);
		_actionPoker->retain();
		_actionPoker->setVisible(false);

		_whosTurn = CCLabelTTF::create("you turn" , "default" , 40);
		_whosTurn->setPosition(ccp(427,240));
		addChild(_whosTurn);
		_whosTurn->retain();

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
	,_currentPlayer(-1)
	,_needToShowCard(0)
	,_whosTurn(NULL)
	,_actionPoker(NULL)
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
	for(int i = 0 ; i < 3 ; i++){
		CC_SAFE_DELETE(_ais[i]);
		CC_SAFE_RELEASE_NULL(_pokerBack[i]);
		CC_SAFE_RELEASE_NULL(_pokerCount[i]);
	}
	CC_SAFE_RELEASE_NULL(_whosTurn);
	CC_SAFE_RELEASE_NULL(_actionPoker);
}

void HelloWorld::pokerCallBack( cocos2d::CCObject* pSender )
{
	if(_currentPlayer == 0){
		CCNode* n = (CCNode*)pSender;
		int index = n->getZOrder() - 3;
		if(!_isActived[index]){
			if(_currentSelect == index){
				_currentPlayer++;
				_needToShowCard = _userPoker[index];
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
	_actionPoker->setVisible(false);
	_poker[_needToShowCard]->setVisible(true);
	//_currentPlayer = 0;
	if(_currentPlayer == 4){
		_currentPlayer = 0;
		_whosTurn->setString("your turn");
		_currentSelect = -1;
	}else{
		char s[100] = {0};
		sprintf(s , "player %d" , _currentPlayer);
		_whosTurn->setString(s);
		if(_ais[_currentPlayer - 1]->getLeftPokerCount() > 0){
			_needToShowCard = _ais[_currentPlayer - 1]->thisTimePoker();
			sprintf(s , "pc_%d.png" , _needToShowCard + 1);
			_actionPoker->setDisplayFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(s));
			_actionPoker->stopAllActions();
			_actionPoker->setPosition(ccp(30 + 794 * (int)(_currentPlayer / 3) , 190 + 150 * (int)(_currentPlayer / 2)));
			_actionPoker->setVisible(true);
			_actionPoker->runAction(CCSequence::create(
				CCEaseSineInOut::create(CCMoveTo::create(0.5f , _poker[_needToShowCard]->getPosition())),
				CCDelayTime::create(0.5f),
				CCCallFunc::create(this, callfunc_selector(HelloWorld::moveEndCallBack)),
				NULL));
			_actionPoker->runAction(CCSequence::createWithTwoActions(
				CCScaleTo::create(0.5f, 1.3f),
				CCScaleTo::create(0.5f, 1.0f)
				));
			_currentPlayer++;
		}
	}
}

void HelloWorld::show7()
{

}
