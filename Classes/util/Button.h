/************************************************************************
Button (can be add to BatchNode) v1.2.2
author: waiter
v1.0.1:
remove event retain and release (same to CCMenuItem , have some problem , but...)
v1.1.0:
add selected time , so it can be set long click event
v1.1.1:
change getSelectedDurationTime() return from int to double , improve the accuracy.
v1.1.2:
use cocos2d-x V2.0.2
v1.2.0
check visable to catch the event
v1.2.1
repair the scale bug
v1.2.2
for cocos2d-x 2.1
************************************************************************/
#ifndef __BUTTON_H__WT_
#define __BUTTON_H__WT_
#include "cocos2d.h"
/************************************************************************
Button  base class
Subclass Button to creat new type of button
you can set three callback function:
selected  (setSelectedTarget)
unselected  (setUnselectedTarget)
click  (setTarget)
************************************************************************/
class Button : public cocos2d::CCSprite, public cocos2d::CCTargetedTouchDelegate
{
public:
	Button();
	~Button();
	virtual bool init(cocos2d::CCSpriteFrame* spriteframe , cocos2d::CCObject* rec, cocos2d::SEL_MenuHandler selector);
	virtual void onEnter();
	virtual void onExit();

	cocos2d::CCRect rect();
	bool containsTouchLocation(cocos2d::CCTouch* touch);

	/** Activate the item */
	virtual void activate();
	/** The item was selected (not activated), similar to "mouse-over" */
	virtual void selected();
	/** The item was unselected */
	virtual void unselected();

	inline bool isSelected(){
		return p_isSelected;
	};
	inline bool isEnabled(){
		return p_isEnabled;
	};
	virtual void setEnabled(bool isEnable);

	virtual bool ccTouchBegan(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchMoved(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);
	virtual void ccTouchEnded(cocos2d::CCTouch* touch, cocos2d::CCEvent* event);

	virtual void normalSelectedS(cocos2d::CCObject* pSender);
	virtual void normalUnselectedS(cocos2d::CCObject* pSender);

	virtual void setSelectedTarget(cocos2d::CCObject* rec ,cocos2d::SEL_MenuHandler selector);
	virtual void setUnselectedTarget(cocos2d::CCObject* rec ,cocos2d::SEL_MenuHandler selector);
	virtual void setTarget(cocos2d::CCObject* rec ,cocos2d::SEL_MenuHandler selector);

	inline bool isStable(){
		return p_isStable;
	};
	inline float getStableScaleX(){
		return p_stableScaleX;
	};
	inline float getStableScaleY(){
		return p_stableScaleY;
	};
	inline void saveStableScale(bool isSave){
		p_isStable = isSave;
		if(isSave){
			p_stableScaleX = m_fScaleX;
			p_stableScaleY = m_fScaleY;
		}else{
			p_stableScaleX = 1.0f;
			p_stableScaleY = 1.0f;
		}
	};
	//return seconds (with microSeconds) . 
	inline double getSelectedDurationTime(){
		return p_selectedDurationTime;
	};

	static Button* create(cocos2d::CCSpriteFrame* spriteframe , cocos2d::CCObject* rec, cocos2d::SEL_MenuHandler selector);
protected:
	bool p_isEnabled;
	bool p_isSelected;
	float p_stableScaleX;
	float p_stableScaleY;
	bool p_isStable;
	cocos2d::CCObject* p_selectL;
	cocos2d::SEL_MenuHandler p_selectS;
	cocos2d::CCObject* p_unselectL;
	cocos2d::SEL_MenuHandler p_unselectS;
	cocos2d::CCObject* p_activateL;
	cocos2d::SEL_MenuHandler p_activateS;
	struct cocos2d::cc_timeval *p_startSelectedTime;
	double p_selectedDurationTime;
};
/************************************************************************
NormalButton 
The images has 3 different states:
- unselected image (must not be NULL)
- selected image (can be NULL , if NULL it show UNSELECTED IMAGE)
- disabled image (can be NULL , if NULL it show UNSELECTED IMAGE)
************************************************************************/
enum{
	cfNormal = 0,
	cfSelected,
	cfDisable,
};
class NormalButton : public Button
{
public:
	NormalButton();
	~NormalButton();
	bool init(cocos2d::CCSpriteFrame* noraml , cocos2d::CCSpriteFrame* selected , cocos2d::CCSpriteFrame* disable , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector);

	virtual void setEnabled(bool isEnable);

	virtual void normalSelectedS(cocos2d::CCObject* pSender);
	virtual void normalUnselectedS(cocos2d::CCObject* pSender);

	static NormalButton* create(cocos2d::CCSpriteFrame* noraml , cocos2d::CCSpriteFrame* selected , cocos2d::CCSpriteFrame* disable , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector);
	static NormalButton* create(cocos2d::CCSpriteFrame* noraml , cocos2d::CCSpriteFrame* selected , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector);
	//to be different with above create
	static NormalButton* create(cocos2d::CCSpriteFrame* noraml , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector , cocos2d::CCSpriteFrame* disable);
	static NormalButton* create(cocos2d::CCSpriteFrame* noraml , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector);
protected:
	cocos2d::CCSpriteFrame* p_normalFrame;
	cocos2d::CCSpriteFrame* p_seletedFrame;
	cocos2d::CCSpriteFrame* p_disableFrame;
private:
	int _currentFrame;
};
/************************************************************************
ScaleButton
the images has 2 states:
- unselected image (must not be NULL)
- disabled image (can be NULL , if NULL it show UNSELECTED IMAGE)
when selected the unselected image will be scaled , when unselect the unselected image will be return to normal(stableScale or 1.0f)
when disabled the scale will return to normal
************************************************************************/
class ScaleButton : public Button
{
public:
	ScaleButton();
	~ScaleButton();
	bool init(cocos2d::CCSpriteFrame* noraml , float scale , cocos2d::CCSpriteFrame* disable , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector);

	virtual void setEnabled(bool isEnable);
	virtual void normalSelectedS(cocos2d::CCObject* pSender);
	virtual void normalUnselectedS(cocos2d::CCObject* pSender);

	static ScaleButton* create(cocos2d::CCSpriteFrame* noraml , float scale , cocos2d::CCSpriteFrame* disable , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector);
	static ScaleButton* create(cocos2d::CCSpriteFrame* noraml , float scale , cocos2d::CCObject* rec , cocos2d::SEL_MenuHandler selector);
protected:
	cocos2d::CCSpriteFrame* p_normalFrame;
	cocos2d::CCSpriteFrame* p_disableFrame;
	float p_scale;
private:
	int _currentFrame;
	void toScale(bool isToNoraml);
};
#endif