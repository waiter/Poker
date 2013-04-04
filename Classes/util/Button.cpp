#include "Button.h"
USING_NS_CC;
/************************************************************************
Button  v1.2.2
************************************************************************/
Button::Button()
	:p_isEnabled(true)
	,p_isSelected(false)
	,p_activateL(NULL)
	,p_activateS(NULL)
	,p_selectL(NULL)
	,p_selectS(NULL)
	,p_unselectL(NULL)
	,p_unselectS(NULL)
	,p_stableScaleX(1.0f)
	,p_stableScaleY(1.0f)
	,p_isStable(false)
	,p_startSelectedTime(NULL)
	,p_selectedDurationTime(0)
{

}

Button::~Button()
{
	CC_SAFE_DELETE(p_startSelectedTime);
}

void Button::activate()
{
	if(isEnabled() && p_activateL && p_activateS){
		(p_activateL->*p_activateS)(this);
	}
}

void Button::selected()
{
	if(isEnabled()){
		p_isSelected = true;
		p_selectedDurationTime = 0;
		CCTime::gettimeofdayCocos2d(p_startSelectedTime, NULL);
		if(p_selectL && p_selectS){
			(p_selectL->*p_selectS)(this);
		}
	}
}

void Button::unselected()
{
	if(isEnabled()){
		p_isSelected = false;
		struct cc_timeval now;
		CCTime::gettimeofdayCocos2d(&now, NULL);
		p_selectedDurationTime = (now.tv_sec - p_startSelectedTime->tv_sec) + (now.tv_usec - p_startSelectedTime->tv_usec) / 1000000.0f;
		if(p_unselectL && p_unselectS){
			(p_unselectL->*p_unselectS)(this);
		}
	}
}

void Button::onEnter()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->addTargetedDelegate(this, kCCMenuHandlerPriority, true);
	CCSprite::onEnter();
}

void Button::onExit()
{
	CCDirector* pDirector = CCDirector::sharedDirector();
	pDirector->getTouchDispatcher()->removeDelegate(this);
	CCSprite::onExit();
}

bool Button::init(CCSpriteFrame* spriteframe , CCObject* rec, SEL_MenuHandler selector)
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!CCSprite::initWithSpriteFrame(spriteframe));

		p_activateL = rec;
		p_activateS = selector;

		p_selectL = this;
		p_selectS = menu_selector(Button::normalSelectedS);

		p_unselectL = this;
		p_unselectS = menu_selector(Button::normalUnselectedS);

		p_startSelectedTime = new struct cc_timeval();
		bRet = true;
	} while (0);
	return bRet;
}

Button* Button::create( CCSpriteFrame* spriteframe , cocos2d::CCObject* rec, cocos2d::SEL_MenuHandler selector )
{
	Button* button = new Button();
	if(button && button->init(spriteframe , rec , selector)){
		button->autorelease();
		return button;
	}else{
		delete button;
		button = NULL;
		return NULL;
	}
}

void Button::normalSelectedS(CCObject* pSender)
{

}

void Button::normalUnselectedS(CCObject* pSender)
{

}

bool Button::ccTouchBegan( CCTouch* touch, CCEvent* event )
{
	if(!isEnabled() || !isVisible()){
		return false;
	}

	for (CCNode *c = this->m_pParent; c != NULL; c = c->getParent())
	{
		if (c->isVisible() == false)
		{
			return false;
		}
	}

	if(containsTouchLocation(touch)){
		selected();
		return true;
	}

	return false;
}

void Button::ccTouchMoved( CCTouch* touch, CCEvent* event )
{
	if(isEnabled()){
		if(containsTouchLocation(touch)){
			if(!isSelected()){
				selected();
			}
		}else{
			if(isSelected()){
				unselected();
			}
		}
	}
}

void Button::ccTouchEnded( CCTouch* touch, CCEvent* event )
{
	if(isEnabled()){
		if(isSelected()){
			unselected();
			if(containsTouchLocation(touch)){
				activate();
			}
		}
	}
}

cocos2d::CCRect Button::rect()
{
	CCSize s = getContentSize();
	return CCRectMake(-m_obAnchorPointInPoints.x , -m_obAnchorPointInPoints.y , s.width , s.height);
}

bool Button::containsTouchLocation( CCTouch* touch )
{
	return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

void Button::setEnabled( bool isEnable )
{
	p_isEnabled = isEnable;
}

void Button::setSelectedTarget( cocos2d::CCObject* rec ,cocos2d::SEL_MenuHandler selector )
{
	p_selectL = rec;
	p_selectS = selector;
}

void Button::setUnselectedTarget( cocos2d::CCObject* rec ,cocos2d::SEL_MenuHandler selector )
{
	p_unselectL = rec;
	p_unselectS = selector;
}

void Button::setTarget( cocos2d::CCObject* rec ,cocos2d::SEL_MenuHandler selector )
{
	p_activateL = rec;
	p_activateS = selector;
}

/************************************************************************
NormalButton 
************************************************************************/
NormalButton::NormalButton()
	:p_normalFrame(NULL)
	,p_seletedFrame(NULL)
	,p_disableFrame(NULL)
	,_currentFrame(0)
{

}

NormalButton::~NormalButton()
{
	CC_SAFE_RELEASE_NULL(p_normalFrame);
	CC_SAFE_RELEASE_NULL(p_seletedFrame);
	CC_SAFE_RELEASE_NULL(p_disableFrame);
}

bool NormalButton::init( cocos2d::CCSpriteFrame* noraml , cocos2d::CCSpriteFrame* selected , cocos2d::CCSpriteFrame* disable , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector )
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Button::init(noraml , rec , selector));
		p_normalFrame = noraml;
		p_seletedFrame = selected;
		p_disableFrame = disable;

		CC_SAFE_RETAIN(p_normalFrame);
		CC_SAFE_RETAIN(p_seletedFrame);
		CC_SAFE_RETAIN(p_disableFrame);
		bRet = true;
	} while (0);
	return bRet;
}

void NormalButton::normalSelectedS( cocos2d::CCObject* pSender )
{
	if(_currentFrame != cfSelected && p_seletedFrame){
		setDisplayFrame(p_seletedFrame);
		_currentFrame = cfSelected;
	}
}

void NormalButton::normalUnselectedS( cocos2d::CCObject* pSender )
{
	if(_currentFrame != cfNormal && p_normalFrame){
		setDisplayFrame(p_normalFrame);
		_currentFrame = cfNormal;
	}
}

void NormalButton::setEnabled( bool isEnable )
{
	if(p_isEnabled != isEnable){
		p_isEnabled = isEnable;
		if(p_isEnabled){
			if(_currentFrame != cfNormal && p_normalFrame){
				setDisplayFrame(p_normalFrame);
				_currentFrame = cfNormal;
			}
		}else{
			if(_currentFrame != cfDisable && p_disableFrame){
				setDisplayFrame(p_disableFrame);
				_currentFrame = cfDisable;
			}
		}
	}
}

NormalButton* NormalButton::create( cocos2d::CCSpriteFrame* noraml , cocos2d::CCSpriteFrame* selected , cocos2d::CCSpriteFrame* disable , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector )
{
	NormalButton* nb = new NormalButton();
	if(nb && nb->init(noraml,selected,disable,rec,selector)){
		nb->autorelease();
		return nb;
	}else{
		delete nb;
		nb = NULL;
		return NULL;
	}
}

NormalButton* NormalButton::create( cocos2d::CCSpriteFrame* noraml , cocos2d::CCSpriteFrame* selected , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector )
{
	return NormalButton::create(noraml , selected , NULL , rec , selector);
}

NormalButton* NormalButton::create( cocos2d::CCSpriteFrame* noraml  , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector , cocos2d::CCSpriteFrame* disable)
{
	return NormalButton::create(noraml , NULL , disable , rec , selector);
}

NormalButton* NormalButton::create( cocos2d::CCSpriteFrame* noraml , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector )
{
	return NormalButton::create(noraml , NULL , NULL , rec , selector);
}
/************************************************************************
ScaleButton
************************************************************************/
ScaleButton::ScaleButton()
	:p_normalFrame(NULL)
	,p_disableFrame(NULL)
	,p_scale(1.0f)
{

}

ScaleButton::~ScaleButton()
{
	CC_SAFE_RELEASE_NULL(p_normalFrame);
	CC_SAFE_RELEASE_NULL(p_disableFrame);
}

bool ScaleButton::init( cocos2d::CCSpriteFrame* noraml , float scale , cocos2d::CCSpriteFrame* disable , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector )
{
	bool bRet = false;
	do 
	{
		CC_BREAK_IF(!Button::init(noraml , rec , selector));

		p_scale = scale;
		p_normalFrame = noraml;
		p_disableFrame = disable;

		CC_SAFE_RETAIN(p_normalFrame);
		CC_SAFE_RETAIN(p_disableFrame);

		bRet = true;
	} while (0);
	return bRet;
}

void ScaleButton::setEnabled( bool isEnable )
{
	if(p_isEnabled != isEnable){
		p_isEnabled = isEnable;
		if(p_isEnabled){
			if(_currentFrame != cfNormal && p_normalFrame){
				setDisplayFrame(p_normalFrame);
				_currentFrame = cfNormal;
			}
		}else{
			if(_currentFrame != cfDisable && p_disableFrame){
				setDisplayFrame(p_disableFrame);
				_currentFrame = cfDisable;
			}
		}
		toScale(true);
	}
}

void ScaleButton::normalSelectedS( cocos2d::CCObject* pSender )
{
	toScale(false);
}

void ScaleButton::normalUnselectedS( cocos2d::CCObject* pSender )
{
	if(_currentFrame != cfNormal && p_normalFrame){
		setDisplayFrame(p_normalFrame);
		_currentFrame = cfNormal;
	}
	toScale(true);
}

void ScaleButton::toScale( bool isToNoraml )
{
	if(isToNoraml){
		if(isStable()){
			if(m_fScaleX != p_stableScaleX) setScaleX(p_stableScaleX);
			if(m_fScaleY != p_stableScaleY) setScaleY(p_stableScaleY);
		}else{
			if(m_fScaleX != 1.0f) setScaleX(1.0f);
			if(m_fScaleY != 1.0f) setScaleY(1.0f);
		}
	}else{
		if(isStable()){
			if(m_fScaleX != p_stableScaleX * p_scale) setScaleX(p_stableScaleX * p_scale);
			if(m_fScaleY != p_stableScaleY * p_scale) setScaleY(p_stableScaleY * p_scale);
		}else{
			if(m_fScaleX != 1.0f * p_scale) setScaleX(1.0f * p_scale);
			if(m_fScaleY != 1.0f * p_scale) setScaleY(1.0f * p_scale);
		}
	}
}

ScaleButton* ScaleButton::create( cocos2d::CCSpriteFrame* noraml , float scale , cocos2d::CCSpriteFrame* disable , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector )
{
	ScaleButton* scalebutton = new ScaleButton();
	if (scalebutton && scalebutton->init(noraml , scale , disable , rec , selector))
	{
		scalebutton->autorelease();
		return scalebutton;
	} 
	else
	{
		delete scalebutton;
		scalebutton = NULL;
		return NULL;
	}
}

ScaleButton* ScaleButton::create( cocos2d::CCSpriteFrame* noraml , float scale , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector )
{
	return create(noraml , scale , NULL , rec , selector);
}
